# Réalisé par :
  Hamza Mahjoubi le 2026-04-16 
  version 1.0.0

## Système de Détection pour Distributeur de Savon Intelligent (IoT)

Ce programme assure le fonctionnement d'un capteur infrarouge avec contact intégré à un système de distribution automatique. Il permet de détecter en temps réel l'état du niveau de liquide : 
le système passe à l'état activé lors de la détection du liquide et à l'état désactivé en cas d'absence.
Grâce à une connexion sécurisée vers un topic MQTT spécifique, le dispositif transmet instantanément les informations d'état du distributeur. 
Cette communication déclenche automatiquement une mise à jour des données dans la base de données centrale. Avant toute opération, le programme gère une phase d'inscription automatique du distributeur pour 
configurer ses accès au réseau.
Le projet est entièrement développé en C++.

## Description des fonctionnalités

Le programme assure le fonctionnement du capteur infrarouge avec contact. Il détecte deux états distincts : l'activation lors de la présence de liquide et l'extinction en cas d'absence de liquide. 

Le cycle de fonctionnement comprend :
* **Provisioning et Inscription** : Phase initiale où le distributeur s'enregistre pour obtenir ses accès sécurisés.
* **Authentification** : Utilisation d'un module de login pour valider l'identité du matériel auprès de l'API.
* **Communication MQTTS** : Envoi des changements d'état en temps réel via un broker sécurisé, permettant une mise à jour immédiate de la base de données.

## Architecture logicielle

Le projet est organisé selon une structure modulaire :

* **Dossier includes** : Contient les définitions des classes et les prototypes des méthodes (fichiers `.h` et `.hpp`).
* **Dossier src** : Contient l'implémentation de la logique métier :
    * **Module Login** : Gestion exclusive des requêtes d'accès vers l'API.
    * **Module Water** : Gestion du statut des détections et configuration des broches (pins) du capteur infrarouge.
    * **Module Update** : Logique de publication des messages sur les topics MQTT sécurisés.
* **Dossier config** : Contient le fichier `config.h` regroupant les constantes de connexion.

## Configuration (Exemple : config.h)

Le fichier de configuration centralise tous les paramètres réseau et applicatifs. Voici la structure utilisée :

#ifndef CONFIG_H
#define CONFIG_H

// Paramètres de connexion WiFi
#define WIFI_SSID "NOM_DU_RESEAU_WIFI"
#define WIFI_PASSWORD "MOT_DE_PASSE_WIFI"

// Configuration de l'API (HTTPS)
#define API_HOST "votre.domaine.ca"
#define API_PORT 443
#define API_ENDPOINT "/api/dispenser/login"
#define DISPENSER_NAME "identifiant_unique"
#define DISPENSER_PASSWORD "mot_de_passe_securise"

// Configuration MQTT Sécurisé (MQTTS)
#define MQTT_HOST "votre.domaine.ca" 
#define MQTT_PORT 8883 
#define MQTT_USER "utilisateur_mqtt"
#define MQTT_PASSWD "mot_de_passe_mqtt"

// Topics MQTT
#define TOPIC_SCANS "nom/status"
#define TOPIC_SCANS_RESPONSE "nom/reponse"
#define TOPIC_SCANS_DYNAMIC_RESPONSE "nom/reponse_dynamique"

#endif

## Bibliothèques utilisées
Le développement s'appuie sur les ressources suivantes pour le pilotage matériel et la sécurité :

PubSubClient : Pour la gestion de la communication avec le broker MQTT.

WiFiClientSecure : Pour l'établissement de la couche de sécurité TLS/SSL nécessaire au protocole MQTTS (port 8883).

Wire / GPIO Stack : Pour l'interaction avec les entrées/sorties physiques et la lecture du capteur infrarouge.

## Installation et déploiement
Configuration matérielle : Brancher le capteur infrarouge sur les broches spécifiées dans le module de gestion du capteur (page water).

Configuration logicielle : Modifier le fichier config.h avec les identifiants de votre environnement.

Compilation : Utiliser un environnement de développement compatible C++ (type PlatformIO ou Arduino IDE) pour téléverser le code sur l'appareil.


