#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HDC1000.h>

// Create an instance of the sensor
Adafruit_HDC1000 hdc = Adafruit_HDC1000();

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  while (!Serial) delay(10);  // Wait for serial port to be available

  // Initialize the I2C bus
  Wire.begin();

  // Initialize the sensor
  if (!hdc.begin()) {
    Serial.println("Failed to find HDC1080DMBR chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("HDC1080DMBR Found!");
}

void loop() {
  // Read temperature and humidity from the sensor
  float temperature = hdc.readTemperature();
  float humidity = hdc.readHumidity();

  // Print the readings to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Add a delay before taking the next reading
  delay(2000);
}
