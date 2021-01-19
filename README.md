
# 代码说明

## connect_wifi : 两种连接WIFI的方式

## request_get : 发送HTTP请求，GET方式拿到互联网时间，POST方式向onenet的应用发送温度信息

## mqtt_led : 连接到onenet的MQTT服务器，控制灯泡

## MQTT_eqmx : 连接到阿里云的EMQX服务器，控制灯泡

### VScode乱码修复问题
> C:\Users\lhlc6\.vscode\extensions\vsciot-vscode.vscode-arduino-0.3.2\out\src\common\utils.js

把这里注释掉
```javascript
        /*
        if (os.platform() === "win32") {
            try {
                const chcp = childProcess.execSync("chcp.com");
                codepage = chcp.toString().split(":").pop().trim();
            }
            catch (error) {
                outputChannel_1.arduinoChannel.warning(`Defaulting to code page 850 because chcp.com failed.\
                \rEnsure your path includes %SystemRoot%\\system32\r${error.message}`);
                codepage = "850";
            }
        }
        */
```