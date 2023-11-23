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

#### 1) Utilisation de bibliothèques :

Nous avons intégré des bibliothèques telles que _ESP8266WiFi.h_, _ESP8266HTTPClient.h_, _ArduinoJson.h_, et _MFRC522.h_ pour profiter de fonctionnalités prêtes à l'emploi, simplifiant ainsi notre travail avec l'ESP8266, les requêtes HTTP, la manipulation de JSON, et le lecteur RFID.

#### 2) Configuration WiFi :

Nous avons stocké les informations de connexion WiFi, le SSID et le mot de passe, en tant que constantes (ssid et password). Cela rend notre code plus lisible, mais il est important de noter que dans un contexte de sécurité, il serait préférable d'adopter des méthodes plus sûres pour stocker ces informations.

#### 3) URL du serveur :

Nous avons défini l'URL du serveur en tant que constante (serverUrl) pour permettre une modification facile si jamais le serveur change. Cela rend notre code plus flexible.

#### 4) HTTPS et WiFiClientSecure :

Pour assurer la sécurité de nos données, nous avons choisi d'utiliser le protocole HTTPS en utilisant _WiFiClientSecure_. Cela garantit que les échanges entre le périphérique et le serveur sont chiffrés.

#### 5) Rétroaction LED :

Nous avons inclus une LED connectée à la broche D2 pour fournir une rétroaction visuelle claire à l'utilisateur ou à l'administrateur, indiquant si l'accès est autorisé ou refusé.

#### 6) Configuration du module RFID :

Nous avons utilisé les broches _SS_PIN_ et _RST_PIN_ pour configurer le module RFID (_MFRC522_). Cette configuration est nécessaire pour que le module fonctionne correctement.

#### 7) Communication série :

Nous avons intégré la communication série pour permettre le débogage en affichant des messages d'état sur le moniteur série. Cela aide à comprendre ce qui se passe pendant l'exécution du programme.

#### 8) Structure du code :

Nous avons organisé le code en fonctions setup() et loop(), ce qui permet de séparer l'initialisation des opérations répétitives. La fonction readCard() encapsule la logique de lecture des cartes RFID pour une meilleure lisibilité.

#### 9) Gestion des erreurs :

Nous avons inclus des vérifications d'erreur, comme la vérification du statut de la connexion WiFi et la gestion des erreurs HTTP, pour assurer la robustesse du programme même en cas de problèmes.

#### 10) Fonction de délai :

Nous avons introduit un délai d'une seconde (delay(1000)) entre les lectures de cartes RFID pour éviter une lecture trop fréquente. Cela peut être ajusté en fonction des besoins spécifiques.
