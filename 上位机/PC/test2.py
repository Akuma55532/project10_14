import requests

def save_GPS_image():
    parameters = {
        'key': "8521f35f47f6dfc1c6874ac57e9c1f56",
        'location': "102.857,24.850",  # 注意这里的键名是 'location' 而不是 'loaction'
        'zoom': "15",
        'size': "309*360",
    }
    response = requests.get("https://restapi.amap.com/v3/staticmap", params=parameters)

    if response.status_code == 200:
        # 如果状态码为 200，则表示请求成功
        with open('gps_image.jpg', 'wb') as f:
            f.write(response.content)  # 写入图片数据
        print("图片已保存")
    else:
        print(f"请求失败，状态码：{response.status_code}")

# 调用函数
save_GPS_image()