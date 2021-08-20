import wiringpi2 as gpio
HIGH = 1
LOW = 0
OUTPUT = 1
INPUT = 0
TRIG = 4
ECHO = 5

gpio.wiringPiSetup()
gpio.pinMode(TRIG,OUTPUT)
gpio.pinMode(ECHO,INPUT)

while True:
    #Ultrasonic sensor_H
    gpio.digitalWrite(TRIG,LOW)
    gpio.delayMicroseconds(2)

    gpio.digitalWrite(TRIG,HIGH)
    gpio.delayMicroseconds(10)
    gpio.digitalWrite(TRIG,LOW)
    
    while gpio.digitalRead(ECHO) == LOW:
	      startTime = gpio.micros()
    while gpio.digitalRead(ECHO) == HIGH:
	      endTime = gpio.micros()
#unit=cm	
    distance = (endTime - startTime) / 58.0
    
    if distance < 400:
        print('Distance:', round(distance,2), 'cm')
