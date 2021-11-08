#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); 
const byte address[6] = "00001";
int safe_mode_ON =2;
int safe_mode_OFF =3;
int Fire =4;
char text;

void setup() {
  pinMode(safe_mode_ON,INPUT);
  pinMode(safe_mode_OFF,INPUT);
  pinMode(Fire,INPUT);
  radio.begin();
  radio.openWritingPipe(address); 
  radio.setPALevel(RF24_PA_MAX); 
  //RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX
  //10uF compatitor 
  radio.stopListening();  
  Serial.begin(9600);  
}

void loop() {
  /*
  Serial.print("SAFEMODE_ON ");
  Serial.println(digitalRead(safe_mode_ON));
  Serial.print("SAFEMODE_OFF ");
  Serial.println(digitalRead(safe_mode_OFF));
  Serial.print("Fire ");
  Serial.println(digitalRead(Fire));
*/

  if(digitalRead(safe_mode_ON)== 1){
  char text[] ="1";
  radio.write(&text, sizeof(text)); 
  //Serial.println("1");
  }
  else if(digitalRead(safe_mode_OFF)==1){
  char text[] ="2";
  radio.write(&text, sizeof(text)); 
  //Serial.println("2");
  }
  else if(digitalRead(Fire)==1){
  char text[] ="3";
  radio.write(&text, sizeof(text)); 
  //Serial.println("3");
  }
}