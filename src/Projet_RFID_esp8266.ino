#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "Seb";
const char* password = "12345678914";
const char* serverUrl = "https://cloud.sebhost.fr/rfid-app/checkAccess.php";  // URL du serveur pour la vérification d'accès via RFID

#define SS_PIN 15  // D8
#define RST_PIN 2   // D4
MFRC522 mfrc522(SS_PIN, RST_PIN);

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
}

void loop() {
    String cardId = readCard();  // Lecture de l'identifiant de la carte RFID

    if (cardId != "") {
        WiFiClientSecure client;  // Utilisation de WiFiClientSecure pour HTTPS
        client.setInsecure();  // Ignorer la vérification du certificat
        HTTPClient http;

        String url = String(serverUrl) + "?cardId=" + cardId;  // Construction de l'URL avec l'identifiant de la carte

        if (http.begin(client, url)) {
            int httpCode = http.GET();

            if (httpCode == 200) {
                DynamicJsonDocument doc(1024);  // Création d'un document JSON dynamique pour stocker la réponse
                DeserializationError error = deserializeJson(doc, http.getString());

                if (error) {
                    Serial.print("JSON parsing failed! Error: ");
                    Serial.println(error.c_str());
                } else {
                    const char* status_ = doc["status"];

                    if (strcmp(status_, "Autorisé") == 0) {
                        Serial.println("Accès Autorisé");
                    } else if (strcmp(status_, "Refusé") == 0) {
                        Serial.println("Accès Refusé");
                    } else if (strcmp(status_, "Inconnu") == 0) {
                        Serial.println("Carte Inconnue");
                    } else {
                        Serial.println("Statut Inconnu");
                    }
                }
            } else {
                Serial.print("HTTP Code: ");
                Serial.println(httpCode);
                Serial.println("Error accessing server");
            }

            http.end();
        }
    }

    delay(1000);
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
