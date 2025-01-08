// Define the pin connected to the MOSFET gate
const int buzzerPin = 27;

// PWM properties
const int pwmChannel = 0;
const int pwmFrequency = 2700; // 2.7 kHz as per resonant frequency of MLT-8530
const int pwmResolution = 8; // 8-bit resolution

void setup() {
  // Configure the PWM channel
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);

  // Attach the PWM channel to the buzzer pin
  ledcAttachPin(buzzerPin, pwmChannel);
}

void loop() {
  // Generate tone by setting the duty cycle
  ledcWrite(pwmChannel, 128); // 50% duty cycle (128 out of 255 for 8-bit resolution)
  
  // Keep the tone on for 1 second
  delay(1000);
  
  // Stop the tone by setting the duty cycle to 0
  ledcWrite(pwmChannel, 0);
  
  // Pause for a while before generating the next tone
  delay(2000); // 2 seconds
}
