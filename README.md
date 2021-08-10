# ARG_2021
<img src="https://user-images.githubusercontent.com/88171531/128798059-5e8daf42-f17a-4a33-8818-220339d0c7a5.png">
<img src="https://user-images.githubusercontent.com/88171531/128798076-b0f0e147-077e-42b8-a984-964f46e72cb6.png">
'''
## 파이카메라 세팅
sudo raspi-config

raspistill -v -o test.jpg

##라즈베리파이 업데이트
sudo apt update
sudo apt full-upgrade
sudo apt-get update
sudo apt-get dist-upgrade

##서보 소프트웨어 간섭 해결 코드
sudo pigpiod
sudo pip install pandas
sudo pip install gpiozero
##Tensorflow Lite script 다운로드
git clone https://github.com/EdjeElectronics/TensorFlow-Lite-Object-Detection-on-Android-and-Raspberry-Pi.git

#Tensorflow Lite 가상환경(폴더) 설정
mv TensorFlow-Lite-Object-Detection-on-Android-and-Raspberry-Pi tflite1
cd tflite1
sudo pip3 install virtualenv
python3 -m venv tflite1-env
source tflite1-env/bin/activate
ls


##Tensorflow Lite, Opencv 설치
bash get_pi_requirements.sh
##!!opencv설치안되면 pip install opencv-python 코드 바꾸기

##Detection Model 설정
##옵션1)주어진 샘플모델 다운로드
wget https://storage.googleapis.com/download.tensorflow.org/models/tflite/coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.zip
unzip coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.zip -d Sample_TFLite_model

##옵션2)Custom-trained model 사용, 모델폴더(detect.tflite와 labelmap.txt파일)를 /home/pi/tflite디렉토리에 옮기기

#Tensorflow Lite 모델 실행(웹캠이나 파이카메라 실행)
python3 TFLite_detection_webcam.py --modeldir=Sample_TFLite_model

#STEP2 (구매후)________________________________________________________________________________________________
#Edge TPU실행- 라이브러리 설치
##cd /home/pi/tflite1
##source tflite1-env/bin/activate

##echo "deb https://packages.cloud.google.com/apt coral-edgetpu-stable main" | sudo tee /etc/apt/sources.list.d/coral-edgetpu.list
##curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
##sudo apt-get update

##sudo apt-get install libedgetpu1-std

##사용자 정의 edge tpu model사용 (TFlite model->Edge TPU model) https://coral.ai/docs/edgetpu/compiler/#help

#Edge TPU 감지 실행
##python3 TFLite_detection_webcam.py --modeldir=Sample_TFLite_model --edgetpu
##python3 TFLite_detection_video.py --modeldir=Sample_TFLite_model --edgetpu
##
##python3 TFLite_detection_image.py --modeldir=Sample_TFLite_model --edgetpu

#STEP3 대상 좌표 및 크기 데이터 뽑아내기 https://www.tensorflow.org/lite/inference_with_metadata/task_library/object_detector
'''
