#ifndef MFRCUTILS_H
#define MFRCUTILS_H
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
extern MFRC522 *mfcr_ptr;
extern MFRC522::MIFARE_Key key;

MFRC522* Mfrc522_init(int SS_PIN, int RST_PIN);
String Mfrc522_detaile(byte *buffer, byte bufferSize);
void dump_byte_array(byte *buffer, byte bufferSize);
void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);
#endif