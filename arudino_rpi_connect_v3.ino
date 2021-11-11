#define trig 3   
#define echo 4
#define servoPin 10
#define angle  90 //servo_idle
#define active_angle  0 //servo_active
#define buzzer 2

#include <Servo.h> 
#include <math.h>
#include <stdlib.h>

#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);
const byte address[6] = "00001"; 

Servo servo; 

int values[2]; //received values

byte i,m; //for communication,m:2ndtimelaunch

float valuexrad, valueyrad, height, height_test, cond1, cond2, cond3; 
float velocity = 15.0; //Trial and error 15m/s

//initial value
String r_text = "4";

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);
   pinMode(servoPin, OUTPUT);
   pinMode(buzzer,OUTPUT); 
   servo.attach(servoPin);  
   m=0;
   valuexrad,valueyrad = 1600; //prevent to move at firsttime
   height_test = 2.0;

   radio.begin();
   radio.openReadingPipe(0, address);
   radio.setPALevel(RF24_PA_MAX); //10uF
   radio.startListening();

}

void loop(){
//radio.available()?
  if(radio.available()) {
  char text[32] = "";
  radio.read(&text, sizeof(text));
  //Serial.println(text);
  //1:Safe mode, 2:Safe mode off, 3:Launch
  r_text = text; //
 }
  if(r_text == "1"){
    noTone(buzzer); 
    servo.write(angle);
    delay(50);
  }
  else if(r_text == "2"){
    tone(buzzer,1000);
    i = 0;
    
    //read data sent by rpi(x angle*10,y angle*10)
    String data = Serial.readStringUntil('\n');
    //string data to int
    char data_char[15]; //length of data
    data.toCharArray(data_char,15);
    char *ptr = strtok(data_char, ","); //classify with ","
    while (ptr != NULL)              
    {
      values[i] = atoi(ptr);
      ptr = strtok(NULL, ","); 
      i = i+1;
    }
    //no values
    if(values[0]==0 && values[1]==0){
      values[0]=1600;
      values[1]=1600;
    }
    valuexrad = float(values[0]) * 0.0017453; //deg to rad
    valueyrad = float(values[1]) * 0.0017453; //deg to rad

    //ultrasonic_height 
    digitalWrite(trig, LOW);        
    delayMicroseconds(2);            
    digitalWrite(trig, HIGH);   
    delayMicroseconds(10);            
    digitalWrite(trig, LOW);    
    //height filtering
    height_test = pulseIn(echo, HIGH) * 0.000170;  //meter
    if (height_test >= 2.0){
    height = height_test;
    }
    /*
    //height debugging
    Serial.print("height");
    Serial.println(height_test);
    */

    //deploy condition
    cond1 = atanf((velocity) * sqrt(height) * 0.30102); //x angle
    cond2 = atanf(((velocity) * sqrt(0.20387 * height) - 4.50000) / height) - 0.78540; //y angle1
    cond3 = atanf(((velocity) * sqrt(0.20387 * height) + 4.50000) / height) - 0.78540; //y angle2

      if ((valuexrad)<=cond1){
         if ((valueyrad)>= cond2 && (valueyrad)<=cond3){
            m = m+1;
          if(m == 3){ 
            servo.write(active_angle);
            delay(50); 
            m = 0;
           }
         }
      }
      else{
        m = 0; 
      }
  }

  else if(r_text == "3"){
    noTone(buzzer);
    servo.write(active_angle);
    delay(50);  
    }

  delay(50);
}
