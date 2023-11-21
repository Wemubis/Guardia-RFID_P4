#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

//https://randomnerdtutorials.com/esp32-https-requests/#esp32-https-requests-httpclient-without-certificate

const char* ssid = "Seb";
const char* password = "12345678";
const char* serverUrl = "https://172.20.10.2/rfid-app/checkAccess.php";
const int ledPin = D2;  // Remplacez D2 par le numéro de la broche de votre LED


const char* rootCACertificate = \ 
"-----BEGIN CERTIFICATE-----\n" \
"MIID+zCCAuOgAwIBAgIUctKpCws5JnEwtzrPO75taz8y/rQwDQYJKoZIhvcNAQEL\n" \
"BQAwgYwxCzAJBgNVBAYTAkZSMRMwEQYDVQQIDApTb21lLVN0YXRlMQ0wCwYDVQQH\n" \
"DARMeW9uMSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxDDAKBgNV\n" \ 
"BAMMA1NlYjEoMCYGCSqGSIb3DQEJARYZY2hlcmtpc2ViYXN0aWVuQGdtYWlsLmNv\n" \
"bTAeFw0yMzExMjExNTIxNDNaFw0yNDExMjAxNTIxNDNaMIGMMQswCQYDVQQGEwJG\n" \
"UjETMBEGA1UECAwKU29tZS1TdGF0ZTENMAsGA1UEBwwETHlvbjEhMB8GA1UECgwY\n" \
"9w0BCQEWGWNoZXJraXNlYmFzdGllbkBnbWFpbC5jb20wggEiMA0GCSqGSIb3DQEB\n" \
"SW50ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMQwwCgYDVQQDDANTZWIxKDAmBgkqhkiG\n" \ 
"9w0BCQEWGWNoZXJraXNlYmFzdGllbkBnbWFpbC5jb20wggEiMA0GCSqGSIb3DQEB\n" \
"AQUAA4IBDwAwggEKAoIBAQCG4w1JCzlMAmvByI49y1iRlBkuil4z5Hp0u4qZfW6p\n" \
"uihpSfMuPH10YNdw5iAOHqagzz4MfZPSQ8kY4fBFfKGOGjna1Xg/buRNuvWItkk6\n" \
"6TJ9gQzENnDdcNtLb8G18o0d4P9TCs/Bd7jp8VK4uNOLQxSOTFw7rFsiHOgqIfQK\n" \
"pf2vGgtpqRoeUkzIVgMzryGxF2Z2PpUZKYuICi+S1PzFLTBW4l3gWt4EUd9/vL7Q\n" \
"+TnQvkVQ2mLg0KOfjzULY1P1mcM4Vq2Epb+iY9J1hOXh+X8Ppxw//l1WGQi1tJ1A\n" \
"WlFFbKzcGcYzr3sZYsjjla7rCqibPJ7SN2lmne/q9M7bAgMBAAGjUzBRMB0GA1Ud\n" \
"DgQWBBQR8vSbptkn0SnhD/H/P/1/dXxNDzAfBgNVHSMEGDAWgBQR8vSbptkn0Snh\n" \
"D/H/P/1/dXxNDzAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQB0\n" \
"nJiny2TeH/R1svA+rZdbh9Z0tPXPlYKz4+PHChT0jAaAEft9xjUMXwdXdrJlcW7i\n" \
"yuInZb7O10t2qZlatrC5BXDEPu+8YLiwXYMF2UD2gjJYAfeb4GLZ9E1vXDJAoCuI\n" \
"MFnPbJ3uCV/s52GFTedjmptr6h8g8+JuPbKkNTmF4OPRI7jLG7oaN4XX3CZINQCl\n" \
"F8mSXd/aycAWpJ0qNH+q95ad2dmuQkWpcLp/oBaQkuKl3QfvFCMdcpv0YcvV6ZxY\n" \
"JG+3iDnA+M6i4vBdahtiKCmbqbmEa+KVKz6v9tKImEKf55j05CR04YmDePi3VyL1\n" \
"lGuoePXq0N8RS6r1BGa+\n" \
"-----END CERTIFICATE-----\n";

//https


#define SS_PIN D1 // D8
#define RST_PIN D0 // D4
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

  //Serial.println(curl)

}

  

void loop() {    
    // Lire l'ID de la carte RFID

    String cardId = readCard();

  

    if (cardId != "") {

        // Effectuer une requête HTTP au serveur

        WiFiClientSecure *client = new WiFiClientSecure; //https

        client->setCACert(rootCACertificate); //https
        
        WiFiClientSecure client;

        HTTPClient https;

  

        String url = String(serverUrl) + "?cardId=" + cardId;

        https.begin(client, url); //https au lieu de http

  

        int httpCode = https.GET(); //https au lieu de http

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
