""" 
连接到EMQX服务器，用来在ControllerLED主题上发布消息来控制灯的亮灭
"""
import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))


def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


client = mqtt.Client()

client.on_connect = on_connect
client.on_message = on_message
client.connect('...', 1883, 600)  # 600为keepalive的时间间隔  第一个参数为服务器的ip地址

# 1. 先连上我的服务器  --
# 2. 获取输入，把输入发送到ControllerLED主题上
print("请您输入")
while True:
    info = input()
    client.publish('ControllerLED', payload=info, qos=0)
