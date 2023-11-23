#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D1
#define RST_PIN D0

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key  defaultKey = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
MFRC522::MIFARE_Key  newKeyA = {0x4F, 0x2E, 0x7A, 0x91, 0xC8, 0x3F};
byte  newKey[16] = {0x4F, 0x2E, 0x7A, 0x91, 0xC8, 0x3F, 0xFF, 0x07, 0x80, 0x69, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};

MFRC522::StatusCode status;


void setup() {
	Serial.begin(115200);
	SPI.begin();
	mfrc522.PCD_Init();
	Serial.println("Scan the RFID card to change the keys...");
}

void loop() {
  // CHECK IF NEW CARD
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");

    // GET UID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    Serial.print(F("UID: "));
    Serial.println(uid);

    // ADD TEXT IN BLOCKS
    byte text[16] = {"We did it Dora"};

    status = addText(text);
    if (status != MFRC522::STATUS_OK)
      return ;
    Serial.println("Text wrote successfully!");

    int block = 1;
    // IN ALL SECTORS
    while (block < 64) {
      status = changeKeys(block);
      if (status != MFRC522::STATUS_OK)
        return ;
      block += 4;
    }
    Serial.println("Keys changed successfully!");

    // Halt PICC (put the card into sleep mode)
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    delay(1000);
    Serial.println("Scan the RFID card to change the keys...");
  }
}

MFRC522::StatusCode changeKeys(int block) {
  int   trailerBlock;

  trailerBlock = (block / 4 * 4) + 3; //determine trailer block for the sector
  
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &defaultKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return status;
  }

  status = mfrc522.MIFARE_Write(trailerBlock, newKey, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Write() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return status;
  }

  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &newKeyA, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return status;
  }
  return status;
}

MFRC522::StatusCode addText(byte text[])
{
  int   i = 0;
  byte  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &defaultKey, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return status;
  }

  if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Write() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return status;
  }

  Serial.println("block written");
  return status;
}
