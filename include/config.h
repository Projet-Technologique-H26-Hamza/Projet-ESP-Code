#ifndef CONFIG_H
#define CONFIG_H



// ===== WiFi EcoleDuWeb2.4g   EcoleDuWEB@@  172.16.7.161 pour api  =====
#define WIFI_SSID "Residence"
#define WIFI_PASSWORD "I0FvvB7eGtwN"

// ===== API login (HTTPS via Nginx) =====
// On passe par le port 443 (HTTPS standard)
#define API_HOST "distributeur.alain.lab.edwrdl.ca"
#define API_PORT 443
#define API_ENDPOINT "/api/dispenser/login"
#define DISPENSER_NAME "string123"
#define DISPENSER_PASSWORD "Patate123"



// On utilise le nom de domaine pour que le certificat SSL soit valide
#define MQTT_HOST "distributeur.alain.lab.edwrdl.ca" 
#define MQTT_PORT 8883 // Port MQTTS standard
#define MQTT_USER "admin"
#define MQTT_PASSWD "Patate123"



// Topics MQTT
#define TOPIC_SCANS "dispenser/status"
#define TOPIC_SCANS_RESPONSE "dispenser/response"
#define TOPIC_SCANS_DYNAMIC_RESPONSE "dispenser/dynamic_response"



#endif