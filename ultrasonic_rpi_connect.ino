#define trig 8   
#define echo 9
#include <Servo.h> 


Servo servo; 
int servoPin = 10;
int angle = 0; 
int active_angle = 90;

int values[5];//[x각도3,y각도3,속도2,안전모드해제1,고도2]
float realvalues[5];
void setup()
{
   Serial.begin(9600);    
   pinMode(trig, OUTPUT);    
   pinMode(echo, INPUT);   
   servo.attach(servoPin); 
}


void loop()
{  //if (Serial.available() > 0){
//라즈베리파이에서 안전모드해제(1,2)/x축각도(0~90.0)/y축각도(0~90.0)/속도(0~9.0)/ [45.0/45.0/9.0/0]9자리
  //String data = Serial.readStringUntil('\n');
  String data = "145045068";
  long int main_value = data.toInt();
  Serial.println(main_value);

//string data에서 int변수로 바꾸기   
  values[0] = main_value/100000000; //안전모드해제1
  values[1] = main_value/100000-values[0]*1000; //x축각도450
  values[2] = main_value/100-(values[0]*1000000+values[1]*1000); //y축각도450
  values[3] = main_value/1-(values[0]*100000000+values[1]*100000+values[2]*100); //속도68   
  realvalues[0] = values[0];
   realvalues[1] = values[1]*0.1;
    realvalues[2] = values[2]*0.1;
     realvalues[3] = values[3]*0.1;
  Serial.println(realvalues[0]);
  Serial.println(realvalues[1]);
 Serial.println(realvalues[2]);
 Serial.println(realvalues[3]);

 
 
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
    //Serial.println(values[0]);
//서보 작동해서 투하
    if (values[0]==1){
        servo.write(active_angle); 
        delay(500);
        servo.write(angle);
        delay(500);
   }
// } 
}
