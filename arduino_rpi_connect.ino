#define trig 8   
#define echo 9
#include <Servo.h> 
#include <math.h>
#include <stdlib.h>

Servo servo; 
const int servoPin = 10;
const int angle = 0; 
const int active_angle = 90;

const int buzzer = 3;
const int receiver_pin = 5;
int values[2]; 
int i,safe_mode;
unsigned long t1, t2;
float duration, velocity, distance1, distacne2, delt , height, height_test, cond1, cond2, cond3; 

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
   pinMode(buzzer, OUTPUT);
   pinMode(receiver_pin,INPUT);
}


void loop()
{ 
  //initialize
  i = 0;
  //read data sent by rpi(|x angle*10|/y angle*10/velocity*10)
  //String data = Serial.readStringUntil('\n');
  String data = "468,-789";
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
   
   //ultrasonic_height 
  digitalWrite(trig, LOW);        
  delayMicroseconds(2);            
  digitalWrite(trig, HIGH);   
  delayMicroseconds(10);            
  digitalWrite(trig, LOW);        
  duration = pulseIn(echo, HIGH);  
  //height filtering
  height_test = duration * 0.000170;
  if(height_test >= 2.0 && height_test <= 8.0)
  {
  height = height_test; //reliable height value
  }
  else
  {
  Serial.println("unreliable ultrasonic sensor value occured");      
  }
   
   //velocity
   distance1 = tan((float(values[1])*0.1+0.78540)*height;
   t1 = millis() //late 1100
   delt = float(t1-t2) /1000.0;
   velocity = (distance1 - distance2) /(delt);
   distance2 = tan(float(values[1])*0.1+0.78540)*height;
   t2 = millis(); //early 1000


   
  //deploy condition
  cond1 = atanf((float(values[2])*0.1)*sqrt(0.20408*distance)*0.66667);
  cond2 = atanf(((float(values[2])*0.1)*sqrt(0.20408*distance)-4.5)/distance)-0.78540;
  cond3 = atanf(((float(values[2])*0.1)*sqrt(0.20408*distance)+4.5)/distance)-0.78540;

 //verify safemode
  safe_mode = pulseIn(receiver_pin,HIGH);
  
  Serial.println("----VALUES----");
  Serial.print("SAFE_MODE : ");    
  Serial.println(safe_mode);
  Serial.print("|X_ANGLE| : ");    
  Serial.println(values[0]*0.1);
  Serial.print("Y_ANGLE : ");
  Serial.println(values[1]*0.1);
  Serial.print("VELOCITY : ");
  Serial.println(values[2]*0.1);
  Serial.print("HEIGHT : ");
  Serial.println(distance);
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
    if ((float(values[0])*0.001745)<=cond1){ 
      tone(buzzer, 1000);
        if ((float(values[1])*0.001745)>= cond2 && (float(values[1])*0.001745)<=cond3){ 
          servo.write(active_angle); 
          delay(2000);
          servo.write(angle);
          delay(500);
          }
      } 
   //}
}
