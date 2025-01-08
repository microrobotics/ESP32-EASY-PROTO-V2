#include <Wire.h>

// Define the I2C address for the LSM6DS3TR-C
#define LSM6DS3TR_C_ADDRESS 0x6A

// Define register addresses
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // Initialize I2C with SDA on GPIO 21 and SCL on GPIO 22
  delay(1000); // Wait for serial communication to start

  // Configure accelerometer (CTRL1_XL)
  // ODR_XL = 104 Hz, 2g full scale, 100 Hz filter
  writeRegister(LSM6DS3TR_C_ADDRESS, CTRL1_XL, 0x60);

  // Configure gyroscope (CTRL2_G)
  // ODR_G = 104 Hz, 250 dps full scale
  writeRegister(LSM6DS3TR_C_ADDRESS, CTRL2_G, 0x60);

  Serial.println("LSM6DS3TR-C initialization complete");
}

void loop() {
  // Read accelerometer data
  int16_t accelX = read16(LSM6DS3TR_C_ADDRESS, OUTX_L_XL, OUTX_H_XL);
  int16_t accelY = read16(LSM6DS3TR_C_ADDRESS, OUTY_L_XL, OUTY_H_XL);
  int16_t accelZ = read16(LSM6DS3TR_C_ADDRESS, OUTZ_L_XL, OUTZ_H_XL);

  // Convert to g values
  float accelX_g = accelX * 0.061 / 1000;
  float accelY_g = accelY * 0.061 / 1000;
  float accelZ_g = accelZ * 0.061 / 1000;

  // Read gyroscope data
  int16_t gyroX = read16(LSM6DS3TR_C_ADDRESS, OUTX_L_G, OUTX_H_G);
  int16_t gyroY = read16(LSM6DS3TR_C_ADDRESS, OUTY_L_G, OUTY_H_G);
  int16_t gyroZ = read16(LSM6DS3TR_C_ADDRESS, OUTZ_L_G, OUTZ_H_G);

  // Convert to dps values
  float gyroX_dps = gyroX * 8.75 / 1000;
  float gyroY_dps = gyroY * 8.75 / 1000;
  float gyroZ_dps = gyroZ * 8.75 / 1000;

  // Print the values
  Serial.print("Accel X: "); Serial.print(accelX_g, 3); Serial.print(" g, ");
  Serial.print("Accel Y: "); Serial.print(accelY_g, 3); Serial.print(" g, ");
  Serial.print("Accel Z: "); Serial.print(accelZ_g, 3); Serial.println(" g");

  Serial.print("Gyro X: "); Serial.print(gyroX_dps, 3); Serial.print(" dps, ");
  Serial.print("Gyro Y: "); Serial.print(gyroY_dps, 3); Serial.print(" dps, ");
  Serial.print("Gyro Z: "); Serial.print(gyroZ_dps, 3); Serial.println(" dps");

  delay(1000);
}

// Function to read a 16-bit register
int16_t read16(uint8_t deviceAddress, uint8_t lowAddress, uint8_t highAddress) {
  uint8_t lowByte = readRegister(deviceAddress, lowAddress);
  uint8_t highByte = readRegister(deviceAddress, highAddress);
  return (int16_t)(highByte << 8 | lowByte);
}

// Function to read an 8-bit register
uint8_t readRegister(uint8_t deviceAddress, uint8_t registerAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.endTransmission(false); // Send stop at end of transmission
  Wire.requestFrom(deviceAddress, (uint8_t)1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0; // Return 0 if no data is available
}

// Function to write to an 8-bit register
void writeRegister(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}
