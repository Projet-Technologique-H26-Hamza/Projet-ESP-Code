#include "Login.h"
#include "config.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h> // Nécessaire pour le HTTPS

Login::Login() {}

int Login::LoginDispenser(String &tokenOut) {
    if (WiFi.status() != WL_CONNECTED) return -1;

    WiFiClientSecure *client = new WiFiClientSecure;
    client->setInsecure(); 

    HTTPClient http;
    
    String url = "https://" + String(API_HOST) + String(API_ENDPOINT);
    
    http.begin(*client, url);
    http.addHeader("Content-Type", "application/json");

    String jsonBody = "{\"name_soap_dispenser\":\"" + String(DISPENSER_NAME) + "\",\"password\":\"" + String(DISPENSER_PASSWORD) + "\"}";
    
    int code = http.POST(jsonBody);

    if (code == 200) {
        String response = http.getString();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);
        
        if (doc.containsKey("token")) {
            tokenOut = doc["token"].as<String>();
            http.end();
            delete client; 
            return 1;
        }
    } else {
        Serial.printf(" Erreur HTTP au Login : %d\n", code);
    }
    
    http.end();
    delete client;
    return -1;
}
