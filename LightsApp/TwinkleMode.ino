#include "LED_Buffer.h"

// --- Configuration ---
#define NUM_LEDS 200       // Your specific count
#define FADE_SPEED 3       // How fast lights breathe (Lower is slower/smoother)
#define SANTA_INTERVAL 15000 // How often Santa flies by (ms)

// --- Helper Class for Individual Pixels ---
class Festive_LED {
  public:
    float currentBrightness;
    float step;           // How fast this specific pixel changes
    bool increasing;      // Is it getting brighter or dimmer?
    uint8_t hue;          // Red or Green
    
    void setup(uint8_t myIndex) {
      // Alternate Red (0) and Green (96) based on position
      // We use slight variations so it doesn't look too uniform
      if (myIndex % 2 == 0) {
        hue = 0; // Red
      } else {
        hue = 96; // Green
      }

      // Randomize start so the whole house doesn't pulse in sync
      currentBrightness = random(20, 200);
      increasing = random(0, 2) == 1;
      
      // Give each pixel a unique "breathing" speed
      step = random(10, 40) / 10.0; 
    }

    CRGB getColor() {
      // Logic: Update brightness
      if (increasing) {
        currentBrightness += step;
        if (currentBrightness >= 255) {
          currentBrightness = 255;
          increasing = false;
        }
      } else {
        currentBrightness -= step;
        if (currentBrightness <= 40) { // Don't turn completely off, keeps it warm
          currentBrightness = 40;
          increasing = true;
        }
      }

      // SPARKLE LOGIC: 
      // If brightness is very high (>240), reduce saturation to make it look 
      // like a white/gold sparkle instead of pure red/green.
      uint8_t saturation = 255;
      if (currentBrightness > 240) {
        saturation = map(currentBrightness, 240, 255, 255, 100);
      }

      return CHSV(hue, saturation, (uint8_t)currentBrightness);
    }
};

// Create the array of pixel objects
Festive_LED festiveLEDs[NUM_LEDS];
bool isInitialized = false;

// Call this once to setup the alternating colors
void InitTwinkleMode() {
  for(uint16_t i = 0; i < NUM_LEDS; i++) {
    festiveLEDs[i].setup(i);
  }
  isInitialized = true;
}

void StepTwinkle(LED_Buffer &led_buffer) {
  // Ensure we initialized our custom objects
  if (!isInitialized) InitTwinkleMode();

  CRGB new_led_array[NUM_LEDS];
  
  // 1. Render the Background (Breathing Red/Green)
  for (int i = 0; i < NUM_LEDS; i++) {
    new_led_array[i] = festiveLEDs[i].getColor();
  }

  // --- SANTA SLEIGH LOGIC (Previously Ghost Streak) ---
  static uint32_t lastStreakTime = 0;
  static int streakPosition = -1; // -1 means Santa is at the North Pole (inactive)
  uint32_t now = millis();

  // Trigger Santa every 15 seconds
  if (streakPosition == -1 && (now - lastStreakTime >= SANTA_INTERVAL)) {
    streakPosition = 0; 
    lastStreakTime = now;
  }

  // Draw Santa overlay
  if (streakPosition >= 0) {
    // Length of the sleigh trail
    const int sleighLength = 8; 

    for (int i = 0; i < sleighLength; i++) {
      int pixelPos = streakPosition - i;
      
      // Only draw if within bounds of the LED strip
      if (pixelPos >= 0 && pixelPos < NUM_LEDS) {
        if (i == 0) {
          // RUDOLPH: The leading pixel is bright Red
          new_led_array[pixelPos] = CRGB(255, 0, 0); 
        } else if (i == 1 || i == 2) {
           // REINDEER GAP: Leave a slight gap or dim spot for contrast
           new_led_array[pixelPos] = CRGB(10, 10, 0);
        } else {
          // THE SLEIGH: Warm Gold/Yellow trail
          // Fade out the tail
          uint8_t brightness = 255 - (i * (255/sleighLength));
          new_led_array[pixelPos] = CHSV(45, 180, brightness); 
        }
      }
    }

    // Move Santa forward
    streakPosition++; // Speed of Santa is determined by how often StepTwinkle is called
    
    // Check if Santa has left the building
    if (streakPosition - sleighLength > NUM_LEDS) {
      streakPosition = -1; // Reset
    }
  }

  // Push to buffer
  led_buffer.updateBufferWithArray(new_led_array);
  led_buffer.SetAndIncrementUpdateIndex();
}