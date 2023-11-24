#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
// IF ESP8266 IS USED
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
// IF ESP32-C3 IS USED
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Seb";
const char* password = "12345678914";
const char* serverUrl = "https://cloud.sebhost.fr/rfid-app/checkAccess.php";

// IF ESP8266 IS USED
// #define SS_PIN D8
// #define RST_PIN D4
// IF ESP32-C3 IS USED
#define SS_PIN D1
#define RST_PIN D0

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::StatusCode status;

// DEFINE NEW KEYS
MFRC522::MIFARE_Key  newKeyA = {0x4F, 0x2E, 0x7A, 0x91, 0xC8, 0x3F};
// MFRC522::MIFARE_Key  newKeyB = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};


void setup() {
	Serial.begin(115200);

	WiFi.begin(ssid, password);
	Serial.println("Connecting to WiFi...");
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("Connected to WiFi!");

	SPI.begin();
	mfrc522.PCD_Init();
	Serial.println("Scan RFID card to check for authorization...");
}

void loop() {
	String cardId = readCard();

	// Look for new cards
	if (cardId != "") {
		Serial.println("Card detected!");

		// Authenticate with the modified keys
		if (checkKeys() != MFRC522::STATUS_OK) {

			// Effectuer une requête HTTPS au serveur
			WiFiClientSecure client;
			client.setInsecure();  // Ignorer la vérification du certificat
			HTTPClient http;

			String url = String(serverUrl) + "?cardId=" + cardId;

			if (http.begin(client, url)) {
				int	httpCode = http.GET();

				if (httpCode == 200) {
					// Analyser la réponse JSON
					DynamicJsonDocument doc(1024);
					DeserializationError error = deserializeJson(doc, http.getString());

					if (error) {
						Serial.print("JSON parsing failed! Error: ");
						Serial.println(error.c_str());
					} else {
						// Vérifier le statut d'accès
						const char* status_ = doc["status"];

						if (strcmp(status_, "Autorisé") == 0) {
							Serial.println("Access Authorized");
						} else if (strcmp(status_, "Refusé") == 0) {
							Serial.println("Access Denied!");
						} else if (strcmp(status_, "Inconnu") == 0) {
							Serial.println("Card Unknown!");
						} else {
							Serial.println("Unknown status");
						}
					}
				} else {
					Serial.print("HTTP Code: ");
					Serial.println(httpCode);
					Serial.println("Error accessing server");
				}
				http.end();
			}

		} else {
			Serial.println("Authorization failed!");
		}

		delay(1000);
		Serial.println("Scan RFID card to check for authorization...");
	}
}

MFRC522::StatusCode checkKeys() {
	int		block = 4; // DO NOT TOUCH SECTOR 0

	while (block < 64) {
		status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &newKeyA, &(mfrc522.uid));
		if (status != MFRC522::STATUS_OK) {
			Serial.print("PCD_Authenticate() failed: ");
			Serial.println(mfrc522.GetStatusCodeName(status));
			return status;
  		}
		block += 4;
	}
	return status;
}

String readCard() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String cardId = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            cardId += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
            cardId += String(mfrc522.uid.uidByte[i], HEX);
        }
        cardId.toUpperCase();
        return cardId;
    }
    return "";
}
