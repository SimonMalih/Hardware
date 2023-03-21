#ifndef RFID_H
#define RFID_H

#include <MFRC522.h>
#include <SPI.h>
#include "Arduino.h"

#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_SS 15
#define HSPI_RST 27

MFRC522 rfid(HSPI_SS, HSPI_RST);

class RFID {
   public:
    String uid = "";

    RFID() {
    }

    void start() {
        SPI.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);  // init SPI bus
        rfid.PCD_Init();                                      // init MFRC522
        Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader\n");
    }

    void read(bool &auth, unsigned long &previousTime) {
        if (rfid.PICC_IsNewCardPresent()) {    // new tag is available
            if (rfid.PICC_ReadCardSerial()) {  // NUID has been readed
                MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
                //Serial.print("RFID/NFC Tag Type: \n");
                //Serial.println(rfid.PICC_GetTypeName(piccType));

                // print UID in Serial Monitor in the hex format
                Serial.print("UID:");
                for (int i = 0; i < rfid.uid.size; i++) {
                    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(rfid.uid.uidByte[i], HEX);
                }
                auth = true;
                previousTime = millis();
                Serial.println();
                // previousTime = millis();
                rfid.PICC_HaltA();       // halt PICC
                rfid.PCD_StopCrypto1();  // stop encryption on PCD
                delay(1000);
            }
        }
    }
};

#endif
