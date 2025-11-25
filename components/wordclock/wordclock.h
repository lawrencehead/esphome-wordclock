#pragma once

#include "esphome/core/component.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/time/real_time_clock.h"
#include "esphome/components/select/select.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/number/number.h"
#include <NeoPixelBrightnessBus.h>

namespace esphome {
namespace wordclock {

const uint16_t PixelCount = 113;
const uint8_t PixelPin = 23;

class Wordclock : public Component {
 public:
  void setup() override;
  void loop() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

  void set_light(light::LightState *light) { light_ = light; }
  void set_time(time::RealTimeClock *time) { time_ = time; }
  void set_timemodus(select::Select *timemodus) { timemodus_ = timemodus; }
  void set_showdate(switch_::Switch *showdate) { showdate_ = showdate; }
  void set_colorcycle(switch_::Switch *colorcycle) { colorcycle_ = colorcycle; }
  void set_ccspeed(number::Number *ccspeed) { ccspeed_ = ccspeed; }

  void on_setled(int number, int red, int blue, int green, int white);

 protected:
  NeoPixelBrightnessBus<NeoGrbFeature, NeoWs2812xMethod> strip_{PixelCount, PixelPin};
  
  light::LightState *light_{nullptr};
  time::RealTimeClock *time_{nullptr};
  select::Select *timemodus_{nullptr};
  switch_::Switch *showdate_{nullptr};
  switch_::Switch *colorcycle_{nullptr};
  number::Number *ccspeed_{nullptr};

  // LED definitions
  int leds_time_it_is_[4] = {0, 1, 2, 3};
  int leds_minutes_[6] = {42, 43, 44, 45, 46, 47};
  
  int leds_time_minutes_[13][13] = {
    {103, 104, 105, 106, 107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1}, // 12 o'clock midnight? (0)
    { 49,  50,  51,  17,  18,  19,  20,  -1,  -1,  -1,  -1,  -1,  -1}, // five past
    { 49,  50,  51,  33,  34,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ten past
    { 49,  50,  51,   7,   8,   9,  10,  11,  12,  -1,  -1,  -1,  -1}, // quarter past
    { 49,  50,  51,  21,  22,  23,  24,  25,  -1,  -1,  -1,  -1,  -1}, // twenty past
    { 49,  50,  51,  21,  22,  23,  24,  25,  17,  18,  19,  20,  -1}, // twentyfive past
    { 49,  50,  51,  27,  28,  29,  30,  31,  -1,  -1,  -1,  -1,  -1}, // half past
    { 39,  40,  21,  22,  23,  24,  25,  17,  18,  19,  20,  -1,  -1}, // twentyfive to
    { 39,  40,  21,  22,  23,  24,  25,  -1,  -1,  -1,  -1,  -1,  -1}, // twenty to
    { 39,  40,   7,   8,   9,  10,  11,  12,  -1,  -1,  -1,  -1,  -1}, // quarter to
    { 39,  40,  33,  34,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ten to
    { 39,  40,  17,  18,  19,  20,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // five to
    {103, 104, 105, 106, 107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1} // 12 o'clock midday? (12)
  };

  int leds_time_hours_[13][6] = {
    { 59,  60,  61,  62,  63,  64}, // twelve (0 hrs)
    { 99, 100, 101,  -1,  -1,  -1}, // one
    { 78,  79,  -1,  -1,  -1,  -1}, // two
    { 94,  95,  96,  97,  98,  -1}, // three
    { 55,  56,  57,  58,  -1,  -1}, // four
    { 73,  74,  75,  76,  -1,  -1}, // five
    { 92,  93,  -1,  -1,  -1,  -1}, // six
    { 65,  66,  67,  68,  65,  -1}, // seven
    { 87,  88,  89,  90,  -1,  -1}, // eight
    { 70,  71,  72,  -1,  -1,  -1}, // nine
    {111, 112,  -1,  -1,  -1,  -1}, // ten
    { 81,  82,  83,  84,  85,  -1}, // eleven
    { 59,  60,  61,  62,  63,  64} // twelve (12 hrs)
  };

  int cijfer_links_[10][17] = {
    {110, 109, 108, 107, 106, 101,  95,  90,  84,  81,  75,  69,  68,  67,  66,  65,  -1},
    {101,  96,  90,  89,  88,  87,  86,  85,  84,  81,  -1,  -1,  -1,  -1,  -1,  -1,  -1},
    {110, 105, 101, 100,  95,  90,  86,  84,  81,  78,  75,  69,  68,  64,  -1,  -1,  -1},
    {111, 105, 101,  98,  95,  90,  87,  84,  81,  78,  75,  69,  68,  66,  65,  -1,  -1},
    {111, 110, 109, 108,  98,  87,  78,  70,  69,  68,  67,  66,  65,  64,  -1,  -1,  -1},
    {111, 110, 109, 108, 105, 101,  98,  95,  90,  87,  84,  81,  78,  75,  70,  66,  65},
    {110, 109, 108, 107, 106, 101,  98,  95,  90,  87,  84,  81,  78,  75,  66,  65,  -1},
    {111,  95,  90,  86,  85,  84,  78,  75,  70,  69,  68,  -1,  -1,  -1,  -1,  -1,  -1},
    {110, 109, 107, 106, 101,  98,  95,  90,  87,  84,  81,  78,  75,  69,  68,  66,  65},
    {110, 109, 106, 101,  98,  95,  90,  87,  84,  81,  78,  75,  69,  68,  67,  66,  65}
  };

  int cijfer_rechts_[10][17] = {
    { 49,  48,  47,  46,  45,  41,  35,  30,  24,  21,  15,  8,   7,    6,   5,   4,  -1},
    { 41,  36,  30,  29,  28,  27,  26,  25,  24,  21,  -1,  -1, -1,   -1,  -1,  -1,  -1},
    { 49,  45,  44,  41,  39,  35,  30,  27,  24,  21,  17,  15,  8,    3,  -1,  -1,  -1},
    { 50,  44,  41,  38,  35,  30,  27,  24,  21,  18,  15,   8,  7,    5,   4,  -1,  -1},
    { 50,  49,  48,  47,  38,  27,  18,   9,   8,   7,   6,   5,  4,    3,  -1,  -1,  -1},
    { 50,  49,  48,  47,  44,  41,  38,  35,  30,  27,  24,  21,  18,  15,   9,   5,   4},
    { 49,  48,  47,  46,  45,  41,  38,  35,  30,  27,  24,  21,  18,  15,   5,   4,  -1},
    { 50,  35,  30,  26,  25,  24,  18,  15,  9,    8,   7,  -1,  -1,  -1,  -1,  -1,  -1},
    { 49,  48,  46,  45,  41,  38,  35,  30,  27,  24,  21,  18,  15,   8,   7,   5,   4},
    { 49,  48,  45,  41,  38,  35,  30,  27,  24,  21,  18,  15,   8,   7,   6,   5,   4}
  };

  // State variables
  int hour_{-1};
  int minute_{-1};
  int second_{-1};
  int red_{124};
  int green_{124};
  int blue_{124};
  int white_{124};
  RgbColor color_;
  RgbColor color2_;
  int brightness_{50};
  int brightness2_;
  bool change_{false};
  bool show_date_{false};
  int hue_{0};
  int counter_{0};
  std::string tm_;

  // Fading variables
  static const int FADE_STEPS = 50;
  static const int FADE_DELAY = 10;
  bool fading_{false};
  int fade_step_{0};
  RgbColor old_colors_[PixelCount];
  RgbColor new_colors_[PixelCount];

  void start_fade();
  void apply_fade();
};

}  // namespace wordclock
}  // namespace esphome