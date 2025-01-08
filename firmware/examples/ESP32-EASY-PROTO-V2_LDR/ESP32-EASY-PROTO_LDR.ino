// Define the pin connected to the LDR
const int ldrPin = 35;  // GPIO 35 (ADC1_7)

// Variable to store the LDR value
int ldrValue = 0;

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Configure the LDR pin as an input
  pinMode(ldrPin, INPUT);
}

void loop() {
  // Read the analog value from the LDR
  ldrValue = analogRead(ldrPin);

  // Print the LDR value to the Serial Monitor
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Add a small delay to avoid overwhelming the serial output
  delay(500);
}
