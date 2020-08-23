#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include "espUtils.h"

/*
接线
https://upload-images.jianshu.io/upload_images/2109322-30f1dae3ba0a9147.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp
MFRC522  ---   NodeMCU
*/

#define RST_PIN 5
#define SS_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
String content = "";
// Servo servo;

void setup()
{
    Serial_Init(115200);
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("MFRC522 TEST START!");

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println("This code scan the MIFARE Classsic NUID.");
    Serial.print("Using the following key:");
    printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
    pinMode(16, OUTPUT);


    // // 舵机
    // servo.attach(2);//PWM引脚设置，与GPIO引脚号对应.
    // servo.write(0);
    // delay(1000);
}

void loop()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if(!mfrc522.PICC_IsNewCardPresent()) {
        // Serial.println("can not find a new card");
        return;
    }
    // Verify if the NUID has been readed
    if(!mfrc522.PICC_ReadCardSerial()) {
        Serial.println("have no card can choose");
    }
    Serial.println("---------------------------");

    Serial.print("PICC TYPE: ");
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // 检查兼容性
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println("Only For Mifare Classic");
        digitalWrite(16, HIGH);
        delay(500);
        return;
    }

    // 显示卡片的详细信息
    Serial.println("CARD UID:");
    Serial.print("In HEX: ");
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

    Serial.print("In DEC: ");
    printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

    /*
      09 131 229 110    09131229110
      01 010 91 31      010109131
      123 144 168 27    12314416827
    */
    if (content == "12314416827") {
        Serial.println("Blue Card !");
        digitalWrite(16, LOW);
        // servo.write(180);
        delay(500);
    } else if(content == "010109131" || content == "09131229110") {
        Serial.println("Write Card !");
        digitalWrite(16, HIGH);
        // servo.write(90);
        delay(500);
    } else {
        Serial.println("I Do Not Know This Card !");
        // servo.write(90);
        delay(500);
    }

    Serial.println("---------------------------");



    // 检查兼容性
    MFRC522::StatusCode status;
    // 检查兼容性
    if (status != MFRC522::STATUS_OK) {
        Serial.print("身份验证失败？或者是卡链接失败");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    // 停止 PICC
    mfrc522.PICC_HaltA();
    /*  停止加密PCD */
    mfrc522.PCD_StopCrypto1();
}
/**
   将字节数组转储为串行的十六进制值
*/
void dump_byte_array(byte *buffer, byte bufferSize) {
    content="";
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
        // content="";
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfrc522.uid.uidByte[i], DEC));
    }
}
/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], DEC);
    }
}
