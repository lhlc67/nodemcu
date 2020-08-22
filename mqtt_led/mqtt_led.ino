/**
* 日期: 2020年8月22日 15:25:35
* 功能：MQTT连接到ＯＮＥＮＥＴ
* 作者：花猫
**/
#include "espUtils.h"
#include <PubSubClient.h>

char *ssid     = "cyq";           // wifi的ssid
char *password = "77011000";      // wifi密码

IPAddress server(183,230,40,39);        // OneNet的服务器地址
WiFiClient client;               // 声明一个WiFiClient对象，用来初始化 MQTT对象
void callback(char *topic, byte *payload, unsigned int length);
PubSubClient MQTTClient(server, 6002, callback, client);                // 声明一个MQTT对象
/* OneNet自己定义设备的信息 */
#define PRODUCT_ID "365396"
#define API_KEY "=YwVMxLyXSjeGSB1GKntDmZVQR0="
#define DEVICE_ID "616758553"
#define TOPIC "ControllerLED"

void setup()
{
    Serial_Init(115200);
    // 1. DHCP连接方式，随机分配IP, 使用了ESP8266WiFiMulti库，
    // 可以连接输入多个WIFI，自动连接网络状况好的
    DHCP_connect();
    // 将NodeMCU的LED灯设置成output状态, 因为该百年其电压来点亮与熄灭
    pinMode(16, OUTPUT);
    delay(1500);

}
void loop()
{
    if(!MQTTClient.connected()) {
        reconnect(PRODUCT_ID, API_KEY, DEVICE_ID);
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
void reconnect(char *onenet_productId, char *onenet_apiKey, char *onenet_deviceId) {
    // Loop until we're reconnected
    while (!MQTTClient.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (MQTTClient.connect(onenet_deviceId,onenet_productId,onenet_apiKey)) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            MQTTClient.publish("outTopic","hello world");
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