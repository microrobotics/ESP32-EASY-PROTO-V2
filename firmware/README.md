# firmware

**Firmware** projects and example sketches for the ESP32-EASY-PROTO-V2. **Proprietary**—see [../LICENSE.md](../LICENSE.md).

## Subfolders

- **examples/**  
  - Contains sample code demonstrating sensor usage, SD card read/write, buzzer control, etc.
- **platformio/**  
  - If you store a PlatformIO project with `platformio.ini`.
- **arduino/**  
  - Arduino `.ino` sketches or library references.
- **esp-idf/**  
  - ESP-IDF-based CMake or Make projects.

## General Tips

1. For **Arduino IDE**:  
   - Copy example sketches from `examples/` into your Arduino sketch folder.

2. For **PlatformIO**:  
   - Open `platformio.ini` in VS Code or your preferred environment.  
   - Build and upload using the designated environment (esp32dev or similar).

3. For **ESP-IDF**:  
   - Place main code in `esp-idf/main/`.  
   - Use `idf.py build` and `idf.py flash`.

