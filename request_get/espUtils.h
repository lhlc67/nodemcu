#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>


extern char *ssid;     // WIFI名
extern char *password; //WIFI密码

extern ESP8266WiFiMulti WiFiMulti;
extern WiFiClient client;


void Serial_Init(unsigned long baud);   //设置波特率, 等待串口释放, 打印提示信息
void DHCP_connect();
void Static_connect(IPAddress staticIP, IPAddress gateway, IPAddress subnet,IPAddress dns1);
void Print_wifiinfo();                  // 打印连接WIFI信息

String getIntnetTime();
void HTTPClient_POST(char *tem);

#endif
