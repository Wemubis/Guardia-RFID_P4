#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

  

const char* ssid = "Seb";
const char* password = "12345678";
const char* serverUrl = "http://172.20.10.2/rfid-app/checkAccess.php";
const int ledPin = D2;  // Remplacez D2 par le numéro de la broche de votre LED

  

#define SS_PIN 15 // D8
#define RST_PIN 2 // D4
MFRC522 mfrc522(SS_PIN, RST_PIN);

  

void setup() {

    Serial.begin(115200);

    WiFi.begin(ssid, password);

  

    while (WiFi.status() != WL_CONNECTED) {

        delay(1000);

        Serial.println("Connecting to WiFi...");

    }

    Serial.println("Connected to WiFi");

  

    pinMode(ledPin, OUTPUT);

    digitalWrite(ledPin, LOW);  // Éteindre la LED au démarrage

  

    SPI.begin();

    mfrc522.PCD_Init();

}

  

void loop() {

    // Lire l'ID de la carte RFID

    String cardId = readCard();

  

    if (cardId != "") {

        // Effectuer une requête HTTP au serveur

        WiFiClient client;

        HTTPClient http;

  

        String url = String(serverUrl) + "?cardId=" + cardId;

        http.begin(client, url);

  

        int httpCode = http.GET();

        if (httpCode == 200) {

    // Analyser la réponse JSON

    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, http.getString());

  

    if (error) {

        Serial.print("JSON parsing failed! Error: ");

        Serial.println(error.c_str());

    } else {

        // Vérifier le statut d'accès

        const char* status = doc["status"];

  

        if (strcmp(status, "Autorisé") == 0) {

            Serial.println("Accès Autorisé");

            // Allumez la LED ou effectuez d'autres actions autorisées

            digitalWrite(ledPin, HIGH);

            delay(5000);

            digitalWrite(ledPin, LOW);

        } else if (strcmp(status, "Refusé") == 0) {

            Serial.println("Accès Refusé");

            // Gérez un accès refusé

        } else if (strcmp(status, "Inconnu") == 0) {

            Serial.println("Carte Inconnue");

            // Gérez une carte inconnue

        } else {

            Serial.println("Unknown status");

            // Gérez d'autres statuts si nécessaire

        }

    }

} else {

    Serial.print("HTTP Code: ");

    Serial.println(httpCode);

    Serial.println("Error accessing server");

}

  

        http.end();

    }

  

    delay(2000);  // Attendez 5 secondes avant de lire la carte suivante (ajustez selon vos besoins)

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

