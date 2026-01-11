#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define DISPLAY_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool displayInitialized;

void setup() {
  displayInitialized = display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);

  if (displayInitialized) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Hello,");
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("SSD1306 OLED Example");
    display.display();
  }
}

void loop() {
}