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

    // Utilisation de WiFiClientSecure pour le HTTPS
    WiFiClientSecure *client = new WiFiClientSecure;
    // On ignore la vérification stricte du certificat pour simplifier (Insecure)
    // Utile si tu n'as pas le certificat root chargé dans l'ESP32
    client->setInsecure(); 

    HTTPClient http;
    
    // Construction de l'URL en HTTPS avec le domaine
    String url = "https://" + String(API_HOST) + String(API_ENDPOINT);
    
    Serial.print("📡 Connexion à : ");
    Serial.println(url);

    // On passe le client secure à http.begin
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
            
            Serial.println("🔑 TOKEN REÇU :");
            Serial.println(tokenOut); 
            
            http.end();
            delete client; // Libérer la mémoire
            return 1; // Succès
        }
    } else {
        Serial.printf("❌ Erreur HTTP au Login : %d\n", code);
        // Si code est -1, c'est souvent un problème de connexion SSL
    }
    
    http.end();
    delete client; // Libérer la mémoire
    return -1; // Échec
}