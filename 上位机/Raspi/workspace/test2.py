import subprocess
import os

# 设置YOLOv5的路径
yolov5_path = '/home/car/Desktop/workspace'  # 替换为YOLOv5的实际路径


# 调用YOLOv5检测命令
command = [
    'python',
    os.path.join(yolov5_path, 'test.py'),
]

# 执行命令
subprocess.run(command)

# 检测结果保存在 yolov5/runs/detect/exp/labels 文件夹中
