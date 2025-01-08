// Define the pin connected to the potentiometer
const int potPin = 34;  // GPIO 34 (ADC1_6)

// Variable to store the potentiometer value
int potValue = 0;

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Configure the potentiometer pin as an input
  pinMode(potPin, INPUT);
}

void loop() {
  // Read the analog value from the potentiometer
  potValue = analogRead(potPin);

  // Print the potentiometer value to the Serial Monitor
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  // Add a small delay to avoid overwhelming the serial output
  delay(500);
}
