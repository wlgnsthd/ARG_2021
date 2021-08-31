#define trig 8   
#define echo 9
#define servoPin 10
#define angle  0
#define active_angle  90
#define buzzer  3
//#define receiver_pin 5

#include <Servo.h> 
#include <math.h>
#include <stdlib.h>

Servo servo; 

int values[2]; 
byte i;
//short safe_mode;
unsigned long t1, t2;
float valuexrad, valueyrad, duration, velocity, distance1, distance2, height, height_test, cond1, cond2, cond3; 

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
   pinMode(buzzer, OUTPUT);
   //pinMode(receiver_pin,INPUT);
   distance1 = 0;
   distance2 = 0;
}


void loop()
{ 
  //initialize
  i = 0;
  height = 0.00;
  distance1 = 0.00;//distance2;
  t1 = t2;
  noTone(buzzer);
  t2 = millis(); 
  //read data sent by rpi(|x angle*10|,y angle*10)
  //String data = Serial.readStringUntil('\n');
  String data = "121,201"; //test data
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
  if(height_test >= 1.5 && height_test <= 8.0) //1.5~8.0m
  {
   height = height_test; //reliable height value
  }
  //height = ; //test
  
  //velocity - order is important
  distance2 = tan(valueyrad + (0.78540)) * height; //0.78540rad = 45deg
  velocity = (distance1 - distance2) * 1000 / float(t2-t1);
   
  //deploy condition
  cond1 = atanf((velocity) * sqrt(height) * 0.30102); //x angle
  cond2 = atanf(((velocity) * sqrt(0.20387 * height) - 4.50000) / height) - 0.78540; //y angle1
  cond3 = atanf(((velocity) * sqrt(0.20387 * height) + 4.50000) / height) - 0.78540; //y angle2

  //verify safemode ->too much time
  //safe_mode = pulseIn(receiver_pin,HIGH);

  Serial.println("----VALUES----");
 //Serial.print("SAFE_MODE : ");    
 // Serial.println(safe_mode);
  Serial.print("|X_ANGLE| : ");    
  Serial.print(values[0]*0.1);
  Serial.print("  in rad : ");
  Serial.println(valuexrad);
  Serial.print("Y_ANGLE : ");
  Serial.print(values[1]*0.1);
  Serial.print("  in rad : ");
  Serial.println(valueyrad);
  Serial.print("HEIGHT : ");
  Serial.print(height);
  Serial.print(" test : ");
  Serial.println(height_test);
  Serial.print("Time gap : ");    
  Serial.println(t2-t1);
  Serial.print("Distance gap : ");    
  Serial.print(distance2 - distance1);
  Serial.print("  Distance 2 : ");  
  Serial.println(distance2);
  Serial.print("VELOCITY : ");    
  Serial.println(velocity);
  Serial.println("----CONDITION----");
  Serial.print("X_ANGLE COND : ");
  Serial.println(cond1);
  Serial.print("Y_ANGLE MIN: ");
  Serial.println(cond2);              
  Serial.print("Y_ANGLE MAX: ");         
  Serial.println(cond3);      
  Serial.println("----------------");                 

//deploy
 //if(safe_mode >= 1500){ //from rc receiver
    if (valuexrad<=cond1) //compare with x_angle
    { 
      tone(buzzer, 1000);
      Serial.println("Good x!");
        if ((valueyrad)>= cond2 && (valueyrad)<=cond3) //Compare with y_angle
        {            
  //one more time!         
  i = 0;
  height = 0.00;
  distance1 = distance2;
  t1 = t2;
   
  //time when received           
  t2 = millis();         
  //read data sent by rpi(|x angle*10|,y angle*10)
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
  if(height_test >= 1.5 && height_test <= 8.0) //1.5~8.0m
  {
   height = height_test; //reliable height value
  }
  
  //velocity - order is important
  distance2 = tan(valueyrad + (0.78540)) * height; //0.78540rad = 45deg
  velocity = (distance1 - distance2) * 1000 / float(t2-t1);
   
  //deploy condition
  cond1 = atanf((velocity) * sqrt(height) * 0.30102); //x angle
  cond2 = atanf(((velocity) * sqrt(0.20387 * height) - 4.50000) / height) - 0.78540; //y angle1
  cond3 = atanf(((velocity) * sqrt(0.20387 * height) + 4.50000) / height) - 0.78540; //y angle2
           
   if (valuexrad<=cond1){
       if ((valueyrad)>= cond2 && (valueyrad)<=cond3){
          //Serial.println("Fire!");
          servo.write(active_angle); 
          delay(2000);
          servo.write(angle);
          delay(500);
       }
   }

         }
       else //Good x , bad y
       {
         Serial.println("Adjust y!");
        }
     }
     else
     {
       Serial.println("Adjust x!");
       if ((valueyrad)>= cond2 && (valueyrad)<=cond3) //compare with y_angle
       {
        Serial.println("Good y!");
       }
       else
       {
        Serial.println("Adjust y!");
       }
     }
   //}
}
