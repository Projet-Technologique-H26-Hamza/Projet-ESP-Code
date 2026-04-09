#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Login.h"
#include "UpdateStatus.h"
#include "WaterSensor.h"

WaterSensor monCapteur(23);
Login login;
WiFiClient espClient;
PubSubClient mqtt(espClient);
UpdateStatus updater(mqtt);

bool lastIsEmpty = true;
bool lastStableState = true;
unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 2000;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n======================================");
    Serial.println("DÉMARRAGE DU SYSTÈME");
    Serial.println("======================================");

    Serial.print("1. Connexion WiFi à: "); Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n WiFi Connecté! IP: " + WiFi.localIP().toString());

    Serial.println("2. Tentative de Login API...");
    String monToken = "";
    int loginResult = login.LoginDispenser(monToken);

    if (loginResult != -1) {
        Serial.println("Login réussi!");
        updater.setToken(monToken);
        updater.setId(6);
    } else {
        Serial.println(" Login échoué (ID -1). On utilise l'ID par défaut.");
    }

    Serial.println("3. Connexion au Broker MQTT...");
    updater.connectMQTT();
    Serial.println(" Setup terminé. Entrée en boucle infinie.");
}
void loop() {
    if (!mqtt.connected()) {
        updater.connectMQTT();
    }
    mqtt.loop();

    bool isHandPresent = monCapteur.estDetecte(); 

    if (isHandPresent != lastStableState) {
        lastChangeTime = millis(); 
        lastStableState = isHandPresent;
    }

    if ((millis() - lastChangeTime) > 50) { 
        if (isHandPresent != !lastIsEmpty) { 
            
            updater.sendStatus(isHandPresent); 
            lastIsEmpty = !isHandPresent; 
            
            Serial.print("Changement d'état détecté ! Nouveau statut : ");
            Serial.println(isHandPresent ? "OCCUPÉ (1)" : "LIBRE (0)");
        }
    }

    delay(10);
}