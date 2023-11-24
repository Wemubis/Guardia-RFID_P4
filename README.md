# Projet-4-RFID

### Conception du circuit et sa documentation : 

  #### 1) ESP8266 Module (NodeMCU) :

  Ce module est utilisé comme microcontrôleur principal pour le système. Il intègre le module WiFi ESP8266 et offre une plateforme compatible avec l'Arduino.

  #### 2) RFID Module (MFRC522):
 
  Le module RFID MFRC522 est utilisé pour la lecture des cartes RFID. Il communique avec l'ESP8266 via SPI (Serial Peripheral Interface).


  
  La connection entre l'ESP32-C3 et le lecteur de carte RFID-RC522 s'effectue en SPI.
  La communication entre la base de données (Raspberry Pi 4) et l'ESP32-C3 se fera en WiFi.

  #### 3) Schéma du circuit :

![schema du circuit](https://github.com/SebastienCherki/Projet-4-RFID/blob/main/Schéma%20ESP32%20et%20RC522.png?raw=true)


## Justification des choix techniques et UX : 
### 1) Utilisation de bibliothèques :

Nous avons intégré des bibliothèques telles que **ESP8266WiFi.h**, **ESP8266HTTPClient.h**, **ArduinoJson.h**, et **MFRC522.h** pour simplifier notre travail avec l'ESP8266, les requêtes HTTP, le traitement de JSON, et le lecteur RFID.

### 2) Configuration WiFi :

Les informations de connexion WiFi (SSID et mot de passe) sont stockées en tant que constantes (ssid et password) pour une meilleure lisibilité. Cependant, il est recommandé d'adopter des méthodes plus sécurisées pour stocker ces données sensibles.

### 3) URL du serveur :

L'URL du serveur est définie en tant que constante _(serverUrl)_, facilitant ainsi d'éventuelles modifications en cas de changement de serveur. Cela rend notre code plus flexible.

### 4) HTTPS et WiFiClientSecure :

Pour sécuriser les échanges de données, nous utilisons le protocole HTTPS avec _WiFiClientSecure_, assurant un chiffrement des données entre le périphérique et le serveur.

### 5) Communication série :

La communication série est intégrée pour faciliter le débogage en affichant des messages d'état sur le moniteur série, aidant à comprendre le déroulement du programme.

### 6) Structure du code :

Le code est organisé en fonctions _`setup()`_ et _`loop()`_, séparant ainsi l'initialisation des opérations répétitives. La fonction `readCard()` encapsule la logique de lecture des cartes RFID, améliorant la lisibilité.

### 7) Gestion des erreurs :

Des vérifications d'erreur sont incluses, telles que la vérification du statut de la connexion WiFi et la gestion des erreurs HTTP, assurant la robustesse du programme même en cas de problèmes.

### 8) Fonction de délai :

Un délai d'une seconde _`delay(1000)`_ entre les lectures de cartes RFID a été ajouté pour éviter une lecture trop fréquente, offrant une personnalisation possible en fonction des besoins spécifiques.
