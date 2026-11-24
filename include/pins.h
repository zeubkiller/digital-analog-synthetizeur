#pragma once
#include <Arduino.h>

// Écran OLED I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define I2C_SDA 8
#define I2C_SCL 9

// 5 Potentiomètres 100k (Broches ADC1 valides sur ESP32-C3 : GPIO 0 à 4)
#define POT_DRIVE  0
#define POT_DECAY  1
#define POT_RESO   2
#define POT_CUTOFF 3
#define POT_TEMPO  4

// Boutons & Sortie Audio
#define BTN_PATTERN 5
#define BTN_BACKING 6
#define PIN_AUDIO   7

// Paramètres Audio
#define SAMPLE_RATE 22050
