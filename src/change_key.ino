#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D1 // D8
#define RST_PIN D0 // D4

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
	Serial.begin(115200);
	SPI.begin();
	mfrc522.PCD_Init();
	Serial.println(F("Scan the RFID card to change the keys..."));
}

void loop() {
  // CHECK IF NEW CARD
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("Card detected!"));

    // GET UID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    Serial.print(F("UID: "));
    Serial.println(F(uid));

    int cur = 1;

    while (cur < 6) {
      // CHANGE KEY A
      byte currentKeyA[MFRC522::MF_KEY_SIZE];
      mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, cur, currentKeyA, &(mfrc522.uid));
      byte newKeyA[MFRC522::MF_KEY_SIZE] = {0x4F, 0x2E, 0x7A, 0x91, 0xC8, 0x3F};
      changeKey(currentKeyA, newKeyA, MFRC522::PICC_CMD_MF_AUTH_KEY_A, cur);
      cur += 2;
    }
    Serial.println(F("Keys A changed successfully!"));

    cur = 1;
    while (cur < 6) {
      // CHANGE KEY B
      byte currentKeyB[MFRC522::MF_KEY_SIZE];
      mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, cur, currentKeyB, &(mfrc522.uid));
      byte newKeyB[MFRC522::MF_KEY_SIZE] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};
      changeKey(currentKeyB, newKeyB, MFRC522::PICC_CMD_MF_AUTH_KEY_B, cur);
      cur += 2;
    }
    Serial.println(F("Keys B changed successfully!"));

    // Halt PICC (put the card into sleep mode | signal that the interaction is complete)
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    delay(1000);
    Serial.println(F("Scan the RFID card to change the keys..."));
  }
}

void changeKey(byte currentKey[MFRC522::MF_KEY_SIZE], byte newKey[MFRC522::MF_KEY_SIZE], byte authKeyType, int cur) {
  for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
    mfrc522.PCD_WriteRegister(MFRC522::SectorTrailBlock, i, newKey[i]);
    currentKey[i] = newKey[i];
	}
  mfrc522.PCD_Authenticate(authKeyType, cur, currentKey, &(mfrc522.uid));
}
