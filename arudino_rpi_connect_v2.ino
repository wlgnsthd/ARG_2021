#define trig 8   
#define echo 9
#define servoPin 10
#define angle  90 //servo_idle
#define active_angle  0 //servo_active
//#define buzzer  3
//#define receiver_pin 5

#include <Servo.h> 
#include <math.h>
#include <stdlib.h>

Servo servo; 

int values[2]; //received values

byte i,m; //for communication,m:2ndtimelaunch

float valuexrad, valueyrad, height, height_test, cond1, cond2, cond3; 
float velocity = 15.0; //Trial and error 15m/s

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);
   pinMode(servoPin, INPUT);   
//   pinMode(buzzer, OUTPUT);
   //pinMode(receiver_pin,INPUT);
   m=0;
   valuexrad,valueyrad = 1600; //prevent to move at firsttime
   height_test = 2.0;
   //servo.write(angle);
   delay(3000);

}

void loop(){
  
  i = 0;
  //height = 5.00; //Trial and error

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
   
  //deploy condition
  cond1 = atanf((velocity) * sqrt(height) * 0.30102); //x angle
  cond2 = atanf(((velocity) * sqrt(0.20387 * height) - 4.50000) / height) - 0.78540; //y angle1
  cond3 = atanf(((velocity) * sqrt(0.20387 * height) + 4.50000) / height) - 0.78540; //y angle2

//Serial.println(values[0]);
//Serial.println(values[1]);
//Serial.println(valuexrad);
//Serial.println(valueyrad);
//Serial.println(cond1);
//Serial.println(cond2);
//Serial.println(cond3);
//Serial.println(height);
//Serial.println(m);
//Serial.println("_______");


  //verify safemode -> too much time
  //safe_mode = pulseIn(receiver_pin,HIGH);        
    if ((valuexrad)<=cond1){
       if ((valueyrad)>= cond2 && (valueyrad)<=cond3){
          m = m+1;
        if(m == 3){
//Serial.println("fire");
          pinMode(servoPin, OUTPUT);
          servo.attach(servoPin); 
          servo.write(active_angle); 
          delay(2000); //
          servo.write(angle);//
          delay(1000);//
          pinMode(servoPin, INPUT);
          m = 0;
         }
       }
    }            
}