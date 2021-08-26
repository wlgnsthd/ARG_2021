#define trig 8   
#define echo 9
#include <Servo.h> 

Servo servo; 
int servoPin = 10;
int angle = 0; 
int active_angle = 90;

float values[4];//[각도3,속도2,안전모드해제1,고도2]

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
}


void loop()
{  if (Serial.available() > 0){
//라즈베리파이에서 각도(0~90.0) 속도(0~9.0) 안전모드해제(0,1) 읽어오기 "56.5/9.5/0":6자리
    String data = Serial.readStringUntil('\n');
    Serial.print("From rpi : ");
    Serial.println(data);
//받은 데이터 분류 csv   
    values[0] = data[0:2]
    values[0] = data[0:2]
//초음파  
    float duration, distance;    
    digitalWrite(trig, LOW);        
    delayMicroseconds(2);            
    digitalWrite(trig, HIGH);   
    delayMicroseconds(10);            
    digitalWrite(trig, LOW);        
 
    duration = pulseIn(echo, HIGH);  
 
    distance = duration * 0.000170;
 
    Serial.print("distance: ");
    Serial.print(distance); 
    Serial.println("m");

//서보 작동해서 투하
    if (){
        servo.write(active_angle); 
        delay(500);
        servo.write(angle);
        delay(500);
   }
 } 
}
