#include "WaterSensor.h"
 
WaterSensor::WaterSensor(int pin) {
    _pin = pin;
    init();
}
 
void WaterSensor::init() {
    
    pinMode(_pin, INPUT_PULLUP);
}
 
bool WaterSensor::estDetecte() {
    
    return (digitalRead(_pin) == LOW);
}
void WaterSensor::UpdateTopic() {
}