#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

// --- 1. CREDENCIALES Y CONFIGURACIÓN --- (Sin cambios)
const char* ssid = "NETLIFE-JHONNY";
const char* password = "1754280269";
const char* mqtt_server = "192.168.100.18";
const int   mqtt_port = 1883;
const char* mqtt_topic_aggregado = "parking/estado";
const bool mqtt_retain_individual = true;

// --- 2. PINES --- (Sin cambios)
const int PIN_SENSOR_P1 = 13; 
const int PIN_SENSOR_P2 = 14;
const int PIN_SENSOR_P3 = 27;
const int PIN_SENSOR_P4 = 26;
const int PIN_SENSOR_P5 = 19;
const int PIN_SENSOR_P6 = 21;
const int PIN_SENSOR_ENTRADA = 4;
const int PIN_SENSOR_SALIDA = 18;
const int PIN_SERVO_ENTRADA = 25;
const int PIN_SERVO_SALIDA = 33;
const int PIN_NEOPIXEL_1 = 22;
const int PIN_NEOPIXEL_2 = 23;

// --- 3. OBJETOS ---
WiFiClient espClient;
PubSubClient client(espClient);

// Tira 1: 4 LEDs (PA1, PA2, PA3, PB3) | Tira 2: 2 LEDs (PB1, PB2)
Adafruit_NeoPixel tira1(4, PIN_NEOPIXEL_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel tira2(2, PIN_NEOPIXEL_2, NEO_GRB + NEO_KHZ800);

Servo servoEntrada;
Servo servoSalida;

// --- 4. ESTADO SENSORES Y TOPICS ---
const int numSensores = 6;
int pinesSensores[numSensores] = {PIN_SENSOR_P1, PIN_SENSOR_P2, PIN_SENSOR_P3, PIN_SENSOR_P4, PIN_SENSOR_P5, PIN_SENSOR_P6};
int estadoSensores[numSensores]; 

const char* topics[numSensores] = {
  "parking/piso1/PB01", "parking/piso1/PB02", "parking/piso1/PB03",
  "parking/piso2/PA01", "parking/piso2/PA02", "parking/piso2/PA03",
};

// --- 5. TIMERS Y SERVOS ---
unsigned long lastMsg = 0;
unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 100;
const unsigned long mqttInterval   = 1000;

unsigned long tiempoServoEntrada = 0, tiempoServoSalida = 0;
bool barreraEntradaAbierta = false, barreraSalidaAbierta = false;

// Sincronizamos posiciones iniciales con el setup (90 grados)
int posServoEntrada = 90;
int objetivoServoEntrada = 90;
unsigned long lastMoveEntrada = 0;

int posServoSalida = 90;
int objetivoServoSalida = 90;
unsigned long lastMoveSalida = 0;

// --- 6. COLORES ---
uint32_t COLOR_ROJO_T1, COLOR_VERDE_T1, COLOR_ROJO_T2, COLOR_VERDE_T2;

// --- 7. FUNCIONES WIFI / MQTT --- (Sin cambios solicitados)
void setup_wifi() {
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < 15000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Conectado");
  }
}

bool reconnect() {
  if (WiFi.status() != WL_CONNECTED) return false;
  String clientId = "ESP32_" + String((uint32_t)ESP.getEfuseMac(), HEX);
  if (client.connect(clientId.c_str())) {
    Serial.println("Conectado al broker MQTT");
    return true;
  }
  return false;
}

bool publishSafe(const char* topic, const char* payload, bool retained = true) {
  if (!client.connected()) reconnect();
  return client.publish(topic, payload, retained);
}

// --- 8. GESTIÓN SERVOS (Corrección de variables de tiempo) ---
void gestionarServos() {
  unsigned long now = millis();

  // --- SERVO ENTRADA ---
  if (digitalRead(PIN_SENSOR_ENTRADA) == LOW && !barreraEntradaAbierta) {
    objetivoServoEntrada = 0;
    barreraEntradaAbierta = true;
    tiempoServoEntrada = now;
  }
  if (barreraEntradaAbierta && digitalRead(PIN_SENSOR_ENTRADA) == LOW) {
    tiempoServoEntrada = now;
  }
  if (barreraEntradaAbierta && digitalRead(PIN_SENSOR_ENTRADA) == HIGH && (now - tiempoServoEntrada >= 10000)) {
    objetivoServoEntrada = 90;
    barreraEntradaAbierta = false;
  }

  // Motor Físico Entrada (Usa su propio temporizador: lastMoveEntrada)
  if (now - lastMoveEntrada >= 15) {
    lastMoveEntrada = now;
    if (posServoEntrada > objetivoServoEntrada) posServoEntrada -= 5;
    else if (posServoEntrada < objetivoServoEntrada) posServoEntrada += 5;
    servoEntrada.write(posServoEntrada);
  }

  // --- SERVO SALIDA ---
  if (digitalRead(PIN_SENSOR_SALIDA) == LOW && !barreraSalidaAbierta) {
    objetivoServoSalida = 180;
    barreraSalidaAbierta = true;
    tiempoServoSalida = now;
  }
  if (barreraSalidaAbierta && digitalRead(PIN_SENSOR_SALIDA) == LOW) {
    tiempoServoSalida = now; 
  }
  if (barreraSalidaAbierta && (now - tiempoServoSalida > 10000)) {
    objetivoServoSalida = 90;
    barreraSalidaAbierta = false;
  }

  // Motor Físico Salida (Usa su propio temporizador: lastMoveSalida)
  if (now - lastMoveSalida >= 15) {
    lastMoveSalida = now;
    if (posServoSalida < objetivoServoSalida) posServoSalida += 5;
    else if (posServoSalida > objetivoServoSalida) posServoSalida -= 5;
    servoSalida.write(posServoSalida);
  }
}

// --- 9. SETUP ---
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  tira1.begin(); tira1.show();
  tira2.begin(); tira2.show();

  COLOR_ROJO_T1 = tira1.Color(80,0,0);
  COLOR_VERDE_T1 = tira1.Color(0,80,0);
  COLOR_ROJO_T2 = tira2.Color(80,0,0);  
  COLOR_VERDE_T2 = tira2.Color(0,80,0);

  servoEntrada.attach(PIN_SERVO_ENTRADA);
  servoSalida.attach(PIN_SERVO_SALIDA);
  servoEntrada.write(90);
  servoSalida.write(90);

  for (int i = 0; i < numSensores; i++) pinMode(pinesSensores[i], INPUT);
  pinMode(PIN_SENSOR_ENTRADA, INPUT);
  pinMode(PIN_SENSOR_SALIDA, INPUT);
}

// --- 10. LOOP PRINCIPAL ---
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  gestionarServos();

  unsigned long now = millis();

  // Lectura y actualización de LEDs según TABLA
  if (now - lastSensorRead >= sensorInterval) {
    lastSensorRead = now;

    for (int i = 0; i < numSensores; i++) {
      int lectura = digitalRead(pinesSensores[i]); 
      int ocupado = (lectura == LOW) ? 1 : 0; // 1 = Ocupado (Rojo), 0 = Libre (Verde)
      estadoSensores[i] = ocupado;
    }

    // --- ASIGNACIÓN SEGÚN TABLA ---
    // IR1 (i=0) -> PB1 -> NP2[0]
    if (estadoSensores[0]) tira2.setPixelColor(0, COLOR_ROJO_T2);
    else tira2.setPixelColor(0, COLOR_VERDE_T2);

    // IR2 (i=1) -> PB2 -> NP2[1]
    if (estadoSensores[1]) tira2.setPixelColor(1, COLOR_ROJO_T2);
    else tira2.setPixelColor(1, COLOR_VERDE_T2);

    // IR3 (i=2) -> PB3 -> NP1[3]
    if (estadoSensores[2]) tira1.setPixelColor(3, COLOR_ROJO_T1);
    else tira1.setPixelColor(3, COLOR_VERDE_T1);

    // IR4 (i=3) -> PA1 -> NP1[0]
    if (estadoSensores[3]) tira1.setPixelColor(0, COLOR_ROJO_T1);
    else tira1.setPixelColor(0, COLOR_VERDE_T1);

    // IR5 (i=4) -> PA2 -> NP1[1]
    if (estadoSensores[4]) tira1.setPixelColor(1, COLOR_ROJO_T1);
    else tira1.setPixelColor(1, COLOR_VERDE_T1);

    // IR6 (i=5) -> PA3 -> NP1[2]
    if (estadoSensores[5]) tira1.setPixelColor(2, COLOR_ROJO_T1);
    else tira1.setPixelColor(2, COLOR_VERDE_T1);

    tira1.show();
    tira2.show();
  }

  // Publicar JSON MQTT (Sin cambios)
  if (now - lastMsg >= mqttInterval) {
    lastMsg = now;
    StaticJsonDocument<256> doc;
    doc["PB_1"] = estadoSensores[0]; doc["PB_2"] = estadoSensores[1]; doc["PB_3"] = estadoSensores[2];
    doc["PA_1"] = estadoSensores[3]; doc["PA_2"] = estadoSensores[4]; doc["PA_3"] = estadoSensores[5];
    char buffer[256];
    serializeJson(doc, buffer, sizeof(buffer));
    publishSafe(mqtt_topic_aggregado, buffer, true);
  }
}