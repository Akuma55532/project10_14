import matplotlib
matplotlib.use('qt5agg')  # 或者其他的GUI后端
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

# 假设我们要绘制的时间序列长度为 100
n_points = 100
x = np.linspace(0, 10, n_points)  # 时间轴
y = np.zeros(n_points)  # 初始化 y 值为零

# 用于模拟数据获取的函数
def get_new_data():
    return np.random.random()  # 返回随机数据点

# 更新 y 值的函数
def update_data(i, y_data):
    new_value = get_new_data()
    y_data[:-1] = y_data[1:]  # 将现有数据向前移动一位
    y_data[-1] = new_value  # 添加新数据点
    return y_data

fig, ax = plt.subplots()

line, = ax.plot([], [], lw=2)  # 创建一个空的线条

# 初始化函数
def init():
    line.set_data([], [])
    return line

# 动画更新函数
def animate(i):
    global y
    y = update_data(i, y)[0]
    line.set_data(x, y)
    ax.set_xlim(0, 10)
    ax.set_ylim(-1, 2)  # 根据你的数据范围进行调整
    return line

# 创建动画
ani = FuncAnimation(fig, animate, frames=range(n_points), init_func=init, blit=True, interval=200)

plt.show()