""" 
连接到EMQX服务器，作用和NodeMCU一样，都是订阅主题为ControllerLED的消息
"""
import paho.mqtt.client as mqtt


def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))


def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))


client = mqtt.Client(client_id="python_sun.py")
client.on_connect = on_connect
client.on_message = on_message
# client.on_disconnect = on_disconnect
# 600为keepalive的时间间隔 第一个参数为服务器的ip地址
client.connect('...', 1883, 600)

# 1. 先连上的服务器  --
client.subscribe('ControllerLED', qos=0)
client.loop_forever()  # 保持连接


# HTTP  --->  Web开发的    发送一次就没了，功耗比较大
#
# MQTT  --->  物联网开发   长连接、低功耗
# 他们都是基于TCP/IP的应用层协议
