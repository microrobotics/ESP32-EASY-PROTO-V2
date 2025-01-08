#include <FastLED.h>

// Define the number of LEDs
#define NUM_LEDS 1  // Change this to the number of LEDs you have

// Define the pin connected to the data line of the WS2812B LED
#define DATA_PIN 4

// Create a CRGB array to hold the LED data
CRGB leds[NUM_LEDS];

void setup() {
  // Initialize the FastLED library
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  // Set the LED to red
  leds[0] = CRGB::Red;
  FastLED.show();
  delay(1000);

  // Set the LED to green
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(1000);

  // Set the LED to blue
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(1000);

  // Turn off the LED
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(1000);
}
