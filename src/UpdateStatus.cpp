#include "UpdateStatus.h"
#include "config.h"
#include <Arduino.h>
#include <WiFiClientSecure.h> // Nécessaire pour le MQTTS

// On prépare le client sécurisé
static WiFiClientSecure espClient;

UpdateStatus::UpdateStatus(PubSubClient &mqttClient) : mqtt(mqttClient) {}

void UpdateStatus::setId(int id) {
    _myId = id;
}

void UpdateStatus::setToken(String token) {
    _token = token; 
}

void UpdateStatus::connectMQTT() {
    // Indispensable pour accepter ton certificat auto-signé sur le port 8883
    espClient.setInsecure(); 
    
    // On lie le client sécurisé à PubSubClient
    mqtt.setClient(espClient);
    mqtt.setServer(MQTT_HOST, MQTT_PORT);

    while (!mqtt.connected()) {
        Serial.print("Tentative de connexion MQTTS...");
        if (mqtt.connect("ESP32ClientStone", MQTT_USER, MQTT_PASSWD)) {
            Serial.println(" connecté !");
            // Topic initial avec le NOM
            String initTopic = "/status/" + String(DISPENSER_NAME);
            mqtt.publish(initTopic.c_str(), "online");
            mqtt.subscribe(TOPIC_SCANS_RESPONSE);
        } else {
            Serial.print(" échec, code erreur : ");
            Serial.print(mqtt.state());
            Serial.println(" ... nouvelle tentative dans 5s");
            delay(5000);
        }
    }
}

void UpdateStatus::sendStatus(bool isEmpty) {
    if (!mqtt.connected()) {
        connectMQTT();
    }

    // Utilisation du NOM dans le topic au lieu de l'ID
    String topic = "/status/" + String(DISPENSER_NAME);
    
    // JSON envoyé au broker contenant le token
    String payload = "{\"name\":\"" + String(DISPENSER_NAME) + "\",";
    payload += "\"is_empty\":" + String(isEmpty ? "1" : "0") + ",";
    payload += "\"token\":\"" + _token + "\"}";

    mqtt.publish(topic.c_str(), payload.c_str());
    Serial.println("📤 Message envoyé avec le Token sur topic : " + topic);
}