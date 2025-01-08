#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LPS2X.h>

// Create an instance of the sensor
Adafruit_LPS22 lps;

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  while (!Serial) delay(10);  // Wait for serial port to be available

  // Initialize the I2C bus
  Wire.begin();

  // Initialize the sensor with the specific I2C address 0x5C
  if (!lps.begin_I2C(0x5C)) {
    Serial.println("Failed to find LPS22HBTR chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("LPS22HBTR Found!");

  // You can also adjust the sensor configuration here if needed
  lps.setDataRate(LPS22_RATE_ONE_SHOT);
}

void loop() {
  // Read pressure and temperature from the sensor
  sensors_event_t pressure_event;
  sensors_event_t temp_event;

  lps.getEvent(&pressure_event, &temp_event);

  // Print the readings to the Serial Monitor
  Serial.print("Pressure: ");
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.print("Temperature: ");
  Serial.print(temp_event.temperature);
  Serial.println(" °C");

  // Add a delay before taking the next reading
  delay(1000);
}
