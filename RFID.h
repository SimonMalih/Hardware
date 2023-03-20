#ifndef RFID_H
#define RFID_H

#include <MFRC522.h>
#include <SPI.h>
#include "Arduino.h"

#define SS_PIN 5    // ESP32 pin GIOP5
#define RST_PIN 27  // ESP32 pin GIOP27

MFRC522 rfid(SS_PIN, RST_PIN);

class RFID {
   public:
    String uid = "";

    // RFID() {
    //     SPI.begin();      // init SPI bus
    //     rfid.PCD_Init();  // init MFRC522
    //     Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader\n");
    // }

    void start() {
        SPI.begin();      // init SPI bus
        rfid.PCD_Init();  // init MFRC522
        Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader\n");
    }

    void read(bool &auth) {
        //printf("in\n");
        if (rfid.PICC_IsNewCardPresent()) {    // new tag is available
            printf("im in\n");
            if (rfid.PICC_ReadCardSerial()) {  // NUID has been readed
                MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
                //Serial.print("RFID/NFC Tag Type: \n");
                Serial.println(rfid.PICC_GetTypeName(piccType));

                // print UID in Serial Monitor in the hex format
                Serial.print("UID:");
                for (int i = 0; i < rfid.uid.size; i++) {
                    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(rfid.uid.uidByte[i], HEX);
                }
                auth = true;
                //previousTime = millis();
                rfid.PICC_HaltA();       // halt PICC
                rfid.PCD_StopCrypto1();  // stop encryption on PCD
                delay(1);
            }
        }
    }
};

#endif
