#include "TwinkleMode.h"

void TwinkleModeInit(void) {
    UI_16_t i;

    for(i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
}

void StepTwinkle(void) {
    UI_16_t i;

    for(i = 0; i < NUM_LEDS; i++) {

    }
} 