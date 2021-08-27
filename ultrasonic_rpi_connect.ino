#define trig 8   
#define echo 9
#include <Servo.h> 
#include <math.h>


Servo servo; 
const int servoPin = 10;
int angle = 0; 
int active_angle = 90;

const int buzzer = 3;
const int receiver_pin = 5;
int values[4];//[x각도3,y각도3,속도2,고도2]
float realvalues[4];
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
{  //if (Serial.available() > 0){
//라즈베리파이에서 /x축각도(0~90.0)/y축각도(0~90.0)/속도(0~9.0)/ [45.0/45.0/9.0/0]9자리
  //String data = Serial.readStringUntil('\n');
  String data = "45045068";
   //string data에서 int변수로 변환
  long int main_value = data.toInt();
  values[0] = main_value/100000; //x축각도450 deg
  values[1] = main_value/100-values[0]*1000; //y축각도450 deg
  values[2] = main_value-(values[0]*100000+values[1]*100); //속도 6.8
  realvalues[0] = values[0]*0.001745; //x축각도 rad
   realvalues[1] = values[1]*0.001745; // y축각도 rad
    realvalues[2] = values[2]*0.1; //속도m/s
   float cond1 = atan(realvalues[2]*sqrt(0.20408*realvalues[3])*0.66667);
   float cond2 = atan((realvalues[2]*sqrt(0.20408*realvalues[3])-4.5)/realvalues[3])-0.78540;
   float cond3 = atan((realvalues[2]*sqrt(0.20408*realvalues[3])+4.5)/realvalues[3])-0.78540;

 //안전모드 확인
   int safe_mode = pulseIn(receiver_pin,HIGH);
 
//초음파 고도 
    float duration, distance;    
    digitalWrite(trig, LOW);        
    delayMicroseconds(2);            
    digitalWrite(trig, HIGH);   
    delayMicroseconds(10);            
    digitalWrite(trig, LOW);        
 
    duration = pulseIn(echo, HIGH);  
 
    distance = duration * 0.000170;
    realvalues[3] = distance; //고도 m
    Serial.print("distance: ");
    Serial.print(distance); 
    Serial.println("m");
Serial.print("realvalues[0]x각도 : ");    
Serial.println(realvalues[0]);
Serial.print("realvalues[1]y각도 : ");
Serial.println(realvalues[1]);
Serial.print("realvalues[2]속도 : ");
Serial.println(realvalues[2]);
Serial.print("realvalues[3]고도 : ");
Serial.println(realvalues[3]);
Serial.println("----condition----");
Serial.print("x각도   오차 : ");
Serial.println(cond1);
Serial.print("y각도 최소값 : ");
Serial.println(cond2);              
Serial.print("y각도 최대값 : ");          
Serial.println(cond3);                       

//서보 작동해서 투하
  //if(safe_mode >= 1500){ //값 확인!
    if (realvalues[0]<=cond1){ //투하조건2
     tone(buzzer, 1000);
     if (realvalues[1]>= cond2 && realvalues[1]<=cond3){ //투하조건1
        //servo.write(active_angle); 
        //delay(2000);
        //servo.write(angle);
        //delay(500);
     }
    } 
   //}
//available } 
}
