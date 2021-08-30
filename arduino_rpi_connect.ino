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
const int buzzer2 = 4; 
const int receiver_pin = 5;
int values[2]; 
int i,safe_mode;
unsigned long t1, t2;
float valuexrad,valueyrad,duration, velocity, distance1, distance2, delt , height, height_test, cond1, cond2, cond3; 

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
   pinMode(buzzer, OUTPUT);
   pinMode(buzzer2, OUTPUT);

   pinMode(receiver_pin,INPUT);
   delt=0;
   distance1=0;
   distance2=0;
}


void loop()
{ 
  //initialize
  i = 0;
  distance1 = distance2;
  t1 =t2;
  //read data sent by rpi(|x angle*10|,y angle*10)
  //String data = Serial.readStringUntil('\n');
  String data = "001,-201";
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
  valuexrad = float(values[0])*0.0017453;
  valueyrad = float(values[1])*0.0017453;
   //ultrasonic_height 
  digitalWrite(trig, LOW);        
  delayMicroseconds(2);            
  digitalWrite(trig, HIGH);   
  delayMicroseconds(10);            
  digitalWrite(trig, LOW);        
  duration = pulseIn(echo, HIGH);  
  //height filtering
  height_test = duration * 0.000170;
  if(height_test >= 1.5 && height_test <= 8.0)
  {
  height = height_test; //reliable height value
  }
  
  //velocity - order is important
  distance2 = tan(valueyrad+0.78540)*height;
  t2 = millis()-12; //early 1000
  velocity = (distance2 - distance1)*1000/float(t2-t1);
   
  //deploy condition
  cond1 = atanf((velocity)*sqrt(0.20408*height)*0.66667);
  cond2 = atanf(((velocity)*sqrt(0.20408*height)-4.5)/height)-0.78540;
  cond3 = atanf(((velocity)*sqrt(0.20408*height)+4.5)/height)-0.78540;

  //verify safemode
  safe_mode = pulseIn(receiver_pin,HIGH);

  Serial.println("----VALUES----");
  Serial.print("SAFE_MODE : ");    
  Serial.println(safe_mode);
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
  Serial.print(" test : );
  Serial.println(height_test);
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
    if (valuexrad<=cond1){ 
      tone(buzzer, 1000);
      Serial.println("Good x!");
        if ((valueyrad)>= cond2 && (valueyrad)<=cond3){ 
          tone(buzzer2,800);
          Serial.println("Fire!");
          servo.write(active_angle); 
          delay(2000);
          servo.write(angle);
          delay(500);
          }else{
          Serial.println("Adjust y!");}
      }else{Serial.println("Adjust x!");
      if ((valueyrad)>= cond2 && (valueyrad)<=cond3){
        Serial.println("Good y!");
        }else{Serial.println("Adjust y!");
      }
     }
   //}
}
