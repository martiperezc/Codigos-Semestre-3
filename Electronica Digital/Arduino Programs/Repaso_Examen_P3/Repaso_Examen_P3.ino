const int Led_out= 23;
const byte b1=14;
const byte pot= 36;

volatile unsigned long tiempoantirebote= 0;
volatile byte estado_Led =LOW;
unsigned long tiemposerial= 0;
int val_pot =0;

//Definimos configuraciones de señal PWM


const int frecuencia= 1000;
const byte resolucion = 12;




void IRAM_ATTR Control_led(){
  if(millis()-tiempoantirebote>200){
    tiempoantirebote=millis();
    estado_Led = !estado_Led;
  }
}




void setup() {
  Serial.begin(115200);
  Serial.print("Iniciando serial..");
  Serial.println(); 
  //analogReadResolution(12); //Configuramos resolucion de 12 bits

  pinMode(Led_out, OUTPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(pot, INPUT);
  attachInterrupt(digitalPinToInterrupt(b1), Control_led, FALLING);

  ledcAttach(Led_out, frecuencia, resolucion);


}

void loop() {
  val_pot = analogRead(pot);
  ledcWrite(Led_out, val_pot);

  if(millis()-tiemposerial>500){
    tiemposerial=millis();
    Serial.println(estado_Led);
    Serial.println(val_pot);
    Serial.println(); //Imprime salto de linea
  }



  digitalWrite(Led_out,estado_Led);
}





