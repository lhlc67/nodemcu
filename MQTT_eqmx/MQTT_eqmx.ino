/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

[Comment Translate] Google
 MQTT基本示例

 该草图演示了库的基本功能。
 然后连接到OneNet的MQTT服务器：
  -将“ hello world”发布到主题“ outTopic”
  -订阅主题“ inTopic”，打印出所有消息
    它收到。 NB-假设接收到的有效载荷是字符串而不是二进制

 如果使用阻塞断开连接，它将重新连接到服务器
 重新连接功能。请参阅“ mqtt_reconnect_nonblocking”示例以了解如何
 在不阻塞主循环的情况下达到相同的结果。

*/

/* ***************** ***************** *****************
代码使用库自带的例程改的，连接到了阿里云上搭建的EMQX服务器
注意的地方 :
    1.PubSubClient有很多种构造函数, 每种构造函数的说可以参考源码,也可以看菜鸟哥的博客: https://blog.csdn.net/dpjcn1990/article/details/92831686
    无论哪种构造函数, 都必须让实例化出来的对象具有
            - server,port(也就是你要连接到哪个服务器的哪个端口)
            - callback()函数, 这是处理服务器返回信息的函数
            - Client& Client实例, 因为MQTT的PubSubClient如果运行需要用到网络
    2.PubSubClient的参数设置可以在构造函数中设置, 也可以通过 70-72 行的方式来设置
****************** ***************** ******************** */
#include <Arduino.h>
#include <PubSubClient.h>
#include "espUtils.h"
char *ssid     = "cyq";           // wifi的ssid
char *password = "77011000";      // wifi密码

// Update these with values suitable for your network.
IPAddress server(,,,);        // 搭建MQTT的的服务器地址
// PubSubClient MQTTClient;                // 声明一个MQTT对象
WiFiClient wifiClient;               // 声明一个WiFiClient对象，用来初始化 MQTT对象

void callback(char *topic, byte *payload, unsigned int length);

// 按住Ctrl 然后鼠标点击PubSubClient, 可以看到PubSubClient类的14个构造函数, 这里我们选择这一种
// PubSubClient(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
PubSubClient MQTTClient(server, 1883, callback, wifiClient);                // 声明一个MQTT对象

#define TOPIC "ControllerLED"

void setup()
{

    Serial_Init(115200);                          // 设置波特率, 等待串口释放, 打印提示信息
    DHCP_connect();

    // MQTTClient.setServer(server, 6002);        // 设置服务器信息
    // MQTTClient.setClient(wifiClient);          // 设置连接的Client实例
    // MQTTClient.setCallback(callback);          // 设置MQTT对象的回掉函数

    pinMode(16, OUTPUT);                          // 将NodeMCU的LED灯设置成output状态, 因为该百年其电压来点亮与熄灭
    delay(1500);
}

void loop()
{
    if (!MQTTClient.connected()) {
        reconnect();
    }
    // MQTTClient.loop();处理消息以及保持心跳
    MQTTClient.loop();
}

// 回调函数，用来打胤出信息的主题、返回信息和长度
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0; i<length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();

    if ((char)payload[0] == '1') {
        digitalWrite(16, LOW);
    } else {
        digitalWrite(16, HIGH);
    }
}

// 重连接
void reconnect() {
    // Loop until we're reconnected
    while (!MQTTClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (MQTTClient.connect("arduinoClient")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            // MQTTClient.publish("ControllerLED","hello world");
            /**
            * 订阅主题(客户端向服务端发送SUBSCRIBE报文用于创建一个或多个订阅)
            * @param topic 主题
            * @param qos 质量等级
            * @return bool 是否订阅成功
            */
            MQTTClient.subscribe(TOPIC, 1);
        } else {
            Serial.print("failed, rc=");
            Serial.print(MQTTClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

