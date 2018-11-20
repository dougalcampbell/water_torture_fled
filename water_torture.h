#include <FastLED.h>

/**
 * Water_Torture.h
 * 
 * Adapted for FastLED from Danny Havenith's code:
 *   https://github.com/DannyHavenith/ws2811
 *
 * @author Dougal Campbell <dougal@gunters.org>
 * @link https://github.com/dougalcampbell
 * @license BSL 1.0
 */

#define WATERTORTURE_VERSION "1.2.0"
/**
 * The various states for our state machnine, which is handled in our `step()`
 * and `draw()` methods. See the diagram in `Water_Torture.cpp`.
 */
enum States {
  WT_WAITING,       // Dark. Waiting to appear...
  WT_SWELLING,      // Building the droplet...
  WT_FALLING,       // Drop falling down.
  WT_BOUNCING       // Splash!
};

class Water_Torture {
  public:
    // constructor
    Water_Torture(CRGB*, uint16_t);
    Water_Torture(CLEDController*, uint16_t);
    Water_Torture(void);
    ~Water_Torture(void);

    // getters/setters
    uint32_t getColor();
    uint8_t getState();
    int16_t getSpeed();
    int16_t getGravity();
    bool getReverse();

    void setColor(CRGB);
    void setState(States);
    void setSpeed(int16_t);
    void setGravity(int16_t);
    void setReverse(bool);
    void setFastled(CLEDController*);
    void setLeds(CRGB*);

    // primary work functions
    void animate(void);
    void step(void);
    void step(CRGB*);
    void draw(CRGB*);
    bool is_active(void) const;
    uint16_t droplet_pause;

  private:
    static uint8_t add_clipped(uint16_t, uint16_t);
    static void add_clipped_to(CRGB*, CRGB);
    static uint8_t mult( uint8_t, uint16_t);
    static CRGB scale(CRGB, uint16_t);
    
    States state = WT_SWELLING;
    static const uint16_t collision_scaling = 40;

    uint16_t position = 0;
    int16_t speed = 0;
    int16_t gravity = 8;
    uint16_t NumLeds;
    uint8_t maxpos;
    bool reverse_dir = false;
    
    CRGB base_color = CRGB::Blue;
    CRGB color = CRGB::Blue;
    CRGB *leds;
    CLEDController *fastled;
};

// - fin -
