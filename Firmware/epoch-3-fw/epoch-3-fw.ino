/**
 Epoch 3 Project - A 10-channel haptic field emitter
 with touch screen by Mark J. Koch.
        https://www.patreon.com/maehem

 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  MIT license
 ********************************************************************
 
 */
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "glyphs.h"

// Width and height of sprite
#define WIDTH  128
#define HEIGHT 88

#define TOUCH_X_MIN 220
#define TOUCH_Y_MIN 3920
#define TOUCH_X_MAX 3800
#define TOUCH_Y_MAX 240


// default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // PWM functions
TFT_eSPI tft = TFT_eSPI();                                // Screen and touch related functions
TFT_eSprite spr = TFT_eSprite(&tft);                      // Declare Sprite object "spr" with pointer to "tft" object

boolean wastouched = false;
boolean istouched = false;
uint32_t scanTime = millis();

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nEpoch 3 Startup...");
  spr.setColorDepth(1);

  pwmInit();

  tft.init();
  tft.setRotation(0);  // Epoch 3 is portriat mode.

  // Run Touch Calibrate TFT-eSPI example for these values.
  uint16_t calData[5] = { 260, 3429, 247, 3635, 4 };
  tft.setTouch(calData);

  Serial.println("\nsplash");
  showSplashScreen();
  delay(1000);
  
  Serial.println("\nmainpage");
  mainPageInit();
}

void loop() {
  mainPageLoop();
  pwmLoop();
}

void showSplashScreen() {
  spr.createSprite(WIDTH, HEIGHT);
  spr.setBitmapColor(0x9110, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(0x9110);
  tft.drawCentreString("Epoch 3",tft.width()/2, tft.height()/2-88, 4);
  // push the image - only need to do this once.
  spr.pushImage(0, 0, WIDTH, HEIGHT, (uint16_t *)logo);
  // create a palette with the defined colors and push it.
  spr.pushSprite(tft.width() / 2 - WIDTH / 2, tft.height() / 2 - HEIGHT / 2);
}


