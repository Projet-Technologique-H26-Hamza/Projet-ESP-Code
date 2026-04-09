#ifndef UPDATESTATUS_H
#define UPDATESTATUS_H

#include <PubSubClient.h>
#include <Arduino.h>

class UpdateStatus {
public:
    UpdateStatus(PubSubClient &mqttClient);
    
    // Pour mettre à jour l'ID (si besoin)
    void setId(int id);
    
    // NOUVEAU : Pour stocker le token reçu du login
    void setToken(String token);
    
    void connectMQTT();
    void sendStatus(bool isEmpty);

private:
    PubSubClient &mqtt;
    int _myId = 1;      // ID par défaut
    String _token = ""; // Stockage du jeton d'authentification
};

#endif