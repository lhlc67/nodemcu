/**
* 日期: 2020年8月22日 15:25:35
* 功能：发送GET请求(获取网络时间)和POST请求示例(ONENET)
* 作者：花猫
**/

#include "espUtils.h"

char *ssid     = "cyq";           // wifi的ssid
char *password = "77011000";      // wifi密码

void setup()
{
    Serial_Init(115200);
    // 1. DHCP连接方式，随机分配IP, 使用了ESP8266WiFiMulti库，
    // 可以连接输入多个WIFI，自动连接网络状况好的
    DHCP_connect();

}
void loop()
{
    String time = getIntnetTime();      // 获取网络时间
    Serial.println(time);
    HTTPClient_POST("15");
    delay(2000);
}


