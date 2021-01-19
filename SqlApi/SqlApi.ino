/*
 * @Author: lhlc67
 * @Date: 2021-01-10 21:04:44
 * @LastEditTime: 2021-01-19 10:35:38
 * @LastEditors: Please set LastEditors
 * @Description: 数据库插入测试
 * @FilePath: \VScode\SensorDemo\MYSQL\test.ino
 */
#include "SqlUtils.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

/*    获取互联网时间  */
String getIntnetTime();

/*    开关引脚12  */
int pinSwitch = 12;

/*    初始化数据库工具类  */
WiFiClient client;

// 定义一个sql工具类的指针
SqlUtils* Sql = new SqlUtils(client);

void setup()
{
    /*   初始化串口和WIFI   */
    Serial.begin(115200);
    Serial.println("\n\n\n\n");
    WiFi.begin("cyq", "77011000");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("IP Address ->");
    Serial.println(WiFi.localIP());

    /*     设置引脚为输入模式       */
    // INPUT_PULLUP 和 INPUT的区别https://www.arduino.cn/thread-31149-1-9.html
    pinMode(pinSwitch, INPUT);

    /*     配置连接信息       */
    Sql->ConnMessage.server = IPAddress(47, 92, 173, 239);
    Sql->ConnMessage.port = 3306;
    Sql->ConnMessage.user = "lin";
    Sql->ConnMessage.password = "lin456";
    Sql->ConnMessage.database = "supermarket";
    Sql->ConnMessage.tablename = "product";

    /*     数据库进行连接    */
    while (!Sql->MysqlConnect()) {
        Serial.print(".");
    }
    Serial.println("数据库连接成功");
}

void loop()
{
    /* 进行插入操作, IO口默认是高 */
    if (digitalRead(pinSwitch) == LOW) {
        TableMessage data; // SQL语句，语句中的参数
        data.date = getIntnetTime();
        data.device_id = "JH001";
        data.device_name = "压力机";
        data.product_name = "3002138";
        data.product_process = "冲压";

        if (Sql->Insert(&data)) {
            Serial.println("插入成功");
        } else {
            Serial.println("插入失败");
        }
    }
}

// 获取互联网时间
String getIntnetTime()
{
    // HTTPClient库用于HTTP协议通讯。通过ESP8266HTTPClient库，
    // 我们可以使用ESP8266利用互联网或局域网向网络服务器发送HTTP请求，
    // 并且分析网络服务器返回的HTTP响应信息，从而实现物联网应用。

    // TCP/IP
    // ESP8266库中还有一个专门用于TCP通讯的WiFiClient库。由于HTTP协议是建立在TCP协议基础之上的，
    // 我们也可以使用WiFiClient库来实现HTTP通讯。在这一点上，ESP8266HTTPClient库与WiFiClient库在功能上形成了互补。
    WiFiClient client;
    HTTPClient http;
    String payload;
    // Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://quan.suning.com/getSysTime.do")) { // HTTP
        // Serial.print("[HTTP] GET...\n");
        // 获取GET请求的响应码
        int httpCode = http.GET();
        // 如果响应码大于0
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // 如果相应为200(请求正常) 或者 301重定向
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                // 拿到请求的相应信息
                /**
                 * return all payload as String (may need lot of ram or trigger out of memory!)
                 * @return String
                 */
                payload = http.getString();
                // 字符串截取，拿到具体时间
                payload = payload.substring(13, 32);
            }
        } else {
            // 如果HTTPcode < 0那么就是请求失败，打印出失败报错信息
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        // HTTP结束
        http.end();
    } else {
        Serial.printf("[HTTP} Unable to connect\n");
    }
    return payload;
}