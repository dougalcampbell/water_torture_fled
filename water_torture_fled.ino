/**
 * water_torture.ino
 *
 * Adapted for FastLED from Danny Havenith's code:
 *   https://github.com/DannyHavenith/ws2811
 *
 * @author Dougal Campbell <dougal@gunters.org>
 * @link https://github.com/dougalcampbell
 * @license BSL 1.0
 */

#include <FastLED.h>

// Data Out pin
#define DATA_PIN 3

// How many pixels are we driving? NOTE: Max 255 for Water_Torture
#define NUM_LEDS 60

CRGB leds[NUM_LEDS];

CLEDController *fled_ptr; // Pointer to FastLED instance

#include "water_torture.h"

Water_Torture droplet = Water_Torture();
// add a second droplet
Water_Torture droplet2 = Water_Torture();

void setup() {
    fled_ptr = &(FastLED.addLeds<WS2811,DATA_PIN,GRB>(leds, NUM_LEDS));
    FastLED.setBrightness(255);
    droplet.setFastled(fled_ptr);

    // Configure 2nd droplet
    droplet2.setFastled(fled_ptr);
    // Configurable color!
    droplet2.setColor(CRGB::Orange);
    // Configurable gravity!
    droplet2.setGravity(4);
    // Set the initial pause (milliseconds)
    droplet2.droplet_pause = 750;
    // Start from the far end of the strip, in case your
    // setup runs from bottom to top:
    droplet2.setReverse(true);

    FastLED.clear();
}

void loop() {
  droplet.animate();
  droplet2.animate();
}

// - fin -
