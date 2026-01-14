#include <Arduino.h>
//Definimos pines de entradas

const byte But1 = 22; // Boton Antihorario
const byte But2 = 23; // Boton Horario
const byte potenciometro = 34; 
const byte ir = 4;    // Sensor IR

// Salidas Motor 
const int enable = 33; 
const byte L293_A1 = 14; 
const byte L293_A2 = 32; 

// Salidas al 74HC595 (LED Driver)
const byte dataIn = 18;   // DS
const byte LatchCLK = 19; // ST_CP
const byte ShiftCLK = 21; // SH_CP

/* Representación binaria de cada LED individual

 LED_AZUL      = 0x2; // QB 
 LED_VERDE_IZQ = 0x1; // QA 
 LED_VERDE_CEN = 0x4; // QC
 LED_VERDE_DER = 0x10; // QE 
 LED_ROJO      = 0x8; // QD 
*/

//Definimos funciones para secuencia de LEDS

const byte sentido_horario[] = {0x2,0x1,0x4,0x10,0x8};   //Matriz para secuencia de bits sentido horario

const byte sentido_antihorario[] = {0x8,0x10,0x4,0x1,0x2};  //Matriz para secuencia de bits sentido antihorario

// Calculamos el tamaño de nuestros arrays para indicar los pasos bytes que se deben enviar como Data in

const int bytes_horario = sizeof(sentido_horario);
const int bytes_antihorario = sizeof(sentido_antihorario);



//Variables globales para motor y LEDs
volatile byte estado_motor = 0; // 0=OFF, 1=Horario, 2=Antihorario
int paso_actual = 0;            // Índice de la secuencia

//Creamos variables para almacenar el valor de funcion milis()
unsigned long tiempo_ultimo_step_led = 0;
const int velocidad_secuencia = 150; // Velocidad de la secuencia (ms)
unsigned long tiempo_entre_modos = 0;

// Antirrebote
unsigned long tiempo_but1 = 0;
unsigned long tiempo_but2 = 0;
const int debounce = 100;     //Tiempo antirebote en milis



// Configuramos nuestros parametros de PWM
const int freq = 2000;
const byte resolution = 12;
const byte canal = 0;

//Funciones Interrupt

void IRAM_ATTR Stop() {
    estado_motor = 0;
}

void IRAM_ATTR AntiClockwise() {
  if (millis() - tiempo_but1 > debounce) {
    if(millis()-tiempo_entre_modos > 500){
      estado_motor = 2;
      tiempo_entre_modos = millis();
    }
    tiempo_but1 = millis();
  }
}

void IRAM_ATTR Clockwise() {
  if (millis() - tiempo_but2 > debounce) {
      if(millis()-tiempo_entre_modos > 500){
      estado_motor = 1;
      tiempo_entre_modos = millis();
    }
    tiempo_but2 = millis();
  }
}

//Funciones para LEDs

void actualizarShiftRegister(byte data) {
  digitalWrite(LatchCLK, LOW);                  //No memorizamos hasta enviar la secuencia
  shiftOut(dataIn, ShiftCLK, MSBFIRST, data);
  digitalWrite(LatchCLK, HIGH);
}

void moverMotor(int velocidad) {
  ledcWrite(canal, velocidad);
}

void pararMotor() {
  ledcWrite(canal, 0);   //Duty cicle en 0
}

void setup() {
 //Configuramos nuestro analog read para que lea 12bits
 analogReadResolution(12);

  pinMode(But1, INPUT_PULLUP);
  pinMode(But2, INPUT_PULLUP);
  pinMode(ir, INPUT);

  pinMode(dataIn, OUTPUT);
  pinMode(LatchCLK, OUTPUT);
  pinMode(ShiftCLK, OUTPUT);
  pinMode(enable, OUTPUT);  
  pinMode(L293_A1, OUTPUT);
  pinMode(L293_A2, OUTPUT);



  // Interrupciones
  attachInterrupt(digitalPinToInterrupt(But1), AntiClockwise, FALLING);
  attachInterrupt(digitalPinToInterrupt(But2), Clockwise, FALLING);
  attachInterrupt(digitalPinToInterrupt(ir), Stop, FALLING);


  // PWM Motor
  ledcSetup(canal, freq, resolution);
  ledcAttachPin(enable, canal);
  //ledcSetup(enable, freq, resolution);

  // Motor inicialmente detenido
  pararMotor();
  actualizarShiftRegister(0); 
}


void loop() {
   //Leemos potenciometro
  //int potValue = analogRead(potenciometro);
  //int velocidad = map(potValue, 0, 4095, 0, 255);

  int velocidad = analogRead(potenciometro);
  //Logica si ambos sentidos se activan
  bool ambos_presionados = !digitalRead(But1) && !digitalRead(But2);


  //Logica de control motor

  if (estado_motor == 0 || ambos_presionados) {
      pararMotor();
      actualizarShiftRegister(0); // Apagar LEDs
      paso_actual = 0;            // Resetear secuencia de pasos de bits
  } 
  else if (estado_motor == 1) {
      digitalWrite(L293_A1, HIGH);      //Configuramos para sentido horario 
      digitalWrite(L293_A2, LOW);
      moverMotor(velocidad);
  } 
  else if (estado_motor == 2) {
      digitalWrite(L293_A1, LOW);      //Configuramos para sentido antihorario 
      digitalWrite(L293_A2, HIGH);
      moverMotor(velocidad);
  }

  if (estado_motor != 0) {
      //Iniciamos funcion de tiempo, sin bloquear el resto del code
      if (millis() - tiempo_ultimo_step_led > velocidad_secuencia) {
          tiempo_ultimo_step_led = millis();
          byte leds_a_enviar = 0;

          if (estado_motor == 1) {
              //Iniciamos array sentido horario
              leds_a_enviar = sentido_horario[paso_actual];
              paso_actual++;
              if (paso_actual >= bytes_horario) 
              paso_actual = 0;
          } 
          else if (estado_motor == 2) {
              // Iniciamos array Antihorario
              leds_a_enviar = sentido_antihorario[paso_actual];
              paso_actual++;
              if (paso_actual >= bytes_antihorario) 
              paso_actual = 0;
          }

          // Enviamos los datos como bits a nuestra funcion de subida de data
          actualizarShiftRegister(leds_a_enviar);
      }
  }
}