#include "analog_bass.h"
#include "pins.h"
#include "patch_cables.h"

volatile float cable_Bass_Final = 0.0;
volatile float vcf_Cutoff = 0.2;
volatile float vcf_Reso = 0.5;
volatile float env_Decay = 0.9992;
volatile float vca_Drive = 2.0;

static volatile float vco_Cap = 0.0;
static volatile bool lastSqHigh = false, subState = false;
static volatile float env_Cap = 0.0;
static volatile float vcf_Cap1 = 0.0, vcf_Cap2 = 0.0;

void IRAM_ATTR module_AnalogBass_Simulation() {
  float pitchZap = cable_Bass_Freq * (1.0 + env_Cap * env_Cap * 0.35);
  vco_Cap += pitchZap / (float)SAMPLE_RATE;
  if (vco_Cap >= 1.0) vco_Cap -= 1.0;
  float vcoSaw = (vco_Cap * 2.0) - 1.0;
  float vcoSq = (vco_Cap < 0.5) ? 1.0 : -1.0;

  bool sqHigh = (vcoSq > 0.0);
  if (sqHigh && !lastSqHigh) subState = !subState;
  lastSqHigh = sqHigh;
  float subOut = subState ? 1.0 : -1.0;

  float bassMix = (vcoSaw * 0.60) + (subOut * 0.40);

  if (cable_Bass_Gate) env_Cap = 1.0;
  else env_Cap *= env_Decay;

  float totalCut = vcf_Cutoff + (env_Cap * 0.55);
  if (totalCut > 0.88) totalCut = 0.88;
  if (totalCut < 0.02) totalCut = 0.02;

  float feedback = (vcf_Reso * 3.6) * (vcf_Cap1 - vcf_Cap2);
  vcf_Cap1 += totalCut * (bassMix - vcf_Cap1 + feedback);
  vcf_Cap2 += totalCut * (vcf_Cap1 - vcf_Cap2);

  float amp = vcf_Cap2 * env_Cap * vca_Drive;
  if (amp > 1.0) amp = 1.0;
  if (amp < -1.0) amp = -1.0;
  cable_Bass_Final = amp;
}
