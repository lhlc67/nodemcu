#include "espUtils.h"


char *ssid     = "cyq";           // wifi的ssid
char *password = "77011000";      // wifi密码

void setup()
{
    Serial_Init(115200);

    // 1. DHCP连接方式，随机分配IP, 使用了ESP8266WiFiMulti库，
    // 可以连接输入多个WIFI，自动连接网络状况好的
    DHCP_connect();

    // 2. 用固态IP方式接入本地WIFI，提高接收速度和可靠性
    // IPAddress staticIP(192,168,0,22);       // 静态IP
    // IPAddress gateway(192,168,0,1);         // 网关
    // IPAddress subnet(255,255,255,0);        // 子网掩码
    // Static_connect(staticIP, gateway, subnet);
}
void loop()
{

}
