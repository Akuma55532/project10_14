#!/bin/bash

# 激活Conda环境
# 首先，source conda base的激活脚本
source ~/miniconda3/bin/activate

# 检查激活是否成功
if [ $? -ne 0 ]; then
    echo "Failed to activate Conda base environment."
    exit 1
fi

# 激活名为'car'的Conda环境
conda activate car

# 检查激活是否成功
if [ $? -ne 0 ]; then
    echo "Failed to activate Conda environment 'car'."
    exit 1
fi

# 切换到工作空间目录
cd ~/Desktop/workspace || { echo "Failed to change directory to workspace."; exit 1; }

# 执行Python脚本
python3 main.py


