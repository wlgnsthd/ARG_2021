## find arudino
~~~
ls /dev/tty* 
## authorize
sudo adduser your_username dialout
sudo apt install python3-pip
python3 -m pip install pyserial
~~~

# python code sending message from rpi
~~~
import serial
import time
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    while True:
        ser.write(b"Hello from Raspberry Pi!\n")
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
        time.sleep(1)
~~~
# python code sending message from arduino
~~~
import serial
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
~~~
