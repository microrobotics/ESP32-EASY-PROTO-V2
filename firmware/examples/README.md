# ESP32-EASY-PROTO-V2 - Firmware Examples

This folder contains **sample firmware projects** demonstrating key features of the **ESP32-EASY-PROTO-V2** board, written by Lucienne Swart. Each subfolder corresponds to a specific peripheral, sensor, or combined demo.

---

## Folder Overview

- **ESP32-EASY-PROTO-V2_BUZZER**  
  Example code generating tones or beep patterns on GPIO27. This drives the 2N7002 MOSFET and buzzer (MLT8530) through a 10 kΩ gate resistor and 1N4001 diode.

- **ESP32-EASY-PROTO-V2_COMBINED_REV2**  
  A combined, more advanced sketch showcasing multiple peripherals or features together (e.g., reading IMU + HDC1080 + LPS22HB + controlling LEDs, etc.). Use this to see how different parts of the board can be integrated in one sketch.

- **ESP32-EASY-PROTO-V2_IMU**  
  Demonstrates how to interface with the LSM6DS3TR-C accelerometer/gyroscope sensor over I2C (GPIO21 = SDA, GPIO22 = SCL), printing motion data to Serial.

- **ESP32-EASY-PROTO-V2_IR_RECEIVER**  
  Sketch for reading IR remote signals on GPIO36 (with known silkscreen swap note). Uses typical IR libraries (e.g., IRremoteESP8266) to decode remote codes.

- **ESP32-EASY-PROTO-V2_LDR**  
  Reads the photoresistor (GT36516) on GPIO35 (ADC). Prints the light level to Serial or toggles an LED in response.

- **ESP32-EASY-PROTO-V2_LEDS**  
  Simple sketch toggling onboard LEDs (GPIO2 = Blue, GPIO15 = Red), plus additional debugging or blinking patterns.

- **ESP32-EASY-PROTO-V2_POT**  
  Reads the potentiometer (RK09D1130C3W) on GPIO34 (ADC). Prints or uses the analog value to drive PWM, adjust brightness, etc.

- **ESP32-EASY-PROTO-V2_PRESSURE_SENSOR**  
  Interfaces with the LPS22HBTR (I2C 0x5C). Reads pressure data, then prints it to Serial.

- **ESP32-EASY-PROTO-V2_PUSH_BUTTONS**  
  Demonstrates detecting push button presses on GPIO32 (SW1) and GPIO33 (SW2). Could be extended to handle interrupts or debouncing.

- **ESP32-EASY-PROTO-V2_RGB_LED**  
  Controls a WS2812B (Neopixel) on GPIO4 with level shifting by TXB0101 to 5 V. Uses either Adafruit NeoPixel or FastLED library to create lighting animations.

- **ESP32-EASY-PROTO-V2_TEMP_HUM_SENSOR**  
  Reads temperature and humidity from the HDC1080DMBR (I2C 0x40). Logs the data to Serial and can optionally toggle an LED if certain thresholds are reached.

---

## Usage

1. **Open Each Sketch**  
   - Using the **Arduino IDE**, simply copy the folder (e.g., `ESP32-EASY-PROTO-V2_BUZZER`) into your Arduino `sketches/` directory, open the `.ino` file, select the correct board (“ESP32 Dev Module” or similar), and upload.

   - For **PlatformIO** or **ESP-IDF**, you can migrate the `.ino` code into the respective structure, ensuring you add the correct libraries and setup.

2. **Edit Config as Needed**  
   - Adjust I2C pins if you changed hardware defaults (GPIO21 = SDA, GPIO22 = SCL).  
   - For IR sketches, confirm your IR library’s requirements.  
   - For WS2812B LED, confirm you have the Adafruit NeoPixel or FastLED library installed.

3. **Build and Flash**  
   - Once configured, build and upload. Open Serial Monitor to see sensor data or debug output.

---

## Combined Example vs. Individual Sketches

- The **combined** sketch (`ESP32-EASY-PROTO-V2_COMBINED_REV2`) merges multiple features (IMU, humidity, pressure, LED control, etc.) into one.  
- The **individual** sketches (e.g., `_IMU`, `_PRESSURE_SENSOR`, `_BUZZER`) help you **focus** on each peripheral or sensor in isolation.

---

## Additional Resources

- For **buzzer** hardware details, see the [buzzer_test README](../docs/…) or the main board documentation explaining how the 2N7002 MOSFET and 1N4001 diode are wired on GPIO27.  
- For the **WS2812B** library usage, refer to [Adafruit NeoPixel docs](https://github.com/adafruit/Adafruit_NeoPixel) or [FastLED docs](https://github.com/FastLED/FastLED).  
- For sensor libraries like HDC1080 or LPS22HB, see [Adafruit_LPS22](https://github.com/adafruit/Adafruit_LPS2X) or SparkFun equivalents.

---

**Enjoy exploring each peripheral on the ESP32-EASY-PROTO-V2 board!** If you have any questions or encounter issues, please contact **Micro Robotics** via [Your Support Email / Website] or the private issue tracker.
