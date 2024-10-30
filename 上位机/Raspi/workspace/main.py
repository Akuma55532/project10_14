import paho.mqtt.client as mqtt
import threading
import random
import time
import cv2
import base64
import serial
import binascii
import subprocess
import os
import glob
import shutil
import numpy as np

"""
小车指令接口
"W" --- 前进
"S" --- 后退
"A" --- 左移
"D" --- 右翼
"""


yolov5_path = '/home/car/Desktop/Lite/YOLOv5-Lite'  # 替换为YOLOv5的实际路径



class MqttNodeFromPaspi:
    def __init__(self): # 初始化参数

        #设置串口参数
        self.ser1 = serial.Serial(
            port='/dev/ttyUSBB2',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )
        self.ser2 = serial.Serial(
            port='/dev/ttyUSBB1',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )


        # mqtt服务器ip地址和端口号
        self.broker_ip = "127.0.0.1" 
        self.broker_port = 1883
        # 服务连接等待时长
        self.timeout = 60
        # 连接标志位
        self.connected = False
        # 节点名字
        self.node_name = "Raspi"
        # 节点初始化
        self.client = mqtt.Client(self.node_name)
        # 开始尝试连接
        self.start()

        self.start_timed_thread()

        self.camera_on_flag = 0

        self.detect_on_flag = 0

        self.autodetect_on_flag = 0

        self.action_group = ["J\r\n","J\r\n","J\r\n","L\r\n","L\r\n","L\r\n","L\r\n","L\r\n","L\r\n","J\r\n","J\r\n","J\r\n"]

        self.action_index = 0
        pass

    def start(self):
        # 默认连接回调函数
        self.client.on_connect = self.on_connect_callback
        # 默认消息回调函数
        self.client.on_message = self.on_message_callback
        # 发起连接
        self.client.connect(self.broker_ip,self.broker_port,self.timeout)
        # 发起监听
        self.client.subscribe([("photocapture",0),("carmove",0),("armmove",0)])
        # 设置各个监听话题的回调函数
        self.client.message_callback_add("photocapture", self.on_message_photocapture_callback)
        self.client.message_callback_add("carmove", self.on_message_carmove_callback)
        self.client.message_callback_add("armmove", self.on_message_armmove_callback)
        self.client.loop_start()
        

    def on_connect_callback(self, client, userdata, flags, rc):
        # 如果rc == 0就意味着连接成功
        if rc==0:
            self.connected = True
        else:
            raise Exception("False to connect mqtt server")

    def on_message_callback(self, client, userdata, message):
        pass

    def on_message_photocapture_callback(self, client, userdata, message):
        if message.payload.decode('utf-8') == "Camera":
            if self.camera_on_flag == 0:
                self.camera_on_flag = 1
                print("Camera")
                camera_thread = threading.Thread(target=node.camera_on)
                camera_thread.daemon = True  # 设置为守护线程，程序结束时自动关闭
                camera_thread.start()

            elif self.camera_on_flag == 1:
                self.camera_on_flag = 0

        if message.payload.decode('utf-8') == "Capture":
            if self.detect_on_flag == 0:
                self.detect_on_flag = 1
                print("Capture")
                camera_thread = threading.Thread(target=node.capture_pic)
                camera_thread.daemon = True  # 设置为守护线程，程序结束时自动关闭
                camera_thread.start()

        if  message.payload.decode("utf-8") == "AutoDetect":
            if self.autodetect_on_flag == 0:
                self.autodetect_on_flag = 1
                print("AutoDetect")
                camera_thread = threading.Thread(target=node.autodetect)
                camera_thread.daemon = True  # 设置为守护线程，程序结束时自动关闭
                camera_thread.start()

    def on_message_carmove_callback(self, client, userdata, message):
        self.move(message.payload)
        pass

    def on_message_armmove_callback(self, client, userdata, message):
        self.move(message.payload)
        pass

    def move(self, cmd):
        self.ser1.write(cmd)
        pass
        
    def publish_text(self, topic, payload,qos = 0, retain = False):
        if self.connected:
            return self.client.publish(topic,payload=payload,qos=qos,retain=retain)
        else:
            raise Exception("mqtt server not connected! you may use .start() function to connect to server firstly")
        pass

    def publish_pic(self, topic, payload,qos = 0, retain = False):
        if self.connected:
            return self.client.publish(topic, payload=payload, qos=qos, retain=retain)
        else:
            raise Exception("mqtt server not connected! you may use .start() function to connect to server firstly")

    # 树莓派向PC发送实时视频流
    def camera_on(self):
        # 打开摄像头
        cap = cv2.VideoCapture(0)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
        # 如果camera_on_flag == 1的话就一直循环，知道下一次接受到Camera话题之后就关闭线程
        while self.camera_on_flag == 1:
            ret,frame = cap.read()
            if not ret:
                print("Failed to capture frame")
                continue
            # 调整图像质量，不然帧率太低，延迟太高
            params = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
            ret,jpeg = cv2.imencode('.jpg',frame, params)
            if not ret:
                print("Failed to encode frame")
                continue
            # 将图像数据编码为base64格式,虽然增大了体积，但是有利于包在网络中的传输
            frame_base64 = base64.b64encode(jpeg).decode('utf-8')
            self.client.publish("photoshow", payload=frame_base64, qos=0, retain=False)

            
    # 树莓派向PC发送经过yolo检测之后的瞬时图片帧
    def capture_pic(self):
        if self.detect_on_flag == 1:
            if self.camera_on_flag == 1:
                self.camera_on_flag = 0
                time.sleep(1)
            cap = cv2.VideoCapture(0)
            cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
            cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
            ret,frame = cap.read()
            if not ret:
                print("Failed to capture frame")
            frame = self.detect_best(frame)
            params = [int(cv2.IMWRITE_JPEG_QUALITY), 50]  # 调整质量参数
            ret,jpeg = cv2.imencode('.jpg',frame, params)
            if not ret:
                print("Failed to encode frame")
            frame_base64 = base64.b64encode(jpeg).decode('utf-8')
            self.client.publish("detectshow", payload=frame_base64, qos=0, retain=False)
            print("send---------------------------------------------------------------------------------")
            
            # 打开并读取文件
            with open('detection_results.txt', 'r') as file:
                lines = file.readlines()  # 读取所有行

            # 初始化空列表，用于存储类别和置信度
            categories = []
            confidences = []

            # 遍历每行文本，提取类别和置信度
            for line in lines:
                if '类别' in line:  # 查找包含"类别"的行
                    # 提取类别和置信度
                    parts = line.split(',')
                    category = parts[0].split(':')[-1].strip()  # 获取类别
                    confidence = parts[1].split(':')[-1].strip()  # 获取置信度

                    # 保存到列表
                    categories.append(category)
                    confidences.append(float(confidence))

            # 打印结果
            for i in range(len(categories)):
                print(f'类别: {categories[i]}, 置信度: {confidences[i]}')

            self.client.publish("datastatus",payload=categories[i],qos=0,retain=False)

            self.detect_on_flag = 0



    def detect_vLite_s(self,frame):
        # 保存帧到指定路径
        save_path = '/home/car/Desktop/workspace/raw_image/frame.jpg' 
        cv2.imwrite(save_path, frame)  # 保存帧为JPEG格式
        output_dir = '/home/car/Desktop/workspace/'

        output_image_name = 'detected_image.jpg'  # 替换为你想要的输出图像名称

       # 调用YOLOv5检测命令
        command = [
            'python',
            os.path.join(yolov5_path, 'detect.py'),
            '--source', save_path,
            '--weights', os.path.join(yolov5_path, 'weights', 'v5lite-s.pt'),  # 可以选择其他模型
        ]

        subprocess.run(command)

        output_image_path = glob.glob(os.path.join(output_dir, 'detected_images', '*.jpg'))[-1]
        new_image_path = os.path.join(output_dir, output_image_name)

        # 重命名文件
        os.rename(output_image_path, new_image_path)

        print(f"检测结果已保存为: {new_image_path}")

        image = cv2.imread(new_image_path)

        # 删除输出文件夹
        if os.path.exists(os.path.join(output_dir, 'detected_images')):
            shutil.rmtree(os.path.join(output_dir, 'detected_images'))
            print("删除了输出文件夹 detected_images。")

        return image





    def detect_best(self,frame):
        # 保存帧到指定路径
        save_path = '/home/car/Desktop/workspace/raw_image/frame.jpg' 
        cv2.imwrite(save_path, frame)  # 保存帧为JPEG格式
        output_dir = '/home/car/Desktop/workspace/'

        output_image_name = 'detected_image.jpg'  # 替换为你想要的输出图像名称

       # 调用YOLOv5检测命令
        command = [
            'python',
            os.path.join(yolov5_path, 'detect.py'),
            '--source', save_path,
            '--weights', os.path.join(yolov5_path, 'weights', 'best.pt'),  # 可以选择其他模型
        ]

        subprocess.run(command)

        output_image_path = glob.glob(os.path.join(output_dir, 'detected_images', '*.jpg'))[-1]
        new_image_path = os.path.join(output_dir, output_image_name)

        # 重命名文件
        os.rename(output_image_path, new_image_path)

        print(f"检测结果已保存为: {new_image_path}")

        image = cv2.imread(new_image_path)

        # 删除输出文件夹
        if os.path.exists(os.path.join(output_dir, 'detected_images')):
            shutil.rmtree(os.path.join(output_dir, 'detected_images'))
            print("删除了输出文件夹 detected_images。")

        return image

    def detect_last(self,frame):
        # 保存帧到指定路径
        save_path = '/home/car/Desktop/workspace/raw_image/frame.jpg' 
        cv2.imwrite(save_path, frame)  # 保存帧为JPEG格式
        output_dir = '/home/car/Desktop/workspace/'

        output_image_name = 'detected_image.jpg'  # 替换为你想要的输出图像名称

       # 调用YOLOv5检测命令
        command = [
            'python',
            os.path.join(yolov5_path, 'detect.py'),
            '--source', save_path,
            '--weights', os.path.join(yolov5_path, 'weights', 'last.pt'),  # 可以选择其他模型
        ]

        subprocess.run(command)

        output_image_path = glob.glob(os.path.join(output_dir, 'detected_images', '*.jpg'))[-1]
        new_image_path = os.path.join(output_dir, output_image_name)

        # 重命名文件
        os.rename(output_image_path, new_image_path)

        print(f"检测结果已保存为: {new_image_path}")

        image = cv2.imread(new_image_path)

        # 删除输出文件夹
        if os.path.exists(os.path.join(output_dir, 'detected_images')):
            shutil.rmtree(os.path.join(output_dir, 'detected_images'))
            print("删除了输出文件夹 detected_images。")

        return image

    def autodetect(self):
        while self.autodetected == True:
            self.move(self.action_group[action_index])
            action_index += 1;
            self.detect_plant()

            if action_index == 9:
                break
        pass

    def detect_plant(self):
        self.camera_on_flag = 0
        cap = cv2.VideoCapture(0)
        # 检查摄像头是否成功打开
        if not cap.isOpened():
            print("无法打开摄像头")
            exit()
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
        ret,frame = cap.read()

        # 转换颜色空间到HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # 定义绿色的HSV范围
        lower_green = np.array([35, 43, 46])
        upper_green = np.array([77, 255, 255])
        
        # 创建绿色掩码
        mask = cv2.inRange(hsv, lower_green, upper_green)
        
        # 使用形态学操作去除噪点
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5,5))
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        
        # 将掩码应用到原始图像
        res = cv2.bitwise_and(frame, frame, mask=mask)
        
        # 显示结果
        


    def read_data(self):
        # 发送指定数据
        message = "DA\r\n"
        self.ser2.write(message.encode())
        # 接收数据
        raw_data = self.ser2.read(12)  # 假设STM32返回的是4个字节的数据
        print("Received:", raw_data)

        temp_high_byte = raw_data[0]  # 温度的高8位
        temp_low_byte = raw_data[1]   # 温度的低8位
        temp = (temp_high_byte << 8 | temp_low_byte) / 1000.0  # 合并高低字节并转换为浮点数

        humi_high_byte = raw_data[2]  # 温度的高8位
        humi_low_byte = raw_data[3]   # 温度的低8位
        humi = (humi_high_byte << 8 | humi_low_byte) / 1000.0  # 合并高低字节并转换为浮点数

        tu_temp_high_byte = raw_data[4]  # 温度的高8位
        tu_temp_low_byte = raw_data[5]   # 温度的低8位
        tu_temp = (tu_temp_high_byte << 8 | tu_temp_low_byte) / 1000.0  # 合并高低字节并转换为浮点数

        tu_humi_high_byte = raw_data[6]  # 温度的高8位
        tu_humi_low_byte = raw_data[7]   # 温度的低8位
        tu_humi = (tu_humi_high_byte << 8 | tu_humi_low_byte) / 4

        print("Temperature:", temp)
        print("Humidity:", humi)
        print("tuTemperature:", tu_temp)
        print("tuHumidity:", tu_humi)

        dataenv_buffer = "temp:".encode("utf-8") + str(temp).encode('utf-8')+ " wet:".encode("utf-8") + str(humi).encode('utf-8')

        dataearth_buffer = "temp:".encode("utf-8") + str(tu_temp).encode('utf-8')+ " wet:".encode("utf-8") + str(tu_humi).encode('utf-8')

        self.client.publish("dataenv",payload=dataenv_buffer,qos=0,retain=False)
        self.client.publish("dataearth",payload=dataearth_buffer,qos=0,retain=False)


    # 定时器函数，隔一段时间触发一次
    def timed_task1(self,interval):

        self.read_data()

        time.sleep(interval)

        # 定时器总中断

    def start_timed_thread(self):
        def run():
            while True:
                # 执行定时任务
                self.timed_task1(1)

        # 创建一个线程来执行定时任务
        thread = threading.Thread(target=run)
        thread.daemon = True  # 设置为守护线程，程序结束时自动关闭
        thread.start()

        return thread

if __name__ == "__main__":
    node = MqttNodeFromPaspi()
    while not node.connected:
        pass
    while True:
        time.sleep(1)
