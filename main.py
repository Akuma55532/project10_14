import sys
from UI import Ui_UI
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt
import paho.mqtt.client as mqtt
import base64
import os

import pyttsx3


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


count_detect = 1
count_plantrate = 0
count_plantstatus = 0
count_dataenv = 0
count_dataearth = 0

class MyMainWindow(QWidget):
    def __init__(self, parent=None):
        super(MyMainWindow, self).__init__(parent)
        self.ui = Ui_UI()
        self.ui.setupUi(self)

        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setWindowFlags(Qt.FramelessWindowHint)

        # mqtt服务器设置参数
        self.node_name = "PC"
        self.broker_ip = "192.168.177.120"
        self.broker_port = 1883
        self.topic = "FromRaspi"
        self.timeout = 60
        self.connected = False
        self.node = mqtt.Client(self.node_name)

        self.ui_init()

        self.slot_bind()

        # self.start_connect()

        self.count_detect = 1
        self.count_plantrate = 0
        self.count_plantstatus = 0
        self.count_dataenv = 0
        self.count_dataearth = 0

        self.engine = pyttsx3.init()


    def ui_init(self):
        self.ui.Label_time.setText("2024.10.16")




    def start_connect(self):
        self.node.on_connect = self.on_connect_callback
        self.node.on_message = self.on_message_callback
        self.node.connect(self.broker_ip, self.broker_port, self.timeout)
        self.node.subscribe([("photoshow", 0),
                             ("detectshow", 0),
                             ("dataenv", 0),
                             ("dataearth", 0),
                             ("datastatus", 0)])
        self.node.message_callback_add("photoshow", self.on_message_photoshow_callback)
        self.node.message_callback_add("detectshow", self.on_message_detectshow_callback)
        self.node.message_callback_add("dataenv", self.on_message_dataenv_callback)
        self.node.message_callback_add("dataearth", self.on_message_dataearth_callback)
        self.node.message_callback_add("datastatus", self.on_message_datastatus_callback)
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
        # image_folder = "./dataset/"
        # if detect_data:
        #     frame_base64 = message.payload.decode('utf-8')
        #
        #     # 将Base64编码的字符串转换回字节
        #     jpeg_bytes = base64.b64decode(frame_base64)
        #
        #     # 写入文件
        #     image_path = os.path.join(image_folder, f"image_{self.count_detect}.jpg")
        #     with open(image_path, "wb") as image_file:
        #         image_file.write(jpeg_bytes)
        #     self.count_detect += 1
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
        if leaf_status == "yeku":
            self.engine.say("病种为叶枯病")
            self.engine.runAndWait()
        elif leaf_status == "shuangmei":
            self.engine.say("病种为霜霉病")
            self.engine.runAndWait()
        elif leaf_status == "chongzhu":
            self.engine.say("病种为虫蛀病")
            self.engine.runAndWait()
        pass

    def 

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
            self.ui.Label_camera.setPixmap(pixmap.scaled(self.ui.Label_camera.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
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

    def toolButton_det(self):
        self.node.publish("photocapture", "AutoDetect", 0, False)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.show()
    sys.exit(app.exec_())

