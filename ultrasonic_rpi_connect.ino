#define trig 8   
#define echo 9
#include <Servo.h> 

Servo servo; 
int servoPin = 10;
int angle = 0; 
int active_angle = 90;

float values[5];//[x각도3,y각도3,속도2,안전모드해제1,고도2]

void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
}


void loop()
{  if (Serial.available() > 0){
//라즈베리파이에서 x축각도(0~90.0)/y축각도(0~90.0)/속도(0~9.0)/안전모드해제(0,1) [45.0/45.0/9.0/0]9자리
    String data = Serial.readStringUntil('\n');
    Serial.print("From rpi : ");
    Serial.println(data);
//받은 데이터 분류 csv   
    values[0] = data[0]+data[1]+data[2]; //x각도
    values[1] = data[3]+data[4]+data[5]; //y각도
    values[2] = data[6]+data[7]; //속도
    values[3] = data[8] //안전모드해제
//초음파  
    float duration, distance;    
    digitalWrite(trig, LOW);        
    delayMicroseconds(2);            
    digitalWrite(trig, HIGH);   
    delayMicroseconds(10);            
    digitalWrite(trig, LOW);        
 
    duration = pulseIn(echo, HIGH);  
 
    distance = duration * 0.000170;
    values[4] = distance; //고도
    Serial.print("distance: ");
    Serial.print(distance); 
    Serial.println("m");

//서보 작동해서 투하
    if (values[0]==123){
        servo.write(active_angle); 
        delay(500);
        servo.write(angle);
        delay(500);
   }
 } 
}
