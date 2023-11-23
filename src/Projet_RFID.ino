#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "Seb";  // Nom du réseau WiFi
const char* password = "12345678914";  // Mot de passe du réseau WiFi
const char* serverUrl = "https://cloud.sebhost.fr/rfid-app/checkAccess.php";  // URL du serveur pour la vérification d'accès via RFID (utilisation de HTTPS)

#define SS_PIN 15  // Broche Slave Select pour le MFRC522 (D8)
#define RST_PIN 2   // Broche Reset pour le MFRC522 (D4)
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Initialisation du MFRC522

void setup() {
    Serial.begin(115200);  // Initialisation de la communication série
    WiFi.begin(ssid, password);  // Connexion au réseau WiFi

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");  // Attente de la connexion au WiFi
    }
    Serial.println("Connected to WiFi");

    SPI.begin();  // Initialisation de la communication SPI
    mfrc522.PCD_Init();  // Initialisation du MFRC522
}

void loop() {
    String cardId = readCard();  // Lecture de l'identifiant de la carte RFID

    if (cardId != "") {
        WiFiClientSecure client;  // Utilisation de WiFiClientSecure pour HTTPS
        client.setInsecure();  // Ignorer la vérification du certificat (non recommandé dans un environnement de production)

        HTTPClient http;  // Initialisation de l'objet HTTPClient

        String url = String(serverUrl) + "?cardId=" + cardId;  // Construction de l'URL avec l'identifiant de la carte

        if (http.begin(client, url)) {
            int httpCode = http.GET();  // Requête HTTP GET

            if (httpCode == 200) {
                DynamicJsonDocument doc(1024);  // Création d'un document JSON dynamique pour stocker la réponse
                DeserializationError error = deserializeJson(doc, http.getString());  // Désérialisation de la réponse JSON

                if (error) {
                    Serial.print("JSON parsing failed! Error: ");
                    Serial.println(error.c_str());  // Affichage d'une erreur en cas d'échec de la désérialisation JSON
                } else {
                    const char* status = doc["status"];  // Extraction du statut de la réponse JSON

                    if (strcmp(status, "Autorisé") == 0) {
                        Serial.println("Accès Autorisé");  // Affichage du message en cas d'accès autorisé
                    } else if (strcmp(status, "Refusé") == 0) {
                        Serial.println("Accès Refusé");  // Affichage du message en cas d'accès refusé
                    } else if (strcmp(status, "Inconnu") == 0) {
                        Serial.println("Carte Inconnue");  // Affichage du message en cas de carte inconnue
                    } else {
                        Serial.println("Statut Inconnu");  // Affichage du message en cas de statut inconnu
                    }
                }
            } else {
                Serial.print("HTTP Code: ");
                Serial.println(httpCode);
                Serial.println("Error accessing server");  // Affichage d'une erreur en cas d'échec d'accès au serveur
            }

            http.end();  // Libération des ressources de l'objet HTTPClient
        }
    }

    delay(1000);  // Attente d'une seconde avant la prochaine itération
}

String readCard() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String cardId = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            cardId += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");  // Formattage de l'octet en deux caractères hexadécimaux
            cardId += String(mfrc522.uid.uidByte[i], HEX);
        }
        cardId.toUpperCase();  // Conversion de l'identifiant de la carte en majuscules
        return cardId;
    }

    return "";  // Retourne une chaîne vide si aucune carte n'est détectée
}
