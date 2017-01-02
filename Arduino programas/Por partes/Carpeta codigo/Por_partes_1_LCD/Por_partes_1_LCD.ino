//Apartado para ver que la LCD funciona sola.
#include <Wire.h>//Libreria que nos permite la comunicación por I2C,(SDA linea de datos y SCL linea de reloj).
#include <LiquidCrystal_I2C.h>

//Direccion I2C de la IMU
#define MPU 0x68
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

 lcd.begin(); 
 lcd.clear();
}

void loop() {
 //Mostrar los valores por consola:
   lcd.setCursor(0, 0);//Escribe en linea 0
   
   lcd.print("Hola mundo:   ");    
   lcd.print(" ");//Borrar el último caracter
   
   lcd.setCursor(0, 1);//Escribe en linea 1
   lcd.print("Adios mundo: ");
   lcd.print(" ");//Borrar el último caracter
delay(50); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}


