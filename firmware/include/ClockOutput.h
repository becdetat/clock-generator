#pragma once
#include <Arduino.h>

class ClockOutput {
public:
    ClockOutput(uint8_t outPin, int bpm, unsigned long pulseWidthMicros)
        : pin(outPin), bpm(bpm), pulseWidthMicros(pulseWidthMicros), lastPulseMicros(0), pulseActive(false), pulseStartMicros(0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void setBPM(int newBpm) {
        bpm = newBpm;
    }

    int getBPM() const {
        return bpm;
    }

    void update() {
        unsigned long interval = 60000000UL / bpm;
        unsigned long now = micros();
        if (now - lastPulseMicros >= interval) {
            lastPulseMicros = now;
            digitalWrite(pin, HIGH);
            pulseActive = true;
            pulseStartMicros = now;
        }
        if (pulseActive && (now - pulseStartMicros >= pulseWidthMicros)) {
            digitalWrite(pin, LOW);
            pulseActive = false;
        }
    }

private:
    uint8_t pin;
    int bpm;
    unsigned long pulseWidthMicros;
    unsigned long lastPulseMicros;
    bool pulseActive;
    unsigned long pulseStartMicros;
};
