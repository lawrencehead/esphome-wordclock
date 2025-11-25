#include "wordclock.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wordclock {

static const char *const TAG = "wordclock";

void Wordclock::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Wordclock...");
  
  strip_.Begin();
  strip_.ClearTo(RgbColor(0, 0, 0));
  strip_.Show();

  // Start all LED with on and default color and brightness to check if everything is working
  for(int i = 0; i < PixelCount; i++) { 
    strip_.SetPixelColor(i, RgbColor(red_, 0, 0)); 
    strip_.Show(); 
    delay(10); 
  }
  for(int i = 0; i < PixelCount; i++) { 
    strip_.SetPixelColor(i, RgbColor(0, green_, 0)); 
    strip_.Show(); 
    delay(10); 
  }
  for(int i = 0; i < PixelCount; i++) { 
    strip_.SetPixelColor(i, RgbColor(0, 0, blue_)); 
    strip_.Show(); 
    delay(10); 
  }
  strip_.ClearTo(RgbColor(0, 0, 0));
  strip_.Show();
}

void Wordclock::on_setled(int number, int red, int blue, int green, int white) {
  if (number < PixelCount && number >= 0) {
    ESP_LOGD(TAG, "Setting led number %d to color %i %i %i %i", number, red, green, blue, white);
    strip_.SetPixelColor(number, RgbColor(red, green, blue));
    strip_.Show();
  } else {
    ESP_LOGE(TAG, "Not a valid LED Number - out of range");
  }
}

void Wordclock::start_fade() {
  fading_ = true;
  fade_step_ = 0;
  for (int i = 0; i < PixelCount; i++) {
    old_colors_[i] = strip_.GetPixelColor(i);
  }
}

void Wordclock::apply_fade() {
  if (!fading_) return;

  fade_step_++;
  float progress = static_cast<float>(fade_step_) / FADE_STEPS;

  for (int i = 0; i < PixelCount; i++) {
    RgbColor intermediate_color = RgbColor::LinearBlend(old_colors_[i], new_colors_[i], progress);
    strip_.SetPixelColor(i, intermediate_color);
  }

  strip_.Show();

  if (fade_step_ >= FADE_STEPS) {
    fading_ = false;
  } else {
    delay(FADE_DELAY);
  }
}

void Wordclock::loop() {
  if (time_ == nullptr || light_ == nullptr) {
    return;
  }

  auto time = time_->now();
  auto time_mode = timemodus_ ? timemodus_->current_option() : std::string("Minutes-plus");

  // Get light state
  float state_value = 0.0f;
  float brightness_value = 0.0f;
  float red_value = 0.0f;
  float green_value = 0.0f;
  float blue_value = 0.0f;
  float white_value = 0.0f;

  if (light_) {
    auto call = light_->current_values;
    state_value = call.get_state();
    brightness_value = call.get_brightness();
    red_value = call.get_red();
    green_value = call.get_green();
    blue_value = call.get_blue();
    white_value = call.get_white();
  }

  int h = time.hour;
  int m = time.minute;
  int s = time.second;

  // Convert float 0.0 till 1.0 into int 0 till 255
  red_ = (int)(red_value * 255);
  green_ = (int)(green_value * 255);
  blue_ = (int)(blue_value * 255);
  white_ = (int)(white_value * 255);

  if (colorcycle_ && colorcycle_->state) {
    color_ = HsbColor((float)hue_ / 255, 1.0, 1.0);
    float speed = ccspeed_ ? ccspeed_->state : 1.0f;
    if (counter_ >= (int)(100 / speed)) {
      hue_++;
      counter_ = 0;
      change_ = true;
    }
    if (hue_ == 256) {
      hue_ = 0;
    }
    counter_++;
  } else {
    color2_ = RgbColor(red_, green_, blue_);
    if (color_.R != color2_.R || color_.G != color2_.G || color_.B != color2_.B) {
      change_ = true;
      color_ = color2_;
      ESP_LOGD(TAG, "Color change rgb %i %i %i", red_, green_, blue_);
    }
  }

  // Check if light is on and set brightness
  if (state_value > 0) {
    brightness2_ = (int)(brightness_value * 255);

    if (brightness_ != brightness2_) {
      change_ = true;
      brightness_ = brightness2_;
    }
    if (brightness_ == 0) {
      change_ = true;
      brightness_ = 1;
    }
  } else {
    brightness_ = 0;
    change_ = true;
  }

  strip_.SetBrightness(brightness_);

  // Check if valid time
  if (!time.is_valid()) {
    ESP_LOGE(TAG, "Got invalid time from current_time Time: %i:%i", h, m);
    strip_.SetPixelColor(0, RgbColor(255, 0, 0)); strip_.Show(); delay(250);
    strip_.SetPixelColor(0, RgbColor(0, 255, 0)); strip_.Show(); delay(250);
    strip_.SetPixelColor(0, RgbColor(0, 0, 255)); strip_.Show(); delay(250);
    strip_.SetPixelColor(0, RgbColor(0, 0, 0));   strip_.Show();
  } else {
    if (h != hour_ || m != minute_ || change_ || time_mode != tm_) {
      hour_ = h;
      minute_ = m;
      second_ = s;
      change_ = false;
      tm_ = time_mode;
      if (hour_ >= 0) {
        start_fade();
        for (int i = 0; i < PixelCount; i++) {
          new_colors_[i] = RgbColor(0, 0, 0);
        }
        int tmp_hour;
        int tmp_minute = 0;

        if ((time_mode == "Minutes-min" && minute_ <= 30) || 
            (time_mode == "Minutes-plus" && minute_ <= 34)) {
          tmp_hour = hour_ % 12;
        } else {
          tmp_hour = (hour_ + 1) % 12;
        }

        if (time_mode == "Minutes-plus") {
          tmp_minute = (minute_ - (minute_ % 5)) / 5;
        } else if (time_mode == "Minutes-min") {
          tmp_minute = (minute_ + 4 - ((minute_ + 4) % 5)) / 5;
        }

        for (int i = 0; i < 4; i++) {
          new_colors_[leds_time_it_is_[i]] = color_;
        }
        for (int i = 0; i < 13; i++) {
          if (leds_time_minutes_[tmp_minute][i] >= 0) {
            new_colors_[leds_time_minutes_[tmp_minute][i]] = color_;
          }
        }
        for (int i = 0; i < 6; i++) {
          if (leds_time_hours_[tmp_hour][i] >= 0) {
            new_colors_[leds_time_hours_[tmp_hour][i]] = color_;
          }
        }

        if (tmp_minute != 0 && tmp_minute != 12) {
          for (int i = 0; i < 6; i++) {
            new_colors_[leds_minutes_[i]] = color_;
          }
        }
      }
    }
  }

  apply_fade();
}

}  // namespace wordclock
}  // namespace esphome