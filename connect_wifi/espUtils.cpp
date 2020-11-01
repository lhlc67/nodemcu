#include "espUtils.h"
ESP8266WiFiMulti WiFiMulti;             // 用来处理多个WiFI的对象

//设置波特率, 等待串口释放, 打印提示信息
void Serial_Init(unsigned long baud) {
    Serial.begin(baud);
    Serial.println();
    Serial.println();
    Serial.println();
    while (!Serial);      //  等待端口的释放
    Serial.printf("\nWaiting for Serial...\n");
    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

}
void DHCP_connect() {
    // DHCP连接方式 DHCP连接方式 DHCP连接方式 DHCP连接方式

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);           // WIFI1信息
    WiFiMulti.addAP("123321", "1242473075");      // WIFI2信息
    while (WiFiMulti.run() != WL_CONNECTED) {       // 如果WiFi没有连接，一直循环打印点
        delay(500);
        Serial.print(".");
    }
    Print_wifiinfo();
    // DHCP连接方式 DHCP连接方式 DHCP连接方式 DHCP连接方式
}
void Static_connect(IPAddress staticIP,IPAddress gateway,IPAddress subnet) {
    //用固态IP方式接入本地WIFI，提高接收速度和可靠性
    // IPAddress staticIP(192,168,0,22);       // 静态IP
    // IPAddress gateway(192,168,0,1);         // 网关
    // IPAddress subnet(255,255,255,0);        // 子网掩码
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.config(staticIP, gateway, subnet);
    while (WiFi.status() != WL_CONNECTED) {         // 阻塞式等待，直到连接成功
        delay(500);                                 // 每500MS检测一次，一直到连接成功
        Serial.print(".");
    }
    Print_wifiinfo();
}
void Print_wifiinfo() {
    Serial.println("\nConnected to network");
    Serial.println("********* Print connect info *********");
    Serial.print("My IP address is: ");
    Serial.println(WiFi.localIP());     // 打印开发板的IP地址
    Serial.print("My gatewayIP is: ");
    Serial.println(WiFi.gatewayIP());     // 打印网关地址
    Serial.print("My dnsIP is: ");
    Serial.println(WiFi.dnsIP()); // 打印DNS
    Serial.print("My hostname is: ");
    Serial.println(WiFi.hostname());     // 打印开发板主机名称
    Serial.print("My macAddress is: ");
    Serial.println(WiFi.macAddress());     // 打印开发板MAC
    Serial.println("********* Print connect info *********");

}
