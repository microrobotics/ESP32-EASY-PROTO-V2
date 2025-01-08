#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

// Define I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

#define HDC1000_ADDRESS 0x40  // I2C address for HDC1000 sensor
#define SD_CS_PIN 5           // SD card CS pin on GPIO 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_LPS22 lps22;
Adafruit_LSM6DS3TRC lsm6ds3trc;

const int button1Pin = 32;  // SW1
const int button2Pin = 33;  // SW2
const int irPin = 36;
const int potPin = 34;
const int ldrPin = 35;
const int redLEDPin = 15;   // Swapped with blue LED
const int blueLEDPin = 2;   // Swapped with red LED
const int rgbLEDPin = 4;
const int buzzerPin = 27;

Adafruit_NeoPixel rgbLED(1, rgbLEDPin, NEO_GRB + NEO_KHZ800);

int currentScreen = 0;
unsigned long pulseLow, pulseHigh;
unsigned long irHexValue;
int potValue, ldrValue;
bool buzzerActive = false;
float temp = 0;
float humidity = 0;
float pressure = 0;
float altitude = 0;

// Adjustable logging interval (1 second default)
unsigned long loggingInterval = 1000;
unsigned long previousMillis = 0;

// SD Card status variable
bool sdCardAvailable = false;
String sdCardErrorMessage = "";

// Known sea level pressure for Centurion, South Africa in hPa
const float knownSeaLevelPressure = 1026.0;

// IMU sensor data
sensors_event_t accelEvent;
sensors_event_t gyroEvent;
sensors_event_t tempEvent; // Added to hold temperature data from IMU

// Mutexes for shared resources
SemaphoreHandle_t xSensorDataMutex;
SemaphoreHandle_t xDataMutex;

// Button debouncing variables
int button1State = HIGH;         // the current stable state of button1
int lastButton1Reading = HIGH;   // the previous reading from the button1 pin
unsigned long button1LastDebounceTime = 0; // the last time the button1 input pin was toggled

int button2State = HIGH;         // the current stable state of button2
int lastButton2Reading = HIGH;   // the previous reading from the button2 pin
unsigned long button2LastDebounceTime = 0; // the last time the button2 input pin was toggled

const unsigned long debounceDelay = 50; // the debounce time in milliseconds

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for serial port to be available

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(irPin, INPUT);
  pinMode(potPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize I2C communication
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize display
  Serial.println("Initializing display...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Show splash screen
  displaySplashScreen();

  // Wait for user to press SW1 for IMU calibration
  waitForIMUCalibration();

  // Initialize the LSM6DS3TR-C IMU
  Serial.println("Initializing LSM6DS3TR-C IMU...");
  if (!lsm6ds3trc.begin_I2C()) {
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
  Serial.println("LSM6DS3TR-C Found!");

  // Initialize the LPS22HB pressure sensor
  Serial.println("Initializing LPS22HB sensor...");
  if (!lps22.begin_I2C(0x5C)) {  // Explicitly use address 0x5C
    Serial.println("Failed to find LPS22HB sensor at 0x5C");
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
  Serial.println("LPS22HB sensor initialized.");

  // Initialize RGB LED
  rgbLED.begin();
  rgbLED.show();

  // Print free heap memory
  Serial.print("Free heap memory before mutex creation: ");
  Serial.println(ESP.getFreeHeap());

  // Create mutex for sensor data
  xSensorDataMutex = xSemaphoreCreateMutex();
  if (xSensorDataMutex == NULL) {
    Serial.println("Failed to create sensor data mutex");
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  // Create mutex for shared data
  xDataMutex = xSemaphoreCreateMutex();
  if (xDataMutex == NULL) {
    Serial.println("Failed to create data mutex");
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  Serial.print("Free heap memory after mutex creation: ");
  Serial.println(ESP.getFreeHeap());

  // Initialize SD card (Optional: Comment out if not using SD card)
  /*
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    sdCardAvailable = false;
    sdCardErrorMessage = "SD Init Failed";
  } else {
    Serial.println("SD card initialized.");
    sdCardAvailable = true;
    sdCardErrorMessage = "";
  }
  */

  // Create FreeRTOS tasks with sufficient stack sizes
  xTaskCreate(sensorTask, "Sensor Task", 4096, NULL, 1, NULL);
  xTaskCreate(displayTask, "Display Task", 4096, NULL, 1, NULL);

  // Only create logTask if SD card is available (Optional)
  /*
  if (sdCardAvailable) {
    xTaskCreate(logTask, "Log Task", 4096, NULL, 1, NULL);
  }
  */
}

void loop() {
  // Main loop is not used in this FreeRTOS implementation
}

void displaySplashScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.setTextWrap(false);
  display.println("Welcome!");
  display.println("");
  display.println("Place the board on a");
  display.println("stable, stationary,");
  display.println("and level surface.");
  display.println("");
  display.println("Press SW1 to calibrate");
  display.println("the IMU.");
  display.display();
}

void waitForIMUCalibration() {
  // Wait for SW1 to be pressed
  while (digitalRead(button1Pin) == HIGH) {
    vTaskDelay(pdMS_TO_TICKS(100));  // Small debounce delay
  }
  vTaskDelay(pdMS_TO_TICKS(200)); // Additional delay to ensure button release

  // Start IMU calibration once the button is pressed
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextWrap(false);
  display.println("Calibrating IMU...");
  display.display();

  calibrateIMU();
}

void calibrateIMU() {
  // Calibration code can be added here if necessary
  vTaskDelay(pdMS_TO_TICKS(2000));  // Simulate calibration delay
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextWrap(false);
  display.println("Calibration complete!");
  display.display();
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void sensorTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    unsigned long currentMillis = millis();

    // Read the current state of button1
    int reading1 = digitalRead(button1Pin);

    // If the switch changed, due to noise or pressing:
    if (reading1 != lastButton1Reading) {
      // reset the debouncing timer
      button1LastDebounceTime = currentMillis;
    }

    if ((currentMillis - button1LastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer than the debounce delay
      // so take it as the actual current state

      // if the button state has changed:
      if (reading1 != button1State) {
        button1State = reading1;

        // only increment screen if the new button state is LOW (button pressed)
        if (button1State == LOW) {
          currentScreen++;
          if (currentScreen > 11) currentScreen = 0;
        }
      }
    }

    lastButton1Reading = reading1;

    // Similar code for button2
    int reading2 = digitalRead(button2Pin);

    if (reading2 != lastButton2Reading) {
      button2LastDebounceTime = currentMillis;
    }

    if ((currentMillis - button2LastDebounceTime) > debounceDelay) {
      if (reading2 != button2State) {
        button2State = reading2;

        if (button2State == LOW) {
          currentScreen--;
          if (currentScreen < 0) currentScreen = 11;
        }
      }
    }

    lastButton2Reading = reading2;

    // Read IMU data
    if (xSemaphoreTake(xSensorDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      if (!lsm6ds3trc.getEvent(&accelEvent, &gyroEvent, &tempEvent)) {
        Serial.println("Failed to read IMU data");
      }
      xSemaphoreGive(xSensorDataMutex);
    } else {
      Serial.println("Failed to take sensor data mutex in sensorTask");
    }

    // Read other sensors based on the current screen
    switch (currentScreen) {
      case 0:
        readIRSensor();
        break;
      case 3:
        readTempHumidity();
        break;
      case 4:
        readPressureSensor();
        break;
      case 5:
        potValue = averageAnalogRead(potPin, 20);
        break;
      case 6:
        ldrValue = analogRead(ldrPin);
        break;
      case 9:
        // Read potentiometer value for RGB LED control
        potValue = averageAnalogRead(potPin, 5);
        break;
      // Cases 7-11 are for LED, buzzer, and SD card status
    }

    vTaskDelay(pdMS_TO_TICKS(10)); // Adjust the delay as needed
  }
}

void readIRSensor() {
  if (digitalRead(irPin) == LOW) {
    pulseLow = pulseIn(irPin, LOW, 100000);   // Timeout of 100 ms
    pulseHigh = pulseIn(irPin, HIGH, 100000); // Timeout of 100 ms
    irHexValue = (pulseHigh << 16) | pulseLow;
  }
}

void readTempHumidity() {
  if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    temp = readTemperature();
    humidity = readHumidity();
    xSemaphoreGive(xDataMutex);
  } else {
    Serial.println("Failed to take data mutex in readTempHumidity");
  }
}

void readPressureSensor() {
  if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    sensors_event_t pressure_event;
    lps22.getPressureSensor()->getEvent(&pressure_event);
    pressure = pressure_event.pressure;
    altitude = calculateAltitude(pressure, knownSeaLevelPressure);
    xSemaphoreGive(xDataMutex);
  } else {
    Serial.println("Failed to take data mutex in readPressureSensor");
  }
}

void displayTask(void *pvParameters) {
  (void)pvParameters;

  for (;;) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextWrap(false);

    // Turn off RGB LED before switching screens unless on screen 9
    if (currentScreen != 9) {
      rgbLED.setPixelColor(0, rgbLED.Color(0, 0, 0));
      rgbLED.show();
    }

    // Turn off LEDs before switching screens
    digitalWrite(redLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);

    // Turn off buzzer before switching screens
    controlBuzzer(false);

    switch (currentScreen) {
      case 0:
        displayIRData();
        break;
      case 1:
        displayAccelData();
        break;
      case 2:
        displayGyroData();
        break;
      case 3:
        displayTempHumidity();
        break;
      case 4:
        displayPressureData();
        break;
      case 5:
        displayPotData();
        break;
      case 6:
        displayLDRData();
        break;
      case 7:
        display.println("Red LED ON");
        digitalWrite(redLEDPin, HIGH);
        break;
      case 8:
        display.println("Blue LED ON");
        digitalWrite(blueLEDPin, HIGH);
        break;
      case 9:
        displayRGBControl();
        break;
      case 10:
        display.println("Buzzer ON");
        controlBuzzer(true);
        break;
      case 11:
        displaySDCardStatus();
        break;
    }

    display.display();
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay for task
  }
}

void displayIRData() {
  display.setCursor(0, 0);
  display.println("IR Receiver Data");
  display.println("");
  display.print("Pulse Low: ");
  display.println(pulseLow);
  display.print("Pulse High: ");
  display.println(pulseHigh);
  display.print("Hex Value: 0x");
  display.println(irHexValue, HEX);
}

void displayAccelData() {
  if (xSemaphoreTake(xSensorDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    display.setCursor(0, 0);
    display.println("Accelerometer Data");
    display.println("");
    display.print("X: ");
    display.print(accelEvent.acceleration.x / 9.81, 2); // Convert m/s² to g
    display.print(" g");
    display.println("");
    display.print("Y: ");
    display.print(accelEvent.acceleration.y / 9.81, 2); // Convert m/s² to g
    display.print(" g");
    display.println("");
    display.print("Z: ");
    display.print(accelEvent.acceleration.z / 9.81, 2); // Convert m/s² to g
    display.print(" g");
    xSemaphoreGive(xSensorDataMutex);
  } else {
    display.println("Accelerometer Data");
    display.println("Unavailable");
  }
}

void displayGyroData() {
  if (xSemaphoreTake(xSensorDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    display.setCursor(0, 0);
    display.println("Gyroscope Data");
    display.println("");
    display.print("X: ");
    display.print(gyroEvent.gyro.x, 2);
    display.print(" rad/s");
    display.println("");
    display.print("Y: ");
    display.print(gyroEvent.gyro.y, 2);
    display.print(" rad/s");
    display.println("");
    display.print("Z: ");
    display.print(gyroEvent.gyro.z, 2);
    display.print(" rad/s");
    xSemaphoreGive(xSensorDataMutex);
  } else {
    display.println("Gyroscope Data");
    display.println("Unavailable");
  }
}

void displayTempHumidity() {
  if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    display.setCursor(0, 0);
    display.println("Temperature & Humidity");
    display.println("");
    display.print("Temp: ");
    display.print(temp, 1);
    display.println(" C");
    display.print("Humidity: ");
    display.print(humidity, 1);
    display.println(" %");
    xSemaphoreGive(xDataMutex);
  } else {
    display.println("Temperature & Humidity");
    display.println("Unavailable");
  }
}

void displayPressureData() {
  if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
    display.setCursor(0, 0);
    display.println("Pressure & Altitude");
    display.println("");
    display.print("Pressure: ");
    display.print(pressure, 1);
    display.println(" hPa");
    display.print("Altitude: ");
    display.print(altitude, 1);
    display.println(" m");
    xSemaphoreGive(xDataMutex);
  } else {
    display.println("Pressure & Altitude");
    display.println("Unavailable");
  }
}

void displayPotData() {
  display.setCursor(0, 0);
  display.println("Potentiometer");
  display.println("");
  display.print("Value: ");
  display.println(potValue);
}

void displayLDRData() {
  display.setCursor(0, 0);
  display.println("LDR Data");
  display.println("");
  display.print("Value: ");
  display.println(ldrValue);
}

void displayRGBControl() {
  display.setCursor(0, 0);
  display.println("RGB LED Control");
  display.println("");
  display.println("Turn pot to change");
  display.println("RGB LED color");

  // Map potentiometer value to RGB color
  uint16_t potValue = averageAnalogRead(potPin, 5);
  uint16_t hue = map(potValue, 0, 4095, 0, 65535);
  uint32_t color = rgbLED.gamma32(rgbLED.ColorHSV(hue));

  // Update RGB LED color
  rgbLED.setPixelColor(0, color);
  rgbLED.show();
}

void displaySDCardStatus() {
  display.setCursor(0, 0);
  display.println("SD Card Status:");
  display.println("");
  if (sdCardAvailable) {
    display.println("SD Card OK");
  } else {
    display.println("SD Card Error:");
    display.println(sdCardErrorMessage);
  }
}

void controlBuzzer(bool state) {
  if (state && !buzzerActive) {
    tone(buzzerPin, 1000); // Play a 1000 Hz tone
    buzzerActive = true;
  } else if (!state && buzzerActive) {
    noTone(buzzerPin); // Stop the tone
    buzzerActive = false;
  }
}

float readTemperature() {
  Wire.beginTransmission(HDC1000_ADDRESS);
  Wire.write(0x00); // Temperature register
  Wire.endTransmission();
  vTaskDelay(pdMS_TO_TICKS(15)); // Wait for conversion
  Wire.requestFrom(HDC1000_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint16_t rawTemp = (Wire.read() << 8) | Wire.read();
    return ((rawTemp / 65536.0) * 165.0) - 40.0;
  }
  return NAN; // Return NaN if no data is available
}

float readHumidity() {
  Wire.beginTransmission(HDC1000_ADDRESS);
  Wire.write(0x01); // Humidity register
  Wire.endTransmission();
  vTaskDelay(pdMS_TO_TICKS(15)); // Wait for conversion
  Wire.requestFrom(HDC1000_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint16_t rawHum = (Wire.read() << 8) | Wire.read();
    return (rawHum / 65536.0) * 100.0;
  }
  return NAN; // Return NaN if no data is available
}

float calculateAltitude(float pressure, float seaLevelPressure) {
  return 44330.0 * (1.0 - pow(pressure / seaLevelPressure, 0.1903));
}

int averageAnalogRead(int pin, int samples) {
  int sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    vTaskDelay(pdMS_TO_TICKS(5)); // Small delay between readings
  }
  return sum / samples;
}

// Optional: Implement logTask if using SD card
/*
void logTask(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    if (sdCardAvailable) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= loggingInterval) {
        previousMillis = currentMillis;

        // Open the file for appending
        File dataFile = SD.open("/data.txt", FILE_APPEND);
        if (dataFile) {
          // Prepare data to log
          String dataString = "";
          dataString += "Time: ";
          dataString += currentMillis / 1000;
          dataString += " s, ";

          // Add sensor data
          dataString += "Temp: ";
          dataString += temp;
          dataString += " C, Humidity: ";
          dataString += humidity;
          dataString += " %, Pressure: ";
          dataString += pressure;
          dataString += " hPa, Altitude: ";
          dataString += altitude;
          dataString += " m";

          // Write data to file
          if (dataFile.println(dataString)) {
            dataFile.close();
          } else {
            // Write failed, possibly SD card full
            Serial.println("Failed to write to SD card");
            sdCardAvailable = false;
            sdCardErrorMessage = "SD Write Failed";
            dataFile.close();
          }
        } else {
          // Failed to open file
          Serial.println("Failed to open file on SD card");
          sdCardAvailable = false;
          sdCardErrorMessage = "SD Open Failed";
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Small delay
  }
}
*/
