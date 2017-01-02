#include <Wire.h> //Librería que nos permite la comunicación por I2C (SDA línea de datos y SCL línea de reloj).
#include <LiquidCrystal_I2C.h>  //Librería de LCD Crystal por I2C
 
//Dirección I2C de la IMU
#define MPU 0x68
// Dirección Display 0x27
// Dirección EEPROM 0x57
// Dirección Reloj 0x68
// Patillaje Físico del módulo RS 4, RW 5, EN 6, D4 9, D5 10, D6 11, D7 12
// ORDEN DE LA FUNCION(addr,EN,RW,RS,D4,D5,D6,D7,BL, BL POL  );
                                                            
LiquidCrystal_I2C lcd (0x27, 16, 2);
//Ratios de conversión
#define A_R 16384.0
#define G_R 131.0
//Conversión de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;

//Ángulos
float Acc[2]; //Acumula 2 valores es decir 0,1. "[x]"es para crear una matriz en este caso de 2 valores.
float Gy[2];
float Angle[2];
float tabla;

//Constantes:
int velo;// velo=velocidad
int dire; // dire=dirección

//Definimos BTS 7960 (definimos los pines de la placa arduino a usar)
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
void setup() { 
  //definimos los pines como salida del BTS-7960 en el Set-Up
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
//--------------------------------------------------------------------------------------------------------------
 lcd.begin(); 
 lcd.clear();
  Wire.begin ();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission (true);
  Serial.begin (9600);
//Mostrar los valores por consola al comenzar el arranque:
   lcd.setCursor (0, 0); //Escribe en línea 0
   lcd.print ("INICIANDO");
      delay (2000); //retardo de 2 segundos de desaparición del texto "INICIANDO".
      lcd.clear ();
}
void loop() {
  //Leer los valores del Acelerómetro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //Se calculan los ángulos Y, X respectivamente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;

//Leer los valores del Giroscopio
   Wire.beginTransmission (MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom (MPU, 4, true); //A diferencia del Acelerómetro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
//Calculo del ángulo del Giroscopio
   Gy [0] = GyX/G_R;
   Gy [1] = GyY/G_R;
 
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
//---------------------------------------------------------------------------------------------------------------------------------------------------- 
 /*crearemos condición para restringir el ángulo de inclinación (5º de inclinación máximo)
 (giroscopio 5º motor no se inclinara más y rectificara el ángulo)*/
tabla=Angle[1]; //tabla es igual al ángulo de "Y"
 if(tabla >=5){  //para cuando el ángulo es mayor de 5º
  tabla=5;
  };
   if(tabla <=-5){ //para cunado el Angulo es menor de 5º
  tabla=-5;
  };
//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------Mapeo de mayor Ángulo más velocidad de PWM -------------------------------------------------------
velo=map((tabla*1000), 0,5000, 0,1023); // tabla * resolución, De 0 al valor de resolución, de 0 hasta el máximo 1023.
 if (velo < 0){
  velo = velo* -1;} // valor del PWM sea positivo siempre entre 0 y 1023.
  
//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------BTS-7960 loop actuación de sus funciones:----------------------------------------------------------------------------------
//Activamos el controlador BTS 7960 a través del giroscopio de su ángulo de inclinación.
//valor del sensor está en el intervalo desde 0 hasta 1023
// Si está inclinado a izquierdas el motor ira a izquierdas, sí está en medio el motor estará parado y si está a derechas el motor a derechas.
if (velo < 512)
  {
    // Rotación a derechas
    int reversePWM = -(velo - 511) / 2;
    analogWrite(LPWM_Output, 0);
    analogWrite(RPWM_Output, reversePWM);
  }
  else
  {
    // Rotación a izquierdas
    int forwardPWM = (velo - 512) / 2;
    analogWrite(LPWM_Output, forwardPWM);
    analogWrite(RPWM_Output, 0);
  }
//----------------------------------------------------------------------------------------------------------------------------------------------------
//Mostrar los valores por consola:
   lcd.setCursor(0, 0);//Escribe en línea 0
   
   lcd.print("Tabla:   ");
   lcd.print(tabla, 1);
   
   lcd.print(" "); //Borrar el último carácter
   lcd.setCursor(0, 1); //Escribe en linea 1
   
   lcd.print("Angle Y: ");
   lcd.print(Angle[1], 1); // para definir cuantos decimales es ,1 para que nos muestre un decimal.
   lcd.print(" "); //Borrar el último carácter
 //----------------------------------------------------------------------------------------------------------------------------------------------------
 //Para mostrarlo por el Monitor Serial:
   Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
   Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.print("\n------------\n");
//----------------------------------------------------------------------------------------------------------------------------------------------------
   delay(50); //Nuestra dt será, pues, 0.010, que es el intervalo de tiempo en cada medida.
}

