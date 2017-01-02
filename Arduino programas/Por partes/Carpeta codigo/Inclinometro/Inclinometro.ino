
#include <Wire.h>
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
float Acc[2];
float Gy[2];
float Angle[2];

void setup() {
 lcd.begin(); 
                                                
  lcd.clear();
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //Se calculan los angulos Y, X respectivamente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;
 
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.010) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.010) + 0.02*Acc[1];
 
   //Mostrar los valores por consola
   lcd.setCursor(9, 0);
   lcd.print("       ");
   lcd.setCursor(0, 0);
   lcd.print("Angle X: ");
   lcd.print(Angle[0]);
   lcd.setCursor(9, 1);
   lcd.print("       ");
   lcd.setCursor(0, 1);
   lcd.print("Angle Y: ");
   lcd.print(Angle[1]);
   /*
   Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
   Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.print("\n------------\n");
   */
   delay(50); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
   
}
