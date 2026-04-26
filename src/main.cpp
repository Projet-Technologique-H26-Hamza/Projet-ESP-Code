#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "Login.h"
#include "UpdateStatus.h"
#include "WaterSensor.h"

constexpr uint8_t SENSOR_PIN = 23;

constexpr unsigned long SERIAL_BAUD = 115200;
constexpr unsigned long SERIAL_START_DELAY = 1000;

constexpr unsigned long WIFI_RETRY_DELAY_MS = 500;

constexpr unsigned long DEBOUNCE_TIME_MS = 2000;
constexpr unsigned long STATE_CONFIRM_TIME_MS = 50;

constexpr unsigned long LOOP_DELAY_MS = 10;

constexpr int DISPENSER_ID = 6;

WaterSensor monCapteur(SENSOR_PIN);
Login login;
WiFiClient espClient;
PubSubClient mqtt(espClient);
UpdateStatus updater(mqtt);

bool lastIsEmpty = true;
bool lastStableState = true;
unsigned long lastChangeTime = 0;

unsigned long lastWifiAttempt = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(SERIAL_START_DELAY);
    Serial.println("DÉMARRAGE DU SYSTÈME");

    Serial.print("1. Connexion WiFi à: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(WIFI_RETRY_DELAY_MS);
        Serial.print(".");
    }

    Serial.println("\n WiFi Connecté! IP: " + WiFi.localIP().toString());

    Serial.println("2. Tentative de Login API...");
    String monToken = "";
    int loginResult = login.LoginDispenser(monToken);

    if (loginResult != -1) {
        updater.setToken(monToken);
        updater.setId(DISPENSER_ID);
    } else {
        Serial.println(" Login échoué ");
    }

    updater.connectMQTT();
}

void loop() {

    if (WiFi.status() != WL_CONNECTED) {
        if (millis() - lastWifiAttempt > 5000) {
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            lastWifiAttempt = millis();
        }
    }

    if (!mqtt.connected()) {
        updater.connectMQTT();
    }

    mqtt.loop();

    bool isHandPresent = monCapteur.estDetecte();

    if (isHandPresent != lastStableState) {
        lastChangeTime = millis();
        lastStableState = isHandPresent;
    }

    if ((millis() - lastChangeTime) > STATE_CONFIRM_TIME_MS) {
        if (isHandPresent != !lastIsEmpty) {

            updater.sendStatus(isHandPresent);
            lastIsEmpty = !isHandPresent;

            Serial.print("Changement d'état détecté ! Nouveau statut : ");
            Serial.println(isHandPresent ? "OCCUPÉ (1)" : "LIBRE (0)");
        }
    }

    delay(LOOP_DELAY_MS);
}