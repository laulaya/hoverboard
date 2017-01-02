  char val;
  int on_led=12;
   
void setup() {
Serial.begin(9600);//aqui establecemos la velocidad
pinMode(on_led,OUTPUT);
digitalWrite(on_led,HIGH);//El relé comenzara apagado hasta que reciba la señal(del LOOP)generada por la app del bluetooth.
}
void loop() {
if(Serial.available())
val=Serial.read();
if(val=='1'){
  digitalWrite(on_led,LOW);}
  if(val=='0'){
    digitalWrite(on_led,HIGH);}
}
//los valores 1 y 0 estan puesto a la inversa porque el relé actua a la inversa.
// Es decir 1= low y 0= high
