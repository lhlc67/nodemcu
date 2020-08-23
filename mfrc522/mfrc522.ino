#include "espUtils.h"
#include "mfrcUtils.h"

/*
接线
https://upload-images.jianshu.io/upload_images/2109322-30f1dae3ba0a9147.png?imageMogr2/auto-orient/strip|imageView2/2/w/1200/format/webp
MFRC522  ---   NodeMCU
*/

const int SS_PIN = 4;
const int RST_PIN = 5;
String content = "";
// Servo servo;
MFRC522 *mfcr_ptr;
void setup()
{
    Serial_Init(115200);

    // SPI.begin();
    // mfcr_ptr->PCD_Init();
    // Serial.println("MFRC522 TEST START!");

    // for (byte i = 0; i < 6; i++) {
    //     key.keyByte[i] = 0xFF;
    // }

    // Serial.println("This code scan the MIFARE Classsic NUID.");
    // Serial.print("Using the following key:");
    // printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    // Serial.println();
    mfcr_ptr = Mfrc522_init(SS_PIN, RST_PIN);

    pinMode(16, OUTPUT);


    // // 舵机
    // servo.attach(2);//PWM引脚设置，与GPIO引脚号对应.
    // servo.write(0);
    // delay(1000);
}

void loop()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if(!mfcr_ptr->PICC_IsNewCardPresent()) {
        // Serial.println("can not find a new card");
        return;
    }
    // Verify if the NUID has been readed
    if(!mfcr_ptr->PICC_ReadCardSerial()) {
        Serial.println("have no card can choose");
    }
    Serial.println("---------------------------");

    Serial.print("PICC TYPE: ");
    MFRC522::PICC_Type piccType = mfcr_ptr->PICC_GetType(mfcr_ptr->uid.sak);
    Serial.println(mfcr_ptr->PICC_GetTypeName(piccType));

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
    printHex(mfcr_ptr->uid.uidByte, mfcr_ptr->uid.size);
    Serial.println();

    Serial.print("In DEC: ");
    printDec(mfcr_ptr->uid.uidByte, mfcr_ptr->uid.size);
    Serial.println();

    content = Mfrc522_detaile(mfcr_ptr->uid.uidByte, mfcr_ptr->uid.size);
    Serial.println(content);
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
    }
    else {
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
        Serial.println(mfcr_ptr->GetStatusCodeName(status));
        return;
    }
    // 停止 PICC
    mfcr_ptr->PICC_HaltA();
    /*  停止加密PCD */
    mfcr_ptr->PCD_StopCrypto1();
}

