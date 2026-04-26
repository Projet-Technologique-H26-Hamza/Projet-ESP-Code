#include "WifiManager.h"

void WifiManager::begin(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;

    WiFi.begin(_ssid, _password);
}

void WifiManager::update() {
    if (WiFi.status() == WL_CONNECTED) return;

    if (millis() - _lastAttempt > 5000) {
        Serial.println("Tentative connexion WiFi...");
        WiFi.begin(_ssid, _password);
        _lastAttempt = millis();
    }
}

bool WifiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}