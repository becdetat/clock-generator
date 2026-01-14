#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define DISPLAY_ADDRESS 0x3C
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool displayInitialized;
int bpm = 120;
volatile int encoderPos = 0;
volatile bool encoderMoved = false;
volatile unsigned long lastEncoderInterrupt = 0;
const unsigned long encoderDebounceMs = 6;

void handleEncoder() {
  static uint8_t lastState = 0;
  uint8_t state = (digitalRead(ENCODER_PIN_A) << 1) | digitalRead(ENCODER_PIN_B);
  // Only count when both pins are LOW (detent position)
  if (state == 0b00 && lastState != 0b00) {
    // Determine direction by checking previous state
    if (lastState == 0b01) {
      encoderPos++;
    } else if (lastState == 0b10) {
      encoderPos--;
    }
    encoderMoved = true;
  }
  lastState = state;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Intializing display");
  displayInitialized = display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  if (displayInitialized) {
    Serial.println("Display initialized");
    display.clearDisplay();
    display.display();
  } else {
    Serial.println("Display initialization failed");
  }
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoder, CHANGE);
}

void loop() {
  if (!displayInitialized) {
    return;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("BPM:");

  display.setCursor(0, 16);
  display.setTextSize(2);
  display.print(bpm);
  display.display();

  if (encoderMoved) {
    bpm += encoderPos;
    if (bpm < 1) bpm = 1;
    if (bpm > 300) bpm = 300;
    encoderPos = 0;
    encoderMoved = false;
  }
}