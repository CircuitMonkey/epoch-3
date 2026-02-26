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

#define BUTTON_SM_SIZE 64
#define BUTTON_LG_SIZE 96
#define GLYPH_SM_SIZE 24

#define MODE_UP_DOWN 0
#define MODE_DOWN 1
#define MODE_UP 2
#define MODE_RANDOM 3
#define MODE_DOT_UP_DOWN 4
#define MODE_DOT_DOWN 5
#define MODE_DOT_UP 6
#define MODE_AST 7

#define MODE_ROW_1_Y 12
#define MODE_ROW_2_Y 88
#define MODE_COL_1_X 12
#define MODE_COL_2_X 88
#define MODE_COL_3_X 164
#define MODE_COL_4_X 240

#define PLAY_PAUSE_X 12
#define PLAY_PAUSE_Y 382

#define HAMMER_X 250
#define HAMMER_Y 400


TFT_eSprite knob = TFT_eSprite(&tft);         // Sprite for the slide knob
SliderWidget s1 = SliderWidget(&tft, &knob);  // Slider 1 widget
SliderWidget s2 = SliderWidget(&tft, &knob);  // Slider 2 widget
SliderWidget s3 = SliderWidget(&tft, &knob);  // Slider 3 widget
SliderWidget s4 = SliderWidget(&tft, &knob);  // Slider 4 widget
SliderWidget s5 = SliderWidget(&tft, &knob);  // Slider 5 widget

uint8_t sliderValue[][5] = {
  { 30,50,50,30,60 }, // Up-down
  { 30,50,50,30,60 }, // down
  { 30,50,50,30,60 }, // up
  { 30,50,50,30,60 }, // random
  { 30,50,50,30,60 }, // dot up-down
  { 30,50,50,30,60 }, // dot down
  { 30,50,50,30,60 }, // dot up
  { 30,50,50,30,60 }, // asterisk
};

uint8_t mode = MODE_UP_DOWN;
uint8_t run = 0;
boolean drag = false;
boolean hammerTime = false;

void mainPageInit() {
  // Slider slot parameters
  slider_t param;
  tft.fillScreen(TFT_BLACK);

  updateModeButtons();

  // Create slider using parameters and plot at 0,0
  param.slotWidth = 12;                // Note: ends of slot will be rounded and anti-aliased
  param.slotLength = SLIDER_H;         // Length includes rounded ends
  param.slotColor = THEME_COLOR_DARK;  // Slot color
  param.slotBgColor = TFT_BLACK;       // Slot background color for anti-aliasing
  param.orientation = V_SLIDER;        // sets it "true" for horizontal

  // Slider control knob parameters (smooth rounded rectangle)
  param.knobWidth = 45;                    // Always along x axis
  param.knobHeight = 35;                   // Always along y axis
  param.knobRadius = SLIDER_KNOB_CORNER;   // Corner radius
  param.knobColor = THEME_COLOR_LITE;      // Anti-aliased with slot backgound colour
  param.knobLineColor = THEME_COLOR_DARK;  // Colour of marker line (set to same as knobColor for no line)

  // Slider range and movement speed
  param.sliderLT = 99;      // Left side for horizontal, top for vertical slider
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

  updateSliderPositions();

  // PLay/Pause toggle button
  updatePlayPauseButton();

  updateHammerButton();
}

void mainPageLoop() {
  static uint32_t scanTime = millis();
  uint16_t t_x = 9999, t_y = 9999;  // To store the touch coordinates

  // Scan for touch every 50ms
  if (millis() - scanTime >= 20) {
    if (tft.getTouch(&t_x, &t_y, 250)) {
      if (s1.checkTouch(t_x, t_y)) {
        Serial.print("Slider 1 = ");
        Serial.println(s1.getSliderPosition());
        sliderValue[mode][0] = s1.getSliderPosition();
      }
      if (s2.checkTouch(t_x, t_y)) {
        Serial.print("Slider 2 = ");
        Serial.println(s2.getSliderPosition());
        sliderValue[mode][1] = s2.getSliderPosition();
      }
      if (s3.checkTouch(t_x, t_y)) {
        Serial.print("Slider 3 = ");
        Serial.println(s3.getSliderPosition());
        sliderValue[mode][2] = s3.getSliderPosition();
      }
      if (s4.checkTouch(t_x, t_y)) {
        Serial.print("Slider 4 = ");
        Serial.println(s4.getSliderPosition());
        sliderValue[mode][3] = s4.getSliderPosition();
      }
      if (s5.checkTouch(t_x, t_y)) {
        Serial.print("Slider 5 = ");
        Serial.println(s5.getSliderPosition());
        sliderValue[mode][4] = s5.getSliderPosition();
      }

      // Mode selection
      if (modeSelectTouch(t_x, t_y)) {
        Serial.println("Mode Changed");
        updateModeButtons();
      }

      // Play/Pause toggle
      if (!drag && playPauseSelectTouch(t_x, t_y)) {
        if (run) {
          run = 0;
        } else {
          run = 1;
        }
        updatePlayPauseButton();
      }

      // Hammer Time toggle
      if (!drag && hammerSelectTouch(t_x, t_y)) {
        if (hammerTime) {
          hammerTime = false;
        } else {
          hammerTime = true;
        }
        updateHammerButton();
      }

      drag = true;
    } else {
      drag = false;
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
  TFT_eSprite sspr = TFT_eSprite(&tft);  // Declare Sprite with pointer to "tft" object
  sspr.fillScreen(TFT_BLACK);
  sspr.setColorDepth(1);
  sspr.createSprite(64, 64);
  sspr.setBitmapColor(color, TFT_BLACK);
  sspr.pushImage(0, 0, 64, 64, (uint16_t *)glyph);
  sspr.pushSprite(x, y);
}

void drawLargeButton(uint16_t x, uint16_t y, const unsigned char *glyph) {
  TFT_eSprite bspr = TFT_eSprite(&tft);  // Declare Sprite with pointer to "tft" object
  bspr.fillScreen(TFT_BLACK);
  bspr.setColorDepth(1);
  bspr.createSprite(96, 96);
  bspr.setBitmapColor(THEME_COLOR_MID, TFT_BLACK);
  bspr.pushImage(0, 0, 96, 96, (uint16_t *)glyph);
  bspr.pushSprite(x, y);
}

void updateModeButtons() {
  // Two rows of glyph buttons
  drawSmallButton(MODE_COL_1_X, MODE_ROW_1_Y, mode == MODE_UP_DOWN ? THEME_COLOR_LITE : THEME_COLOR_DARK, upDown);
  drawSmallButton(MODE_COL_2_X, MODE_ROW_1_Y, mode == MODE_DOWN ? THEME_COLOR_LITE : THEME_COLOR_DARK, down);
  drawSmallButton(MODE_COL_3_X, MODE_ROW_1_Y, mode == MODE_UP ? THEME_COLOR_LITE : THEME_COLOR_DARK, up);
  drawSmallButton(MODE_COL_4_X, MODE_ROW_1_Y, mode == MODE_RANDOM ? THEME_COLOR_LITE : THEME_COLOR_DARK, dice);

  drawSmallButton(MODE_COL_1_X, MODE_ROW_2_Y, mode == MODE_DOT_UP_DOWN ? THEME_COLOR_LITE : THEME_COLOR_DARK, dotUpDown);
  drawSmallButton(MODE_COL_2_X, MODE_ROW_2_Y, mode == MODE_DOT_DOWN ? THEME_COLOR_LITE : THEME_COLOR_DARK, dotDown);
  drawSmallButton(MODE_COL_3_X, MODE_ROW_2_Y, mode == MODE_DOT_UP ? THEME_COLOR_LITE : THEME_COLOR_DARK, dotUp);
  drawSmallButton(MODE_COL_4_X, MODE_ROW_2_Y, mode == MODE_AST ? THEME_COLOR_LITE : THEME_COLOR_DARK, asterisk);
}

void updatePlayPauseButton() {
  drawLargeButton(PLAY_PAUSE_X, PLAY_PAUSE_Y, run ? play : pause);
}

void updateHammerButton() {
  drawSmallButton(HAMMER_X, HAMMER_Y, hammerTime ? THEME_COLOR_LITE : THEME_COLOR_DARK, hammer);
}

/**
 * If one of the mode change buttons was pressed and is different than
 * the current mode, mode variable will be set to the pressed mode.
 *
 * @return true only if the mode was changed.
*/
boolean modeSelectTouch(uint16_t x, uint16_t y) {
  boolean changed = false;
  if (y > MODE_ROW_1_Y && y < MODE_ROW_1_Y + BUTTON_SM_SIZE) {
    if (x > MODE_COL_1_X && x < MODE_COL_1_X + BUTTON_SM_SIZE) {
      if (mode != 0) {
        mode = 0;
        changed = true;
      }
    } else if (x > MODE_COL_2_X && x < MODE_COL_2_X + BUTTON_SM_SIZE) {
      if (mode != 1) {
        mode = 1;
        changed = true;
      }
    } else if (x > MODE_COL_3_X && x < MODE_COL_3_X + BUTTON_SM_SIZE) {
      if (mode != 2) {
        mode = 2;
        changed = true;
      }
    } else if (x > MODE_COL_4_X && x < MODE_COL_4_X + BUTTON_SM_SIZE) {
      if (mode != 3) {
        mode = 3;
        changed = true;
      }
    }
  } else if (y > MODE_ROW_2_Y && y < MODE_ROW_2_Y + BUTTON_SM_SIZE) {
    if (x > MODE_COL_1_X && x < MODE_COL_1_X + BUTTON_SM_SIZE) {
      if (mode != 4) {
        mode = 4;
        changed = true;
      }
    } else if (x > MODE_COL_2_X && x < MODE_COL_2_X + BUTTON_SM_SIZE) {
      if (mode != 5) {
        mode = 5;
        changed = true;
      }
    } else if (x > MODE_COL_3_X && x < MODE_COL_3_X + BUTTON_SM_SIZE) {
      if (mode != 6) {
        mode = 6;
        changed = true;
      }
    } else if (x > MODE_COL_4_X && x < MODE_COL_4_X + BUTTON_SM_SIZE) {
      if (mode != 7) {
        mode = 7;
        changed = true;
      }
    }
  }

  if ( changed ) {
    updateSliderPositions();
  }

  return changed;
}

boolean playPauseSelectTouch(uint16_t x, uint16_t y) {
  boolean changed = false;
  if (y > PLAY_PAUSE_Y && y < PLAY_PAUSE_Y + BUTTON_LG_SIZE) {
    if (x > PLAY_PAUSE_X && x < PLAY_PAUSE_X + BUTTON_LG_SIZE) {
      changed = true;
    }
  }
  return changed;
}

boolean hammerSelectTouch(uint16_t x, uint16_t y) {
  boolean changed = false;
  if (y > HAMMER_Y && y < HAMMER_Y + BUTTON_SM_SIZE) {
    if (x > HAMMER_X && x < HAMMER_X + BUTTON_SM_SIZE) {
      changed = true;
    }
  }
  return changed;
}

void updateSliderPositions() {
  s1.setSliderPosition(sliderValue[mode][0]);
  s1.getSliderPosition();
  s2.setSliderPosition(sliderValue[mode][1]);
  s2.getSliderPosition();
  s3.setSliderPosition(sliderValue[mode][2]);
  s3.getSliderPosition();
  s4.setSliderPosition(sliderValue[mode][3]);
  s4.getSliderPosition();
  s5.setSliderPosition(sliderValue[mode][4]);
  s5.getSliderPosition();

}
