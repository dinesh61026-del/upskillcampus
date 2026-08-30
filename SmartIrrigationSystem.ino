#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RELAY_PIN 25
#define THRESHOLD 2000

int soilValue = 3000;   // Start as wet
bool drying = true;     // Direction of change

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello Rancher!");
  display.println("");
  display.println("Monitoring soil moisture...");
  display.display();
  delay(3000);
}

void loop() {
  // Simulate soil drying/wetting automatically
  if (drying) {
    soilValue -= 100;   // soil dries
    if (soilValue < 1000) drying = false; // switch to wetting
  } else {
    soilValue += 100;   // soil gets wet
    if (soilValue > 3000) drying = true;  // switch to drying
  }

  Serial.println(soilValue);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.println("Soil Value: " + String(soilValue));
  display.println("");

  if (soilValue < THRESHOLD) {
    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    display.println("💧 Soil is Dry");
    display.println("");
    display.println("Pump: ON ✅");
    display.println("");
    display.println("Pumping water...");
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    display.println("🌱 Soil is Wet");
    display.println("");
    display.println("Pump: OFF ❌");
    display.println("");
    display.println("Saving water...");
  }

  display.display();
  delay(1000);
}
