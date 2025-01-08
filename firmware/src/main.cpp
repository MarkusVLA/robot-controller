#include <Arduino.h>
#include <SPI.h>

// LED pin for esp32
#define LED 12

void setup(void){
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
}

void loop(void){
    Serial.println("Hello from ESP!");
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
}


