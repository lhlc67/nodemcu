#include "espUtils.h"
ESP8266WiFiMulti WiFiMulti;             // 用来处理多个WiFI的对象

// Onenet的APIKEY
String APIKEY = "";
//若无DNS，使用Server服务端的IP 183.230.40.33
const char* host = "183.230.40.33";
String url = "http://api.heclouds.com/devices/593148445/datapoints?type=3";
const char OneNetServer[] = "api.heclouds.com";
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
void Static_connect(IPAddress staticIP,IPAddress gateway,IPAddress subnet,IPAddress dns1) {
    //用固态IP方式接入本地WIFI，提高接收速度和可靠性
    // IPAddress staticIP(192,168,0,22);       // 静态IP
    // IPAddress gateway(192,168,0,1);         // 网关
    // IPAddress subnet(255,255,255,0);        // 子网掩码
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    //ESP argument order is: ip, gateway, subnet, dns1
    WiFi.config(staticIP, gateway, subnet,dns1);
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
    Serial.println(WiFi.gatewayIP());     // 打印开发板的IP地址
    Serial.print("My dnsIP is: ");
    Serial.println(WiFi.dnsIP()); // 打印开发板的IP地址
    Serial.print("My hostname is: ");
    Serial.println(WiFi.hostname());     // 打印开发板的IP地址
    Serial.print("My macAddress is: ");
    Serial.println(WiFi.macAddress());     // 打印开发板的IP地址
    Serial.println("********* Print connect info *********");

}

// 获取互联网时间
String getIntnetTime() {
// HTTPClient库用于HTTP协议通讯。通过ESP8266HTTPClient库，
// 我们可以使用ESP8266利用互联网或局域网向网络服务器发送HTTP请求，
// 并且分析网络服务器返回的HTTP响应信息，从而实现物联网应用。

// TCP/IP
// ESP8266库中还有一个专门用于TCP通讯的WiFiClient库。由于HTTP协议是建立在TCP协议基础之上的，
// 我们也可以使用WiFiClient库来实现HTTP通讯。在这一点上，ESP8266HTTPClient库与WiFiClient库在功能上形成了互补。
    WiFiClient client;
    HTTPClient http;


    String payload;
    Serial.print("[HTTP] begin...\n");

    if (http.begin(client, "http://quan.suning.com/getSysTime.do")) {  // HTTP

        Serial.print("[HTTP] GET...\n");
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
                payload=payload.substring(13,32);
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

// 通过HTTPClient类发送POST请求
void HTTPClient_POST(char *tem) {
    WiFiClient client;
    HTTPClient http;

    const uint16_t port = 80;
    if (!client.connect(host, port)) {
        Serial.println("connection failed");
        delay(5000);
        return;
    } else {
        Serial.println("WiFiClient connect ok");
        if(http.begin(client, url)) {
            Serial.print("Remote Server Connect OK!");
            Serial.printf("[HTTP} connect ok\n");
            /*
            正确的POST请求格式
            POST http://api.heclouds.com/devices/593148445/datapoints?type=3 HTTP/1.1
            api-key:cRJZ9JOw579x9g=BOxsAEEdkU0U=
            Host:api.heclouds.com
            Content-Length: 10
            Connection: close

            {"TEM":46}
            */
            http.addHeader("api-key", APIKEY);
            http.addHeader("Host", host);
            http.addHeader("Content-Length", "10");
            http.addHeader("Connection", "close");

            long temprature = random(50, 100);
            String postdata = "{\"" + String("TEM") + "\":" + String(tem)+"}";

            Serial.print("Message that you send-->");
            Serial.println(postdata);

            int httpCode = http.POST(postdata);

            if (httpCode > 0) {
                // 如果状态码正常
                if (httpCode == HTTP_CODE_OK) {
                    Serial.print("HTTP状态码->");
                    Serial.println(httpCode);
                    String payload = http.getString();
                    Serial.print("Message that Server return -->");
                    Serial.println(payload);

                }
            }
            else {
                Serial.println("HTTP code < 0");
                Serial.println(httpCode);
            }
            http.end();
        } else {
            Serial.printf("[HTTP} Unable to connect\n");
        }

    }

}