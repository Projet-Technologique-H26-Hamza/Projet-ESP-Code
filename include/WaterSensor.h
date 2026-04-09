#ifndef WATERSENSOR_H
#define WATERSENSOR_H
#include <Arduino.h>
 
class WaterSensor {
private:
    int _pin;
    void init(); 
public:
    WaterSensor(int pin);    
    bool estDetecte();   
    void UpdateTopic();
};
 
#endif