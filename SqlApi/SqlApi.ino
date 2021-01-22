/*
 * @Author: lhlc67
 * @Date: 2021-01-10 21:04:44
 * @LastEditTime: 2021-01-22 22:01:40
 * @LastEditors: Please set LastEditors
 * @Description: 数据库插入测试
 * @FilePath: \VScode\SensorDemo\MYSQL\test.ino
 */
#include "SqlUtils.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

/*    开关引脚12  */
//int pinSwitch = 12;

/*    初始化数据库工具类  */
WiFiClient client;

// 定义一个sql工具类的指针
SqlUtils* Sql = new SqlUtils(client);
int count = 0;
int count1 = 0;
char ssid[] = "cyq";
char password[] = "77011000";

void Static_connect(IPAddress staticIP, IPAddress gateway, IPAddress subnet);

void setup()
{
    /*   初始化串口和WIFI   */
    Serial.begin(115200);
    Serial.println("\n\n\n\n");

    IPAddress staticIP(192, 168, 0, 22); // 静态IP
    IPAddress gateway(192, 168, 0, 1); // 网关
    IPAddress subnet(255, 255, 255, 0); // 子网掩码
    Static_connect(staticIP, gateway, subnet);

    Serial.print("IP Address ->");
    Serial.println(WiFi.localIP());

    /*     设置引脚为输入模式       */
    // INPUT_PULLUP 和 INPUT的区别https://www.arduino.cn/thread-31149-1-9.html
    //    pinMode(pinSwitch, INPUT);

    /*     配置连接信息       */
    Sql->ConnMessage.server = IPAddress(192, 168, 0, 112);
    Sql->ConnMessage.port = 3308;
    Sql->ConnMessage.user = "root";
    Sql->ConnMessage.password = "123456";
    Sql->ConnMessage.database = "test";
    Sql->ConnMessage.tablename = "record";

    while (!Sql->MysqlConnect()) {
        count1++;
        Serial.println(count1);
        Serial.println("连接失败");
    }
    Serial.println(count1);
    Serial.println("数据库连接成功");
}

void loop()
{
    while (!Sql->MysqlConnect())
        ;

    // 进行插入操作, IO口默认是高
    //    if (digitalRead(pinSwitch) == LOW) {

    TableMessage data; // SQL语句，语句中的参数
    data.equipment_id = "JH001";
    data.product_id = "81-1221-2222";
    data.work_id = "G002";
    data.count = count;

    if (Sql->Insert(&data)) {
        Serial.println("插入成功");
    } else {
        Serial.println("插入失败");
    }
    delay(1000);
    count++;
}

void Static_connect(IPAddress staticIP, IPAddress gateway, IPAddress subnet)
{
    //用固态IP方式接入本地WIFI，提高接收速度和可靠性
    // IPAddress staticIP(192,168,0,22);       // 静态IP
    // IPAddress gateway(192,168,0,1);         // 网关
    // IPAddress subnet(255,255,255,0);        // 子网掩码
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.config(staticIP, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) { // 阻塞式等待，直到连接成功
        delay(500); // 每500MS检测一次，一直到连接成功
        Serial.print(".");
    }
}
