import os
import shutil
import random

# 设置路径
image_dir = 'W:\python\Project10.14\dataset'  # 替换为你的image文件夹路径
label_dir = 'W:\python\Project10.14\label_yolo'  # 替换为你的label文件夹路径
train_image_dir = 'train_image'
val_image_dir = 'val_image'
train_txt_dir = 'train_txt'
val_txt_dir = 'val_txt'

# 创建目标文件夹
os.makedirs(train_image_dir, exist_ok=True)
os.makedirs(val_image_dir, exist_ok=True)
os.makedirs(train_txt_dir, exist_ok=True)
os.makedirs(val_txt_dir, exist_ok=True)

# 获取所有的图片和标签文件
image_files = [f for f in os.listdir(image_dir) if f.endswith('.jpg')]
label_files = [f for f in os.listdir(label_dir) if f.endswith('.txt')]

# 确保每个图像文件都有对应的标签文件
assert all(os.path.splitext(img)[0] + '.txt' in label_files for img in image_files), "每个图像文件必须有对应的标签文件"

# 打乱文件顺序
random.shuffle(image_files)

# 计算分割索引
split_index = int(len(image_files) * 0.83)  # 5:1比例

# 分割为训练和验证集
train_images = image_files[:split_index]
val_images = image_files[split_index:]

# 复制文件到目标文件夹
for img in train_images:
    shutil.copy(os.path.join(image_dir, img), os.path.join(train_image_dir, img))
    shutil.copy(os.path.join(label_dir, os.path.splitext(img)[0] + '.txt'), os.path.join(train_txt_dir, os.path.splitext(img)[0] + '.txt'))

for img in val_images:
    shutil.copy(os.path.join(image_dir, img), os.path.join(val_image_dir, img))
    shutil.copy(os.path.join(label_dir, os.path.splitext(img)[0] + '.txt'), os.path.join(val_txt_dir, os.path.splitext(img)[0] + '.txt'))

print("数据集划分完成！")
