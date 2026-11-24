#pragma once
#include <Arduino.h>

// --- Signaux venant de la PARTIE A (ESP32 Séquenceur) ---
extern volatile float cable_Bass_Freq;
extern volatile bool cable_Bass_Gate;
extern volatile float cable_Backing_Out;

// --- Signaux internes de la PARTIE B (Future Basse NE555 + LM358) ---
extern volatile float cable_Bass_Final;

// --- Paramètres partagés entre l'interface (UI) et le moteur audio ---
extern volatile int currentPattern;
extern volatile int currentStep;
extern volatile bool backingEnabled;
extern volatile unsigned long samplesPerStep;

extern volatile float vcf_Cutoff;
extern volatile float vcf_Reso;
extern volatile float env_Decay;
extern volatile float vca_Drive;
