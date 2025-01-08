# firmware/examples

Sample firmware projects to demonstrate key features of the ESP32-EASY-PROTO-V2 board.

## Contents

- **sensor_demo/**  
  - Reads IMU (LSM6DS3), HDC1080, LPS22HB data over I2C, prints results to Serial.

- **sd_card_test/**  
  - Initializes SD card (GPIO5,18,19,23) and writes/reads a test file.

- **buzzer_test/**  
  - Generates tones or beep patterns on GPIO27 (driving the MOSFET + buzzer).

- **ws2812b_demo/**  
  - Uses Adafruit NeoPixel or FastLED to animate the WS2812B on GPIO4 (3.3→5 V shift).

## Usage

1. Open each folder in Arduino IDE, PlatformIO, or ESP-IDF.  
2. Edit the code or config as needed for your environment.  
3. Build and flash to see each peripheral in action.

