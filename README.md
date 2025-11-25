# esphome-wordclock
## General
ESPhome wordclock based on [jeroen85's version](https://github.com/jeroen85/ha-wordclock-esphome), but updated to be used as an external component within ESPhome, instead of a custom component.

I've adapted it from being a custom component to an external component since the removal of custom components back in 2025.6.  It's now running just fine on 2025.11.0.  Claude.ai did most of the work in adapting it, and for the original code credit goes to [leinich](https://github.com/leinich/ha-wordclock-esphome) and [jeroen85](https://github.com/jeroen85/ha-wordclock-esphome).  Time is still pulled from Home Assistant.

I've also added a fade on/off for the LEDs as the time changes.

I used [this clock](https://www.amazon.co.uk/dp/B09VY54WBB?ref_=ppx_hzsearch_conn_dt_b_fed_asin_title_1&th=1) from Amazon, an ESP32 devkit and a ws2812 led strip.

![Image of wordclock](https://github.com/lawrencehead/esphome-wordclock/blob/main/wordclock.jpg?raw=true)
