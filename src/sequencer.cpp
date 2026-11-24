#include "sequencer.h"
#include "pins.h"
#include "patch_cables.h"

volatile float cable_Bass_Freq = 43.65;
volatile bool cable_Bass_Gate = false;
volatile float cable_Backing_Out = 0.0;

volatile int currentPattern = 0;
volatile int currentStep = 0;
volatile bool backingEnabled = true;
volatile unsigned long samplesPerStep = 1850;

static volatile unsigned long clockCounter = 0;
static volatile float kick_Phase = 0.0, kick_Env = 0.0;
static volatile float mel_Phase = 0.0, mel_Env = 0.0, mel_Freq = 261.63;

static const float noteFreqs[49] = {
    32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74,
    65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
    130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
    261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
    523.25};

static const uint8_t bassPatterns[3][16] = {
    {0, 0, 48, 48, 0, 0, 52, 52, 0, 0, 53, 53, 0, 0, 55, 52},
    {36, 48, 36, 48, 40, 52, 40, 52, 41, 53, 41, 53, 43, 55, 43, 55},
    {36, 36, 48, 36, 41, 41, 53, 41, 43, 43, 55, 43, 40, 40, 52, 43}};

static const uint8_t melodyPatterns[3][16] = {
    {60, 0, 64, 0, 67, 0, 64, 67, 69, 0, 67, 0, 64, 0, 62, 0},
    {72, 0, 67, 64, 67, 0, 72, 0, 69, 0, 65, 69, 67, 0, 64, 67},
    {60, 64, 67, 72, 67, 64, 60, 0, 65, 69, 72, 69, 67, 64, 62, 67}};

void IRAM_ATTR module_ESP32_SequencerAndBacking()
{
  clockCounter += 1;
  if (clockCounter >= samplesPerStep)
  {
    clockCounter = 0;

    uint8_t bNote = bassPatterns[currentPattern][currentStep];
    if (bNote >= 24 && bNote <= 72)
    {
      cable_Bass_Freq = noteFreqs[bNote - 24];
      cable_Bass_Gate = true;
    }
    else
    {
      cable_Bass_Gate = false;
    }

    if (backingEnabled)
    {
      if (currentStep % 4 == 0)
      {
        kick_Env = 1.0;
        kick_Phase = 0.0;
      }
      uint8_t mNote = melodyPatterns[currentPattern][currentStep];
      if (mNote >= 24 && mNote <= 72)
      {
        mel_Freq = noteFreqs[mNote - 24];
        mel_Env = 1.0;
      }
    }
    currentStep = (currentStep + 1) % 16;
  }
  else
  {
    if (clockCounter > 120)
      cable_Bass_Gate = false;
  }

  float kickSound = 0.0;
  float melSound = 0.0;

  if (backingEnabled)
  {
    kick_Env *= 0.9993;
    if (kick_Env > 0.01)
    {
      float kFreq = 42.0 + (kick_Env * kick_Env * 120.0);
      kick_Phase += kFreq / (float)SAMPLE_RATE;
      if (kick_Phase >= 1.0)
        kick_Phase -= 1.0;
      float kWave = (kick_Phase < 0.5) ? (kick_Phase * 4.0 - 1.0) : (3.0 - kick_Phase * 4.0);
      kickSound = kWave * kick_Env * 0.75;
    }

    mel_Env *= 0.9994;
    if (mel_Env > 0.01)
    {
      mel_Phase += mel_Freq / (float)SAMPLE_RATE;
      if (mel_Phase >= 1.0)
        mel_Phase -= 1.0;
      float mWave = (mel_Phase < 0.35) ? 1.0 : -1.0;
      melSound = mWave * mel_Env * 0.22;
    }
  }
  cable_Backing_Out = kickSound + melSound;
}
