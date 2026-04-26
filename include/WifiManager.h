#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WifiManager {
public:
    void begin(const char* ssid, const char* password);
    void update();  
    bool isConnected();

private:
    const char* _ssid;
    const char* _password;
    unsigned long _lastAttempt = 0;
};