#include "LED_Buffer.h"

#define MAX_POSSIBLE_BRIGHTNESS 250
#define MIN_POSSIBLE_BRIGHTNESS 100
#define MIN_POSSIBLE_OFFTIME 20
#define MAX_POSSIBLE_OFFTIME 100
class Twinkle_LED {
  public:
    uint8_t currBrightness;
    uint8_t maxBrightness;
    uint16_t timer;  
    bool brightnessIsIncreasing;
};

Twinkle_LED twinkleLEDs[NUM_LEDS]; 


void InitTwinkleMode() {
  uint16_t i;

  for(i = 0; i < NUM_LEDS; i++) {
    twinkleLEDs[i].currBrightness = 0;
    twinkleLEDs[i].maxBrightness = random(MIN_POSSIBLE_BRIGHTNESS, MAX_POSSIBLE_BRIGHTNESS);
    twinkleLEDs[i].timer = random(MIN_POSSIBLE_OFFTIME, MAX_POSSIBLE_OFFTIME);
    
  }
}
uint8_t randomHue = random(0, 255);
  
void StepTwinkle(LED_Buffer &led_buffer) {
  static uint8_t globalHue = 0;  // Tracks the current hue for all LEDs
  static uint8_t globalBrightness = 0;  // Controls the strobe effect (dim to bright)
  static bool brightnessIncreasing = true;  // Determines if brightness is increasing

  CRGB new_led_array[NUM_LEDS];
  const uint8_t hueDelta = 1;  // Increment for hue to cycle through colors
  const uint8_t brightnessDelta = 5;  // Increment for brightness strobe effect

  // Update global brightness
  if (brightnessIncreasing) {
    globalBrightness += brightnessDelta;
    if (globalBrightness >= 255) {  // Maximum brightness reached
      brightnessIncreasing = false;
    }
  } else {
    globalBrightness -= brightnessDelta;
    if (globalBrightness <= 0) {  // Minimum brightness reached
      brightnessIncreasing = true;
      globalHue += hueDelta;  // Change hue when brightness cycle completes
    }
  }

  // Set all LEDs with the same hue and brightness
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    new_led_array[i] = CHSV(globalHue, 200, globalBrightness);
  }

  led_buffer.updateBufferWithArray(new_led_array);
  led_buffer.SetAndIncrementUpdateIndex();
}
