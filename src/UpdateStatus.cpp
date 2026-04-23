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
    espClient.setInsecure(); 
    
    mqtt.setClient(espClient);
    mqtt.setServer(MQTT_HOST, MQTT_PORT);

    while (!mqtt.connected()) {
        Serial.print("Tentative de connexion MQTTS...");
        if (mqtt.connect("ESP32ClientStone", MQTT_USER, MQTT_PASSWD)) {
            Serial.println(" connecté !");
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

    String topic = "/status/" + String(DISPENSER_NAME);
    
    String payload = "{\"name\":\"" + String(DISPENSER_NAME) + "\",";
    payload += "\"is_empty\":" + String(isEmpty ? "1" : "0") + ",";
    payload += "\"token\":\"" + _token + "\"}";

    mqtt.publish(topic.c_str(), payload.c_str());
}
