#include "mfrcUtils.h"
MFRC522::MIFARE_Key key;
MFRC522* Mfrc522_init(int SS_PIN, int RST_PIN) {
    MFRC522 *mfcr_ptr = new MFRC522(SS_PIN, RST_PIN);

    SPI.begin();
    mfcr_ptr->PCD_Init();
    Serial.println("MFRC522 TEST START!");

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println("This code scan the MIFARE Classsic NUID.");
    Serial.print("Using the following key:");
    printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();
    return mfcr_ptr;
}


String Mfrc522_detaile(byte *buffer, byte bufferSize) {
    String content = "";
    for (byte i = 0; i < bufferSize; i++) {
        content.concat(String(mfcr_ptr->uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfcr_ptr->uid.uidByte[i], DEC));
    }
    return content;
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


