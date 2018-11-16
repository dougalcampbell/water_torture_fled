/**
 * Water_Torture.cpp
 *
 * Adapted for FastLED from Danny Havenith's code:
 *   https://github.com/DannyHavenith/ws2811
 *
 * @author Dougal Campbell <dougal@gunters.org>
 * @link https://github.com/dougalcampbell
 * @license BSL 1.0
 */

#include "Water_Torture.h"

/**
 * Constructor
 *
 * Sets a few sane defaults, but gives you complete freedom to
 * override them, of course. Dont forget to call setFastled()
 * and animate().
 */
Water_Torture::Water_Torture()
{
    state = WT_SWELLING;
    color = CRGB::Blue;
    position = 0;
    speed = 0;
    droplet_pause = 10;
}

/**
 * Deconstructor
 */
Water_Torture::~Water_Torture()
{
    // Go dark on destruction
    color = CRGB::Black;
    fastled->clearLedData();
    fastled->showLeds();
}

/**
 * State machine. Wait for the nextEvent time, and change states.
 *
 * WT_WAITING -> WT_SWELLING
 *      ^              |
 *      |              v
 * WT_BOUNCING <- WT_FALLING
 *
 */
void Water_Torture::step() {

  if ( state == WT_FALLING || state == WT_BOUNCING ) {
    position += speed;
    speed += gravity;

    // If we hit the bottom...
    const uint16_t maxpos16 = maxpos << 8;
    if (position > maxpos16) {
      if ( state == WT_BOUNCING ) {
        // This is the second collision, deactivate.
        state = WT_WAITING;
      } else {
        // reverse direction and dampen the speed
        position = maxpos16 - (position - maxpos16);
        speed = -speed/3;
        color = scale(color, collision_scaling);
        state = WT_BOUNCING;
      }
    }
    
  } else if ( state == WT_SWELLING ) {
    ++position;
    if (color.getLuma() <= 10 || color.getLuma() - position/2 <= 10) {
      state = WT_FALLING;
      position = 0;
    }  
  }

    //leds[position] = color;
}

// perform one step and draw
void Water_Torture::step(CRGB *arr) {
  step();
  draw( arr );
}

void Water_Torture::draw(CRGB *arr) {
  
  if (state == WT_FALLING || state == WT_BOUNCING) {
    uint8_t position8 = position >> 8;
    uint8_t remainder = position; // get lower bits

    add_clipped_to(&leds[position8], scale(color, 256 - remainder));
    if (remainder) {
      add_clipped_to(&leds[position8+1], scale(color, remainder));
    }

    if (state == WT_BOUNCING) {
      add_clipped_to(&leds[maxpos], color);
    }
  } else if (state == WT_SWELLING) {
    add_clipped_to( &leds[0], scale(color, position));
  }
}

bool Water_Torture::is_active() const {
  return (state != WT_WAITING);
}

static uint8_t Water_Torture::add_clipped(uint16_t left, uint16_t right) {
  uint16_t result = left + right;
  if (result > 255) result = 255;
  return result;
}

static void Water_Torture::add_clipped_to(CRGB *left, CRGB right) {
  left->red = add_clipped(left->red, right.red);
  left->green = add_clipped(left->green, right.green);
  left->blue = add_clipped(left->blue, right.blue);
}

static uint8_t Water_Torture::mult( uint8_t value, uint16_t multiplier) {
  return (static_cast<uint16_t>( value) * multiplier) >> 8;  
}

static CRGB Water_Torture::scale(CRGB value, uint16_t amplitude) {
  CRGB newcolor;
  newcolor.red = mult(value.red, amplitude);
  newcolor.green = mult(value.green, amplitude);
  newcolor.blue = mult(value.blue, amplitude);

  return newcolor;
}

void Water_Torture::animate() {
  if (droplet_pause) {
    --droplet_pause;
  } else {
    if (!is_active()) {
      // Create a new droplet

      // Reset to original base color.
      color = base_color;
      droplet_pause = 200 + random16() % 500;
      state = (random8() < 96) ? WT_SWELLING : WT_FALLING;
      // reset positon & speed for new droplet
      position = 0;
      speed = 0;
    }
  }


  fastled->clearLedData();
  step(leds);
  fastled->showLeds();
  FastLED.delay(1);
}

/**
 * Getters
 */
uint32_t Water_Torture::getColor() {
     return color;
}

uint8_t Water_Torture::getState() {
    return state;
}

int16_t Water_Torture::getSpeed() {
    return speed;
}

uint16_t Water_Torture::getGravity() {
    return gravity;
}

/**
 * Setters
 */
void Water_Torture::setColor(CRGB newColor) {
    base_color = newColor;
    color = base_color;
}

void Water_Torture::setState(States newState) {
    state = newState;
}

void Water_Torture::setSpeed(int16_t newspeed) {
  speed = newspeed;
}

void Water_Torture::setGravity(uint16_t newgravity) {
  gravity = newgravity;
}

void Water_Torture::setFastled(CLEDController* fled) {
  fastled = fled;
  leds = fastled->leds();
  NumLeds = fastled->size();
  maxpos = NumLeds - 1;
  droplet_pause = 1;
  position = 0;
  speed = 0;
}

void Water_Torture::setLeds(CRGB* arr) {
  leds = arr;
}

/*
// 
void Water_Torture::test() {
  fastled->clearLedData();
  for (int i = 0; i < NumLeds; i++) {
    leds[i] = CHSV(i * 5, 255, 255);
  }
  fastled->showLeds();
  FastLED.delay(1000);
  fastled->clearLedData();

}
*/

// - fin -
