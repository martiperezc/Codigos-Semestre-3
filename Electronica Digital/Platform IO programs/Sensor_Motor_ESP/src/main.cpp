
#include <Arduino.h>


//Definimos los pines de entradas
const byte ir =4;    //Sensor IR
const byte But1 = 19; // Entrada de boton sentido antihorario
const byte But2 = 21; // Entrada de boton sentido horario

//Definimos los pines de salidas

const byte Led_inf = 22, Led_sup = 23, enable = 14,  L293_A1= 32,  L293_A2= 13;

byte estado_motor =0;   //Definimos variable que almacenara el estado del motor donde 0=OFF 1= ON Sentido Horario 2= ON Sentido Antihorario




void setup() {
  //Configuramos nuestros pines de entrada
  pinMode(ir, INPUT);
  pinMode(But1, INPUT);
  pinMode(But2, INPUT);
  //Configuramos nuestros pines de salida

  pinMode(Led_inf, OUTPUT);
  pinMode(Led_sup, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(L293_A1, OUTPUT);
  pinMode(L293_A2, OUTPUT);


}


//A= Señal de LED IR, B= Boton sentido antihorario, C= Sentido Horario

void loop() {

  //Creamos variables del tipo BOOl 1 o 0 para almacenar el valor de las entradas

  //Validamos la logica para evitar el efecto bouncing
  bool data_ir = digitalRead(ir);
  bool data_b1 = digitalRead(But1);
  bool data_b2 = digitalRead(But2);

  //Configuramos una espera de 20ms que finje ser un capacitor
  delay(20);


    //Una vez trascurrido el tiempo anti-bouncing, iniciamos con la lectura
    bool A = !digitalRead(ir);               
    bool B = !digitalRead(But1);      //Nota: Negamos las salidas debido a que los sensores como los botones envian un estado activo en bajo (LOW)
    bool C = !digitalRead(But2);


    //Escribimos logica para salida A1 basada en algebra bool
    // Nota: !A significa A'
    bool val_A1 = !A && !B && C;     //Esta expresion es equivalente a: (A'B'C) AND de 3


    // Logica para salida A2 
    bool val_A2 = !A && B && !C;    //Equivalente a: (A'BC') AND de 3.

    // Logica para salida enable:        


    // En lenguaje C, el operador ^ es XOR
    bool val_Enable = !A && (B ^ C);         //Equivalente a: A'(B XOR C)  AND y XOR

    //Enviamos los valores de A1 A2 y enable al L293D
    digitalWrite( L293_A1, val_A1);
    digitalWrite(L293_A2, val_A2);
    digitalWrite(enable, val_Enable);





    //Secuencia de LED cuando se activa IR (Parada de emergencia)
  
    if (A==HIGH) {       //Evaluamos si IR = 1 (Activo)
    

     //Creamos secuencia de encendido en diagonal de los leds 
  
    digitalWrite(Led_inf, HIGH);        //Diagonal inferior de leds encendida.
    digitalWrite(Led_sup, LOW);         //Diagonal superior de leds encendida.
    
    delay(200); // Mantenemos este estado durante 200ms

    //Invertimos el encendido de los leds

    digitalWrite(Led_inf, LOW);
    digitalWrite(Led_sup, HIGH);
    
    delay(200); //Esperamos otros 200ms antes de repetir la secuencia
    
  } else {   //Si no existe parada de emergencia, entonces todos los leds=off
    digitalWrite(Led_inf, LOW);
    digitalWrite(Led_sup, LOW);
  }
}

