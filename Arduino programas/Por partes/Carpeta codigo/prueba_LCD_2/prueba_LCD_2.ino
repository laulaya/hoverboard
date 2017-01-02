#include <Wire.h>//Libreria que nos permite la comunicación por I2C,(SDA linea de datos y SCL linea de reloj).
#include <LiquidCrystal_I2C.h>
 
//Direccion I2C de la IMU
#define MPU 0x68

// direccion Display 0x27
// direccion EEPROM 0x57
// direccion Reloj 0x68
// Patillaje Fisico del modulo RS 4, RW 5, EN 6, D4 9, D5 10, D6 11, D7 12
// ORDEN DE LA FUNCION(addr,EN,RW,RS,D4,D5,D6,D7,BL, BL POL  );
                                                            
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Ratios de conversion
#define A_R 16384.0
#define G_R 131.0
 
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779
 
//MPU-6050 da los valores en enteros de 16 bits
//Valores sin refinar
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
 
//Angulos
float Acc[2];//Acumula 2 valores es decir 0,1. "[x]"es para crear una matriz en este caso de 2 valores.
float Gy[2];
float Angle[2];
float tabla;

//Constantes:
int velo;// velo=velocidad
int dire;// dire=dirección

//Definimos BTS 7960 (definimos los pines de la placa arduino a usar)
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
//----------------------------------------------------------------------------------------------------------------------------------------------------
void setup() { 
  //definimos los pines como salida del BTS-7960 en el Set-Up
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
//--------------------------------------------------
 lcd.begin(); 
 lcd.clear();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
   //Mostrar los valores por consola:
   lcd.setCursor(0, 0);//Escribe en linea 0
   
   lcd.print("INICIANDO:");
      Serial.begin(9600);
     delay(2000);//retardo de desaparición del texto "INICIANDO" 2 segundos
   lcd.clear();
}
void loop() {
   //Mostrar los valores por consola:
   lcd.setCursor(0, 0);//Escribe en linea 0
   
   lcd.print("Tabla:   ");
   lcd.print(tabla, 1);
   
   lcd.print(" ");//Borrar el último caracter
   lcd.setCursor(0, 1);//Escribe en linea 1
}
