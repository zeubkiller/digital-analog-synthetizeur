#include <Arduino.h>
#include "pins.h"
#include "patch_cables.h"
#include "sequencer.h"
#include "analog_bass.h"
#include "ui_display.h"

static hw_timer_t *audioTimer = NULL;

void IRAM_ATTR onAudioTimer()
{
  module_ESP32_SequencerAndBacking();
  module_AnalogBass_Simulation();

  float masterOut = (cable_Bass_Final * 0.65) + (cable_Backing_Out * 0.55);
  if (masterOut > 1.0)
    masterOut = 1.0;
  if (masterOut < -1.0)
    masterOut = -1.0;

  uint32_t pwmOut = (uint32_t)((masterOut + 1.0) * 511.5);
  if (pwmOut > 1023)
    pwmOut = 1023;
  ledcWrite(PIN_AUDIO, pwmOut);
}

void setup()
{
  Serial.begin(115200);
  initUI();

  // Configuration PWM Audio (compatible Arduino ESP32 Core v3)
  ledcAttach(PIN_AUDIO, 44100, 10);

  // Configuration Timer Audio à 22 050 Hz (Core v3)
  audioTimer = timerBegin(1000000);
  timerAttachInterrupt(audioTimer, &onAudioTimer);
  timerAlarm(audioTimer, 1000000 / SAMPLE_RATE, true, 0);
}

void loop()
{
  updateControlsAndUI();
  delay(10);
}
