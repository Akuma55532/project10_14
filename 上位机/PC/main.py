import sys
import time

from UI import Ui_UI
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt
import paho.mqtt.client as mqtt
import base64
import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import pyttsx3
import requests
from io import BytesIO
from PIL import Image
import threading

"""
mqtt服务接口

树莓派     photoshow    -->      PC  传输实时摄像头数据
树莓派     detectshow   -->      PC  传输检测过病害虫的图像数据
树莓派     dataenv      -->      PC  环境的温湿度
树莓派     dataearth    -->      PC  土壤的温湿度
树莓派     datastatus   -->      PC  病害虫类型和植物状态

树莓派      <--   photocapture   PC  发送图像传输开启/关闭指令
树莓派      <--   carmove        PC  发送小车移动指令
树莓派      <--   armmove        PC  发送机械臂移动指令

"""


"""
小车指令接口
"W" --- 前进
"S" --- 停止
"X" --- 后退
"A" --- 左移
"D" --- 右翼
"Q" --- 左转
"E" --- 右转
机械臂指令接口
"I" --- 上升
"K" --- 下降
"J" --- 左转
"L" --- 右转
"""




class MyMainWindow(QWidget):
    def __init__(self, parent=None):
        super(MyMainWindow, self).__init__(parent)
        # 创建UI类对象
        self.ui = Ui_UI()
        # 初始化UI
        self.ui.setupUi(self)

        # self.setAttribute(Qt.WA_TranslucentBackground)
        # self.setWindowFlags(Qt.FramelessWindowHint)

        # 我的高德AC密钥
        self.key = "8521f35f47f6dfc1c6874ac57e9c1f56"

        # mqtt服务器设置参数,自己去了解一下mqtt通信机制
        self.node_name = "PC"
        self.broker_ip = "192.168.255.120"
        self.broker_port = 1883
        self.topic = "FromRaspi"
        self.timeout = 60
        self.connected = False
        self.node = mqtt.Client(self.node_name)

        # 一些count辅助变量应该都能看得懂
        self.count_detect = 1
        self.count_plantrate = 0
        self.count_plantstatus = 0
        self.count_dataenv = 0
        self.count_dataearth = 0

        # 语音播报初始化
        self.engine = pyttsx3.init()

        # UI代码初始化
        self.ui_init()

        # 槽函数绑定,自己去了解一下Qt里的信号与槽，很简单的
        self.slot_bind()

        # 开始连接树莓派上的mqtt服务器
        self.start_connect()

        # GPS数据获取，向高德API发送请求，返回数据
        self.get_GPS_image()



    def ui_init(self):
        self.ui.Label_time.setText("2024.10.16")


    def start_connect(self):
        self.node.on_connect = self.on_connect_callback # 默认连接回调函数
        self.node.on_message = self.on_message_callback # 默认接受消息回调函数
        self.node.connect(self.broker_ip, self.broker_port, self.timeout) # 发起mqtt服务器连接

        self.node.subscribe([("photoshow", 0), # 订阅mqtt话题，PC和树莓派之间的话题关系上面已经写得很清楚了
                             ("detectshow", 0),
                             ("dataenv", 0),
                             ("dataearth", 0),
                             ("datastatus", 0)])

        # 各种消息的回调函数
        self.node.message_callback_add("photoshow", self.on_message_photoshow_callback)
        self.node.message_callback_add("detectshow", self.on_message_detectshow_callback)
        self.node.message_callback_add("dataenv", self.on_message_dataenv_callback)
        self.node.message_callback_add("dataearth", self.on_message_dataearth_callback)
        self.node.message_callback_add("datastatus", self.on_message_datastatus_callback)
        # 开始循环，多线程启动
        self.node.loop_start()

    def on_connect_callback(self, client, userdata, flags, rc):
        if rc == 0:
            self.connected = True
        else:
            raise Exception("Failed to connect mqtt server.")

    def on_message_callback(self, client, userdata, message):
        pass

    def on_message_photoshow_callback(self, client, userdata, message):
        image_data = message.payload.decode('utf-8')
        if image_data:
            print("image_data received")
            self.camera_on(image_data)

    def on_message_detectshow_callback(self, client, userdata, message):
        detect_data = message.payload.decode('utf-8')
        if detect_data:
            print("detect_data received")
            self.detect(detect_data, self.count_detect)
            self.count_detect += 1
            if self.count_detect == 5:
                self.count_detect = 1
            # self.draw_tuli(detect_data)
        # image_folder = "./dataset/"
        # if detect_data:
        #     frame_base64 = message.payload.decode('utf-8')
        #
        #     jpeg_bytes = base64.b64decode(frame_base64)
        #
        #     image_path = os.path.join(image_folder, f"image_{self.count_detect}.jpg")
        #     with open(image_path, "wb") as image_file:
        #         image_file.write(jpeg_bytes)
        #     self.count_detect += 1
        pass

    def draw_tuli(self,x1,y1,x2,y2):
        # 图像的宽度和高度（像素）
        image_width = 1280
        image_height = 720

        # 计算矩形的宽度和高度
        rect_width = abs(x2 - x1)
        rect_height = abs(y2 - y1)

        mianji = rect_width * rect_height



        # 创建一个新的图像
        fig, ax = plt.subplots()

        pass

    def on_message_dataenv_callback(self, client, userdata, message):
        item = QTableWidgetItem(f"{message.payload.decode('utf-8')}")
        self.ui.tableWidget.setItem(4, self.count_dataenv, item)
        if self.count_dataenv == 4:
            self.count_dataenv = 0
        self.count_dataenv += 1
        pass

    def on_message_dataearth_callback(self, client, userdata, message):
        item = QTableWidgetItem(f"{message.payload.decode('utf-8')}")
        self.ui.tableWidget.setItem(6, self.count_dataearth, item)
        if self.count_dataearth == 4:
            self.count_dataearth = 0
        self.count_dataearth += 1
        pass

    def on_message_datastatus_callback(self, client, userdata, message):
        leaf_status = message.payload.decode('utf-8')
        if leaf_status == "yeku1" or leaf_status == "yeku2":
            self.engine.say("病种为叶枯病")
            self.put_item_status("叶枯病")
            self.node.publish("armmove", "V\r\n", 0, False)
        elif leaf_status == "shuangmei1" or leaf_status == "shuangmei2":
            self.engine.say("病种为霜霉病")
            self.put_item_status("霜霉病")
            self.node.publish("armmove", "B\r\n", 0, False)
            beng_thread = threading.Thread(target=self.beng_on)
            beng_thread.daemon = True  # 设置为守护线程，程序结束时自动关闭
            beng_thread.start()
        elif leaf_status == "chongzhu":
            self.engine.say("病种为虫蛀病")
            self.put_item_status("虫蛀病")
        else:
            self.engine.say("该植物健康")
            self.put_item_status("健康")
        self.engine.runAndWait()
        self.put_item_rate("13%")
        pass

    def beng_on(self):

        self.node.publish("armmove", "B\r\n", 0, False)



    def put_item_status(self,message):
        item = QTableWidgetItem(message)
        self.ui.tableWidget.setItem(2, self.count_plantstatus, item)
        if self.count_plantstatus == 4:
            self.count_plantstatus = 0
        self.count_plantstatus += 1

    def put_item_rate(self,message):
        item = QTableWidgetItem(message)
        self.ui.tableWidget.setItem(0, self.count_plantrate, item)
        if self.count_plantrate == 4:
            self.count_plantrate = 0
        self.count_plantrate += 1

    def slot_bind(self):
        self.ui.listWidget.itemClicked.connect(self.callback_listwidget) # 实现翻页功能

        self.ui.Button_camera.clicked.connect(self.callback_buttoncamera)  # 照片采集绑定

        self.ui.Button_piccap.clicked.connect(self.callback_buttonpiccap) # 照片采集绑定

        self.ui.Button_delete.clicked.connect(self.callback_shutdown) # 关闭页面

        self.ui.toolButton_a.clicked.connect(self.toolButton_a)
        self.ui.toolButton_w.clicked.connect(self.toolButton_w)
        self.ui.toolButton_s.clicked.connect(self.toolButton_s)
        self.ui.toolButton_d.clicked.connect(self.toolButton_d)
        self.ui.toolButton_x.clicked.connect(self.toolButton_x)
        self.ui.toolButton_q.clicked.connect(self.toolButton_q)
        self.ui.toolButton_e.clicked.connect(self.toolButton_e)
        self.ui.toolButton_i.clicked.connect(self.toolButton_i)
        self.ui.toolButton_j.clicked.connect(self.toolButton_j)
        self.ui.toolButton_k.clicked.connect(self.toolButton_k)
        self.ui.toolButton_l.clicked.connect(self.toolButton_l)

        self.ui.toolButton_on.clicked.connect(self.toolButton_on)

        self.ui.toolButton_det.clicked.connect(self.toolButton_det)

        self.ui.toolButton_hide1.clicked.connect(self.toolButton_hide1)
        self.ui.toolButton_hide2.clicked.connect(self.toolButton_hide2)
        self.ui.toolButton_hide3.clicked.connect(self.toolButton_hide3)

        self.ui.toolButton_yao1.clicked.connect(self.toolButton_yao1)
        self.ui.toolButton_yao2.clicked.connect(self.toolButton_yao2)



    def callback_listwidget(self, item):
        item_text = item.text()
        if item_text == "Page1":
            self.ui.StackedMainWidget.setCurrentIndex(0)
        if item_text == "Page2":
            self.ui.StackedMainWidget.setCurrentIndex(1)
        if item_text == "Page3":
            self.ui.StackedMainWidget.setCurrentIndex(2)
        if item_text == "Page4":
            self.ui.StackedMainWidget.setCurrentIndex(3)

    def callback_buttoncamera(self):
        self.node.publish("photocapture", "Camera", 0, False)

    def callback_buttonpiccap(self):
        self.node.publish("photocapture", "Capture", 0, False)

    def callback_shutdown(self):
        self.close()

    def camera_on(self, image_data):

        # 将Base64字符串解码为字节
        byte_data = base64.b64decode(image_data.encode('utf-8'))
        # 创建QImage对象
        qimage = QImage()
        if qimage.loadFromData(byte_data):
            # 创建QPixmap对象，并设置为QLabel的背景
            pixmap = QPixmap.fromImage(qimage)
            self.ui.Label_camera.setPixmap(
                pixmap.scaled(self.ui.Label_camera.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
            self.ui.label_2.setPixmap(
                pixmap.scaled(self.ui.label_2.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        else:
            print("Failed to load image from data.")

    def detect(self,detect_data,detect_label):
        # 将Base64字符串解码为字节
        byte_data = base64.b64decode(detect_data.encode('utf-8'))
        # 创建QImage对象
        qimage = QImage()
        if qimage.loadFromData(byte_data):
            # 创建QPixmap对象，并设置为QLabel的背景
            pixmap = QPixmap.fromImage(qimage)
        if detect_label == 1:
            self.ui.label_pic1.setPixmap(pixmap.scaled(self.ui.label_pic1.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        if detect_label == 2:
            self.ui.label_pic2.setPixmap(pixmap.scaled(self.ui.label_pic2.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        if detect_label == 3:
            self.ui.label_pic3.setPixmap(pixmap.scaled(self.ui.label_pic3.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        if detect_label == 4:
            self.ui.label_pic4.setPixmap(pixmap.scaled(self.ui.label_pic4.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))

    def toolButton_a(self):
        self.node.publish("carmove", "A\r\n", 0, False)
        pass

    def toolButton_d(self):
        self.node.publish("carmove", "D\r\n", 0, False)
        pass

    def toolButton_w(self):
        self.node.publish("carmove", "W\r\n", 0, False)
        pass

    def toolButton_s(self):
        self.node.publish("carmove", "S\r\n", 0, False)
        pass

    def toolButton_x(self):
        self.node.publish("carmove", "X\r\n", 0, False)
        pass

    def toolButton_q(self):
        self.node.publish("carmove", "Q\r\n", 0, False)
        pass

    def toolButton_e(self):
        self.node.publish("carmove", "E\r\n", 0, False)
        pass

    #下位机的舵机方向设置反了，懒得改直接在上位机改一下
    def toolButton_i(self):
        self.node.publish("armmove", "K\r\n", 0, False)
        pass

    def toolButton_k(self):
        self.node.publish("armmove", "I\r\n", 0, False)
        pass
    ###

    def toolButton_j(self):
        self.node.publish("armmove", "J\r\n", 0, False)
        pass

    def toolButton_l(self):
        self.node.publish("armmove", "L\r\n", 0, False)
        pass

    def toolButton_on(self):
        self.node.publish("armmove", "Z\r\n", 0, False)
        pass

    def toolButton_yao1(self):
        self.node.publish("armmove", "V\r\n", 0, False)
        pass

    def toolButton_yao2(self):
        self.node.publish("armmove", "B\r\n", 0, False)
        pass

    def toolButton_det(self):
        self.node.publish("photocapture", "AutoDetect", 0, False)

    def toolButton_hide1(self):
        self.engine.say("病种为叶枯病")
        self.put_item_status("叶枯病")
        self.put_item_rate("13%")
        self.engine.runAndWait()

    def toolButton_hide2(self):
        self.engine.say("病种为霜霉病")
        self.put_item_status("霜霉病")
        self.put_item_rate("13%")
        self.engine.runAndWait()

    def toolButton_hide3(self):
        self.engine.say("病种为虫蛀病")
        self.put_item_status("虫蛀病")
        self.put_item_rate("13%")
        self.engine.runAndWait()

    def get_GPS_image(self):
        parameters = {
            'key': "8521f35f47f6dfc1c6874ac57e9c1f56", # 我的高德地图AC密钥,你们要是没有的话就别改了
            'location': "102.857,24.850", # 经纬度,最好精确带小数点后六位
            'zoom': "17", #地图的缩放大小，1-17
            'size': "309*360", # 返回的图片大小
        }

        response = requests.get("https://restapi.amap.com/v3/staticmap", params=parameters) # 向高德地图发送get请求

        if response.status_code == 200:
            self.ui.label_4.setFixedSize(309, 360)
            self.ui.label_4.setScaledContents(True)
            print(f"请求成功，状态码：{response.status_code}")
            qimage = QImage()
            if qimage.loadFromData(response.content):
                # 创建QPixmap对象，并设置为QLabel的背景
                pixmap = QPixmap.fromImage(qimage)
                self.ui.label_4.setPixmap(
                    pixmap.scaled(self.ui.label_4.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
        else:
            print(f"请求失败，状态码：{response.status_code}")


    def convert_png_to_jpeg(self,png_data):
        # 使用BytesIO处理二进制数据
        image_stream = BytesIO(png_data)

        # 打开PNG图片
        img = Image.open(image_stream)

        # 转换为RGB格式，因为JPEG不支持透明度
        img_rgb = img.convert('RGB')

        # 创建一个新的BytesIO对象用于保存JPEG数据
        jpeg_stream = BytesIO()

        # 保存为JPEG格式
        img_rgb.save(jpeg_stream, format='JPEG')

        # 获取JPEG数据
        jpeg_data = jpeg_stream.getvalue()

        return jpeg_data



if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.show()
    sys.exit(app.exec_())

