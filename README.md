# Projet-4-RFID

### Conception du circuit et sa documentation : 

  #### 1) ESP8266 Module (NodeMCU) :

  Ce module est utilisé comme microcontrôleur principal pour le système. Il intègre le module WiFi ESP8266 et offre une plateforme compatible avec l'Arduino.

  #### 2) RFID Module (MFRC522):
 
  Le module RFID MFRC522 est utilisé pour la lecture des cartes RFID. Il communique avec l'ESP8266 via SPI (Serial Peripheral Interface).

  #### 3) LED (Indicateur visuel) :

  Une LED est utilisée comme indicateur visuel pour signaler l'accès autorisé ou refusé. Elle est connectée à la broche D2 de l'ESP8266.


  
  La connection entre l'ESP32-C3 et le lecteur de carte RFID-RC522 s'effectue en SPI.
  La communication entre la base de données (Raspberry Pi 4) et l'ESP32-C3 se fera en WiFi.

### Justification des choix techniques et UX : 

  # Documentation du Projet

# Documentation du Projet

### 1) Utilisation de bibliothèques :

Nous avons intégré des bibliothèques telles que _ESP8266WiFi.h_, _ESP8266HTTPClient.h_, _ArduinoJson.h_, et _MFRC522.h_ pour simplifier notre travail avec l'ESP8266, les requêtes HTTP, le traitement de JSON, et le lecteur RFID.

### 2) Configuration WiFi :

Les informations de connexion WiFi (SSID et mot de passe) sont stockées en tant que constantes (ssid et password) pour une meilleure lisibilité. Cependant, il est recommandé d'adopter des méthodes plus sécurisées pour stocker ces données sensibles.

### 3) URL du serveur :

L'URL du serveur est définie en tant que constante (serverUrl), facilitant ainsi d'éventuelles modifications en cas de changement de serveur. Cela rend notre code plus flexible.

### 4) HTTPS et WiFiClientSecure :

Pour sécuriser les échanges de données, nous utilisons le protocole HTTPS avec _WiFiClientSecure_, assurant un chiffrement des données entre le périphérique et le serveur.

### 5) Configuration du module RFID :

Les broches _SS_PIN_ et _RST_PIN_ sont utilisées pour configurer le module RFID (_MFRC522_), garantissant son bon fonctionnement.

### 6) Communication série :

La communication série est intégrée pour faciliter le débogage en affichant des messages d'état sur le moniteur série, aidant à comprendre le déroulement du programme.

### 7) Structure du code :

Le code est organisé en fonctions `setup()` et `loop()`, séparant ainsi l'initialisation des opérations répétitives. La fonction `readCard()` encapsule la logique de lecture des cartes RFID, améliorant la lisibilité.

### 8) Gestion des erreurs :

Des vérifications d'erreur sont incluses, telles que la vérification du statut de la connexion WiFi et la gestion des erreurs HTTP, assurant la robustesse du programme même en cas de problèmes.

### 9) Fonction de délai :

Un délai d'une seconde (`delay(1000)`) entre les lectures de cartes RFID a été ajouté pour éviter une lecture trop fréquente, offrant une personnalisation possible en fonction des besoins spécifiques.
