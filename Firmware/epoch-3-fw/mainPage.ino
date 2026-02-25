#include <TFT_eWidget.h>  // Widget library

#define SLIDER_Y 190
#define SLIDER_1_X 12
#define SLIDER_H 180
#define SLIDER_SPACING 60
#define SLIDER_KNOB_CORNER 10
#define SLIDER_OUTER_CORNER 14

#define THEME_COLOR_LITE 0xB0DF
#define THEME_COLOR_MID 0x8133
#define THEME_COLOR_DARK 0x3868

TFT_eSprite knob = TFT_eSprite(&tft);         // Sprite for the slide knob
SliderWidget s1 = SliderWidget(&tft, &knob);  // Slider 1 widget
SliderWidget s2 = SliderWidget(&tft, &knob);  // Slider 2 widget
SliderWidget s3 = SliderWidget(&tft, &knob);  // Slider 3 widget
SliderWidget s4 = SliderWidget(&tft, &knob);  // Slider 4 widget
SliderWidget s5 = SliderWidget(&tft, &knob);  // Slider 5 widget

void mainPageInit() {
  // Slider slot parameters
  slider_t param;
  tft.fillScreen(TFT_BLACK);

  // Two rows of glyph buttons
  drawSmallButton(12, 12, THEME_COLOR_LITE, upDown);
  drawSmallButton(88, 12, THEME_COLOR_DARK, down);
  drawSmallButton(164, 12, THEME_COLOR_DARK, up);
  drawSmallButton(240, 12, THEME_COLOR_DARK, dice);

  drawSmallButton(12, 88, THEME_COLOR_DARK, dotUpDown);
  drawSmallButton(88, 88, THEME_COLOR_DARK, dotDown);
  drawSmallButton(164, 88, THEME_COLOR_DARK, dotUp);
  drawSmallButton(240, 88, THEME_COLOR_DARK, asterisk);

  // Create slider using parameters and plot at 0,0
  param.slotWidth = 12;                // Note: ends of slot will be rounded and anti-aliased
  param.slotLength = SLIDER_H;         // Length includes rounded ends
  param.slotColor = THEME_COLOR_DARK;  // Slot colour
  param.slotBgColor = TFT_BLACK;       // Slot background colour for anti-aliasing
  param.orientation = V_SLIDER;        // sets it "true" for horizontal

  // Slider control knob parameters (smooth rounded rectangle)
  param.knobWidth = 45;                    // Always along x axis
  param.knobHeight = 35;                   // Always along y axis
  param.knobRadius = SLIDER_KNOB_CORNER;   // Corner radius
  param.knobColor = THEME_COLOR_LITE;      // Anti-aliased with slot backgound colour
  param.knobLineColor = THEME_COLOR_DARK;  // Colour of marker line (set to same as knobColor for no line)

  // Slider range and movement speed
  param.sliderLT = 100;      // Left side for horizontal, top for vertical slider
  param.sliderRB = 0;        // Right side for horizontal, bottom for vertical slider
  param.startPosition = 50;  // Start position for control knob
  param.sliderDelay = 100;   // Microseconds per pixel movement delay (0 = no delay)

  int16_t x, y;   // x and y can be negative
  uint16_t w, h;  // Width and height

  drawSmallGlyph(SLIDER_1_X, SLIDER_Y - 26, tip);
  drawSmallGlyph(1 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y - 26, mid);
  drawSmallGlyph(2 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y - 26, plug);
  drawSmallGlyph(3 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y - 26, snail);
  drawSmallGlyph(4 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y - 26, ghost);

  s1.drawSlider(SLIDER_1_X, SLIDER_Y, param);
  s1.getBoundingRect(&x, &y, &w, &h);                                    // Update x,y,w,h with bounding box
  tft.drawRoundRect(x, y, w, h, SLIDER_OUTER_CORNER, THEME_COLOR_LITE);  // Draw rectangle outline

  s2.drawSlider(1 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y, param);
  s2.getBoundingRect(&x, &y, &w, &h);
  tft.drawRoundRect(x, y, w, h, SLIDER_OUTER_CORNER, THEME_COLOR_LITE);

  s3.drawSlider(2 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y, param);
  s3.getBoundingRect(&x, &y, &w, &h);
  tft.drawRoundRect(x, y, w, h, SLIDER_OUTER_CORNER, THEME_COLOR_LITE);

  s4.drawSlider(3 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y, param);
  s4.getBoundingRect(&x, &y, &w, &h);
  tft.drawRoundRect(x, y, w, h, SLIDER_OUTER_CORNER, THEME_COLOR_LITE);

  s5.drawSlider(4 * SLIDER_SPACING + SLIDER_1_X, SLIDER_Y, param);
  s5.getBoundingRect(&x, &y, &w, &h);
  tft.drawRoundRect(x, y, w, h, SLIDER_OUTER_CORNER, THEME_COLOR_LITE);

  s1.setSliderPosition(70);
  s1.getSliderPosition();
  s2.setSliderPosition(70);
  s2.getSliderPosition();
  s3.setSliderPosition(70);
  s3.getSliderPosition();
  s4.setSliderPosition(70);
  s4.getSliderPosition();
  s5.setSliderPosition(70);
  s5.getSliderPosition();

  // Row of large buttons. Play, Hammer
  drawLargeButton(12, 382, play);

  drawSmallButton(242,390, THEME_COLOR_DARK, hammer);
}

void mainPageLoop() {
  static uint32_t scanTime = millis();
  uint16_t t_x = 9999, t_y = 9999;  // To store the touch coordinates

  // Scan for touch every 50ms
  if (millis() - scanTime >= 20) {
    // Pressed will be set true if there is a valid touch on the screen
    if (tft.getTouch(&t_x, &t_y, 250)) {
      if (s1.checkTouch(t_x, t_y)) {
        Serial.print("Slider 1 = ");
        Serial.println(s1.getSliderPosition());
      }
      if (s2.checkTouch(t_x, t_y)) {
        Serial.print("Slider 2 = ");
        Serial.println(s2.getSliderPosition());
      }
      if (s3.checkTouch(t_x, t_y)) {
        Serial.print("Slider 3 = ");
        Serial.println(s3.getSliderPosition());
      }
      if (s4.checkTouch(t_x, t_y)) {
        Serial.print("Slider 4 = ");
        Serial.println(s4.getSliderPosition());
      }
      if (s5.checkTouch(t_x, t_y)) {
        Serial.print("Slider 5 = ");
        Serial.println(s5.getSliderPosition());
      }
    }
    scanTime = millis();
  }
}

void drawSmallGlyph(uint16_t x, uint16_t y, const unsigned char *glyph) {
  TFT_eSprite gspr = TFT_eSprite(&tft);
  gspr.fillScreen(TFT_BLACK);
  gspr.setColorDepth(1);
  gspr.createSprite(24, 24);
  gspr.setBitmapColor(THEME_COLOR_MID, TFT_BLACK);
  gspr.pushImage(0, 0, 24, 24, (uint16_t *)glyph);
  gspr.pushSprite(x + 12, y);
}

void drawSmallButton(uint16_t x, uint16_t y, uint16_t color, const unsigned char *glyph) {
  TFT_eSprite sspr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
  sspr.fillScreen(TFT_BLACK);
  sspr.setColorDepth(1);
  sspr.createSprite(64, 64);
  sspr.setBitmapColor(color, TFT_BLACK);
  sspr.pushImage(0, 0, 64, 64, (uint16_t *)glyph);
  sspr.pushSprite(x, y);
}

void drawLargeButton(uint16_t x, uint16_t y, const unsigned char *glyph) {
  TFT_eSprite bspr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
  bspr.fillScreen(TFT_BLACK);
  bspr.setColorDepth(1);
  bspr.createSprite(96, 96);
  bspr.setBitmapColor(THEME_COLOR_MID, TFT_BLACK);
  bspr.pushImage(0, 0, 96, 96, (uint16_t *)glyph);
  bspr.pushSprite(x, y);
}
