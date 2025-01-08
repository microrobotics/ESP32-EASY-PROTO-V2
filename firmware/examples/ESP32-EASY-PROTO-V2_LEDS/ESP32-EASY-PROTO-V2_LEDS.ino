// Define the pins connected to the LEDs
const int LED3Pin = 15; // GPIO 15, Blue LED
const int LED4Pin = 2;  // GPIO 2, Red LED

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Configure the LED pins as outputs
  pinMode(LED3Pin, OUTPUT);
  pinMode(LED4Pin, OUTPUT);

  // Start with LEDs turned off
  digitalWrite(LED3Pin, LOW);
  digitalWrite(LED4Pin, LOW);
}

void loop() {
  // Turn on the blue LED
  Serial.println("Turning on the blue LED (LED3)");
  digitalWrite(LED3Pin, HIGH);
  delay(1000); // Keep the blue LED on for 1 second

  // Turn off the blue LED
  Serial.println("Turning off the blue LED (LED3)");
  digitalWrite(LED3Pin, LOW);

  // Turn on the red LED
  Serial.println("Turning on the red LED (LED4)");
  digitalWrite(LED4Pin, HIGH);
  delay(1000); // Keep the red LED on for 1 second

  // Turn off the red LED
  Serial.println("Turning off the red LED (LED4)");
  digitalWrite(LED4Pin, LOW);

  // Add a small delay to avoid overwhelming the serial output
  delay(500);
}
