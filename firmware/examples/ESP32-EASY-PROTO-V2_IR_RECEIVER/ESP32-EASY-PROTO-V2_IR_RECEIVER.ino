#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int recvPin = 36;  // GPIO 36 for the IR receiver

void setup() {
  Serial.begin(115200);
  pinMode(recvPin, INPUT);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  Serial.println("IR Receiver is ready");
}

void loop() {
  // Check if the IR receiver detects a signal
  if (digitalRead(recvPin) == LOW) {
    // Measure the duration of the LOW pulse
    unsigned long lowPulse = pulseIn(recvPin, LOW);
    // Measure the duration of the HIGH pulse
    unsigned long highPulse = pulseIn(recvPin, HIGH);

    // Convert pulse durations to hexadecimal
    char lowPulseHex[10];
    char highPulseHex[10];
    sprintf(lowPulseHex, "0x%lX", lowPulse);
    sprintf(highPulseHex, "0x%lX", highPulse);

    // Print the pulse durations to the serial monitor
    Serial.print("Low pulse: ");
    Serial.print(lowPulse);
    Serial.print(" (");
    Serial.print(lowPulseHex);
    Serial.println(")");
    Serial.print("High pulse: ");
    Serial.print(highPulse);
    Serial.print(" (");
    Serial.print(highPulseHex);
    Serial.println(")");

    // Display the pulse durations on the OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("IR Receiver Data");
    display.setCursor(0, 10);
    display.print("Low: ");
    display.print(lowPulse);
    display.print(" (");
    display.print(lowPulseHex);
    display.println(")");
    display.setCursor(0, 20);
    display.print("High: ");
    display.print(highPulse);
    display.print(" (");
    display.print(highPulseHex);
    display.println(")");
    display.display();
  }
}
