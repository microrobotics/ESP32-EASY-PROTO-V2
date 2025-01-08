// Define the pins connected to the push buttons
const int buttonPin1 = 32;  // GPIO 32
const int buttonPin2 = 33;  // GPIO 33

// Variables to store the button states
int buttonState1 = 0;
int buttonState2 = 0;

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);

  // Configure the button pins as input with internal pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  // Read the state of the push buttons
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  // Check if button 1 is pressed
  if (buttonState1 == LOW) {
    Serial.println("Button 1 Pressed");
  }

  // Check if button 2 is pressed
  if (buttonState2 == LOW) {
    Serial.println("Button 2 Pressed");
  }

  // Add a small delay to avoid overwhelming the serial output
  delay(100);
}
