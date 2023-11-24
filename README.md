# Projet-4-RFID

### Conception du circuit et sa documentation : 

  #### 1) ESP8266 Module (NodeMCU) :

  Ce module est utilisé comme microcontrôleur principal pour le système. Il intègre le module WiFi ESP8266 et offre une plateforme compatible avec l'Arduino.
  

  #### 2) RFID Module (MFRC522):
 
  Le module RFID MFRC522 est utilisé pour la lecture des cartes RFID. Il communique avec l'ESP8266 via SPI (Serial Peripheral Interface).
  
  Un deuxieme module RFID MFRC522 est utilisé pour l'ecriture des cartes RFID. Il communique avec l'ESP32-C3 également via SPI (Serial Peripheral Interface).


  #### 3) Raspberry Pi 4 :
  
  Le Raspberry Pi 4 permet d'avoir une base de donnée SQLi auquel notre module ESP8266 peut se connecter pour avoir des informations sur les cartes, savoir si elles sont valides ou non ainsi que leurs droits d'accès.
  
  La communication entre la base de données et l'ESP8266 se fait en WiFi.


  #### 4) Schéma du circuit :

<!-- ![schema du circuit](https://github.com/SebastienCherki/Projet-4-RFID/blob/main/Schéma%20ESP32%20et%20RC522.png?raw=true) -->
![[Schema_ESP32_RC522.png|500]]

<br><br>

## Justification des choix techniques et UX : 
### 1) Utilisation de bibliothèques :

Nous avons intégré des bibliothèques telles que **ESP8266WiFi.h**, **ESP8266HTTPClient.h**, **ArduinoJson.h**, et **MFRC522.h** en plus de celles de base pour simplifier notre travail avec l'ESP8266, les requêtes HTTP, le traitement de JSON, et le lecteur RFID.

### 2) Configuration WiFi :

Les informations de connexion WiFi (SSID et mot de passe) sont stockées en tant que constantes (ssid et password) pour une meilleure lisibilité. Cependant, il est recommandé d'adopter des méthodes plus sécurisées pour stocker ces données sensibles.

### 3) URL du serveur :

L'URL du serveur est définie en tant que constante _(serverUrl)_, facilitant ainsi d'éventuelles modifications en cas de changement de serveur. Cela rend notre code plus flexible.

### 4) HTTPS et WiFiClientSecure :

Pour sécuriser les échanges de données, nous utilisons le protocole HTTPS avec _WiFiClientSecure_, assurant un chiffrement des données entre le périphérique et le serveur.

### 5) Communication série :

La communication série est intégrée pour faciliter le débogage en affichant des messages d'état sur le moniteur série, aidant à comprendre le déroulement du programme.

### 6) Structure du code :

Le code est organisé en differentes fonctions de base _`setup()`_ et _`loop()`_, séparant ainsi l'initialisation des opérations répétitives.

On y a ajouté plusieurs autres fonctions pour améliorer la lisibilité du code et sa compréhension.
- On a les fonctions _`addText()`_ & _`changeKeys()`_ dans [write_key_text_card.ino](src/write_key_text_card.ino) qui nous permettent d'ajouter du texte sur 16 bytes et de changer les clefs sur tous les secteurs sauf le secteur 0 qui reste en clair (choix fait par l'équipe).
- Et on a les fonctions _`readCard()`_ & _`checkKeys()`_ dans [Projet_RFID.ino](src/Projet_RFID.ino) qui nous permettent de lire les UID des badges et de vérifier l'authentification avec les bonnes clefs sur les secteurs où elles ont été modifies.

### 7) Gestion des erreurs :

Des vérifications d'erreur sont incluses, telles que la vérification du statut de la connexion WiFi, du status après authentification ou ecriture sur les cartes et la gestion des erreurs HTTP, assurant la robustesse du programme même en cas de problèmes.

Pour chaque erreur, on récupère le code associé, on affiche le tout sur la sortie série et on sort de nos conditions/boucles pour revenir au stade de départ de la fonction `void loop()`.

On a récupéré les codes d'erreur à l'aide de ces fonctions :
- _`mfrc522.GetStatusCodeName(status)`_ lors des tests d'authentification ou lors d'écritures sur la carte
- _`httpCode`_ pour les codes d'erreur HTTP
- _`errer.c_str()`_ pour le parking du JSON

<br>

> [!NOTE]
> Un délai d'une seconde _`delay(1000)`_ entre les lectures de cartes RFID a été ajouté pour éviter une lecture trop fréquente, offrant une personnalisation possible en fonction des besoins spécifiques.
