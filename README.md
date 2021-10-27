Run
    git config --global user.email "thdwlgnsdlek@mail.ulsan.ac.kr"
    git config --global user.name "wlgnsthd"

# ARG_2021
<img src="https://user-images.githubusercontent.com/88171531/128798059-5e8daf42-f17a-4a33-8818-220339d0c7a5.png">
<img src="https://user-images.githubusercontent.com/88171531/128798076-b0f0e147-077e-42b8-a984-964f46e72cb6.png">

## RPI Command Code
https://github.com/wlgnsthd/BTS_Camera

## connect camera and configure
# Install TF lite interpreter
~~~
# ! install only TF lite interpreter (version Linux ARM 32 python 3.7)  
pip3 install https://dl.google.com/coral/python/tflite_runtime-2.1.0.post1-cp37-cp37m-linux_armv7l.whl
# pip3 install https://dl.google.com/coral/python/tflite_runtime-2.1.0.post1-cp37-cp37m-linux_aarch64.whl
~~~

# Coral TPU
~~~
echo "deb https://packages.cloud.google.com/apt coral-edgetpu-stable main" | sudo tee /etc/apt/sources.list.d/coral-edgetpu.list
curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
sudo apt-get update
sudo apt-get install python3-tflite-runtime
sudo apt-get install libedgetpu1-max
~~~
#### Put this line into upper code : from tflite_runtime.interpreter import load_delegate
#### Change interpreter = Interpreter(args.model) into interpreter = Interpreter(args.model, experimental_delegates=[load_delegate('libedgetpu.so.1.0')])

# download the example files
~~~
git clone https://github.com/tensorflow/examples --depth 1
cd examples/lite/examples/object_detection/raspberry_pi
# The script takes an argument specifying where you want to save the model files
bash download.sh /home/pi/Desktop/tmp
~~~

# Run the example
~~~
python3 detect_picamera.py \  
--model /home/pi/Desktop/tmp/detect.tflite \ 
# --model /home/pi/Desktop/tmp/mobilenet_ssd_v2_coco_quant_postprocess_edgetpu.tflite \ 
--labels /home/pi/Desktop/tmp/coco_labels.txt
~~~
## verify tflite version 2.5.0 
~~~
python3 -c 'print(__import__("tflite_runtime").__version__)'
python3 -c 'print(__import__("tflite_runtime").__path__)'
rm -r 'PATH'
~~~

# Change detect.py file
![IMG_7770](https://user-images.githubusercontent.com/88171531/135710057-3b845369-41d1-45b8-86c4-ed63efe3dc96.jpeg)
![IMG_8059](https://user-images.githubusercontent.com/88171531/135710062-08e5103f-1b04-41c7-aa3d-c1688abba235.jpg)
![IMG_8060](https://user-images.githubusercontent.com/88171531/135710063-f2c1ef01-7675-4e36-ad9a-0664c153b130.jpeg)
