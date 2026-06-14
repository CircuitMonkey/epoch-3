
#define PWM_TICK_MIN 48
#define PWM_TICK_MIN_TRIM 64

#define PWM_TICK_MAX 143
#define PWM_TICK_MAX_456 120
#define PWM_TICK_MAX_TRIM 128
#define PWM_TICK_MAX_TRIM_456 108
#define FWD true
#define REV false
#define SLOW_SPEED 16
// There are 96 x 2 ticks == 192 total
// A tick is a reference into the wave or dot table.
// Tick 188 wraps around to 0.
int16_t pwmTick = 0;
uint8_t tickStart = 0;
uint8_t tickEnd = PWM_TICK_MAX;
int16_t waitTicks = 0; // Set to slider 4 value when end reached.
boolean tickDir = FWD;  // Forward

/*

 TODO: trimTime ...  tick back forth end points are in by TRIM amount 24?

Do we slow down tick rate so it maches non-trm mode rate? use speed var?

*/


void pwmInit() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // See Adafruit PWM examples for details.
  pwm.setPWMFreq(60);                    // This is the PWM frequency. Low for motor driving.
  Wire.setClock(400000);                 // Fast 400kHz. Lower value if issues.
}

void updateMinMax(uint8_t mode) {
  switch (mode) {
    case 0:
    case 1:
    case 2:
      tickStart = trimTime?PWM_TICK_MIN_TRIM:PWM_TICK_MIN;
      tickEnd = trimTime?PWM_TICK_MAX_TRIM:PWM_TICK_MAX;
      break;
    case 3:           // Don't care
      tickStart = PWM_TICK_MIN;
      tickEnd = PWM_TICK_MAX;
      break;
    case 4:
    case 5:
    case 6:
      tickStart = trimTime?PWM_TICK_MIN_TRIM:PWM_TICK_MIN;
      tickEnd = trimTime?PWM_TICK_MAX_TRIM_456:PWM_TICK_MAX_456;
      break;
    case 7:           // Don't care
      tickStart = PWM_TICK_MIN;
      tickEnd = PWM_TICK_MAX;
      break;
  }
}

void pwmInitMode(uint8_t mode) {
  updateMinMax(mode);

  switch (mode) {
    case 0:
    case 1:
      tickDir = FWD;  // Forward
      pwmTick = tickStart;
      break;
    case 2:
      tickDir = REV;  // Reverse
      pwmTick = tickEnd;
      break;
    case 3: // Don't care
      tickDir = FWD;  // Forward
      pwmTick = tickStart;
      break;
    case 4:
    case 5:
      tickDir = FWD;  // Forward
      pwmTick = tickStart;
      break;
    case 6:
      tickDir = REV;  // Reverse
      pwmTick = tickEnd;
      break;
    case 7: // Don't care
      tickDir = FWD;  // Forward
      pwmTick = tickStart;
      break;
  }
}


static const uint8_t wave[] = {  // Single peak
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 2, 4, 8, 12, 20, 30, 40, 56, 72, 90, 110, 130, 150, 170, 190, 208, 224, 236, 244, 249, 253, 255, 255, 253, 249, 244, 236, 224, 208, 190, 170, 150, 130, 110, 90, 72, 56, 40, 30, 20, 12, 8, 4, 2, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// depricated.  not very good.
static const uint8_t dot[] = {  // Dot/Ripple
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 2, 4, 8, 12, 20, 30, 40, 56, 90, 130, 170, 190, 224, 236, 244, 253, 255, 220, 180, 180, 220, 255, 255, 220, 180, 180, 220, 255, 253, 244, 236, 224, 190, 170, 130, 90, 56, 40, 30, 20, 12, 8, 4, 2, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


void pwmLoop() {
    static uint32_t scanTime = millis();
    uint16_t t_x = 9999, t_y = 9999;  // To store the touch coordinates

    uint8_t speed = sliderValue[mode][3]/2;  // Slider 3 for frame delay

    if ( run == 0 ) {
        motorsOff();
        return;
    }

    if ( waitTicks > 0 ) {
        if ( millis() - scanTime >= (33 ) ) { // Independent of Speed slider.
            waitTicks--;
            //Serial.printf("End Wait: %d\n", endTime);
            scanTime = millis();
        }
        return;
    } else if (millis() - scanTime >= (1 + (49-speed) )) {  // 10fps - Scan for touch every 50ms
        switch (mode) {
        case 0:                // down/up
        case 1:
        case 2:
            processTick( 0, wave );
            break;
        case 3:  // Random
                // Speed slider controls transition (motor old value to motor new value) time
                // Ghost slider controls new random value time.
                // Slider 0 = 200mS min.  99 = 4200mS max

                // TODO
                // Maybe a var like scanTime to change random periodically

            break;
        case 4:
        case 5:
        case 6:
            processTick(1, wave);
            break;
        case 7:  // Asterisk (motors on, no pattern)
            // Test. Use for asterisk mode.
            // First slider. Two motors at tip. Ch: 01 + 02
            pwm.setPWM(0, 0, sliderValue[mode][0] * 40 + 135);  // Range of 0-4095 == 0-99%
            pwm.setPWM(1, 0, 135 + sliderValue[mode][0] * 40);  // Range of 0-4095 == 0-99%

            // Second slider. Four motors at mid. Ch: 03 + 04 + 05 + 06
            pwm.setPWM(2, 0, 135 + sliderValue[mode][1] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(3, 0, 135 + sliderValue[mode][1] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(4, 0, 135 + sliderValue[mode][1] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(5, 0, 135 + sliderValue[mode][1] * 40);  // Range of 0-4095 == 0-99%

            // Third slider. Four motors at rear. Ch: 07 + 08 + 09 + 10
            pwm.setPWM(6, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(7, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(8, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(9, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%

            break;
        }

        // If user changed TRIM mode, let's adjust to it.
        updateMinMax(mode);

        if (mode == 0 || mode == 4) {  // Mode 0 and 4 go back and forth
            if (tickDir) {               // If going forward
                pwmTick++;
                if ( speed > SLOW_SPEED ) {
                    pwmTick++;
                }
                if (pwmTick > tickEnd) {
                    //Serial.println("Mode 0/4 FWD end reached.");
                    pwmTick = tickEnd;
                    tickDir = REV;  // Flip Direction
                    waitTicks = getWait();
                }
            } else {  // going backward
                pwmTick--;
                if ( speed > SLOW_SPEED ) {
                    pwmTick--;
                }
                if (pwmTick < tickStart) {
                    //Serial.println("Mode 0/4 REV end reached.");
                    pwmTick = tickStart; // back where we started
                    tickDir = FWD;  // Flip Direction
                    waitTicks = getWait();
                }
            }
        } else if (mode == 3 || mode == 7) {  // Mode 3 and 7 don't use pwmTick
                // Do nothing.
        } else {  // One direction. tickDir never changes.
            if (tickDir) {
                pwmTick++;
                if (pwmTick > tickEnd) {
                    //Serial.println("Mode 3/7 FWD end reached.");
                    pwmTick = tickStart;  // Reset to zero
                    waitTicks = getWait();
                }
            } else {
                pwmTick--;
                if (pwmTick < tickStart) {
                    //Serial.println("Mode 3/7 REV end reached.");
                    pwmTick = tickEnd;  // Reset to MAX
                    waitTicks = getWait();
                }
            }
        }
        // Advance tick and scanTime
        scanTime = millis();
    }

}

/*
 * style:  0: wave on all channels.
 *          1: wave on ch 1,2,3.  Constant on ch 4,5.
*/
void processTick(int style, const uint8_t* wave) {
    uint16_t intensity;  // 0-99
    uint32_t val;        // 0-4095
    int32_t idx;         // Phase shifted mode index.

    //Serial.printf("processTick() : %d\n", pwmTick);

    // Tip
    intensity = sliderValue[mode][0];  // Slider 0 for motors 1,2
    idx = pwmTick;
    val = getWaveVal(wave, pwmTick) * 16 * intensity / 100;
    pwm.setPWM(0, 0, (uint16_t)val);  // Range of 0-4095 == 0-99%
    pwm.setPWM(1, 0, (uint16_t)val);  // Range of 0-4095 == 0-99%

    // Mid 1
    intensity = sliderValue[mode][1];  // Slider 1 for motors 2,3,4,5
    idx = pwmTick - 12;
    val = getWaveVal(wave, idx) * 16 * intensity / 100;  // 0-4095
    pwm.setPWM(2, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%
    pwm.setPWM(3, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%

    // Mid 2
    idx = pwmTick - 24;
    val = getWaveVal(wave, idx) * 16 * intensity / 100;  // 0-4095
    pwm.setPWM(4, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%
    pwm.setPWM(5, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%

    // Balls
    switch( style ) {
        case 0:
            intensity = sliderValue[mode][2];  // Slider 3 for motors 6,7,8,9
            idx = pwmTick - 36;
            val = getWaveVal(wave, idx) * 16 * intensity / 100;  // 0-4095
            pwm.setPWM(6, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%
            pwm.setPWM(7, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%

            // Plug
            idx = pwmTick - 48;
            val = getWaveVal(wave, idx) * 16 * intensity / 100;  // 0-4095
            pwm.setPWM(8, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%
            pwm.setPWM(9, 0, (uint16_t)val);         // Range of 0-4095 == 0-99%
            break;
        case 1:
            // Third slider. Four motors at rear. Ch: 07 + 08 + 09 + 10
            pwm.setPWM(6, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(7, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(8, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            pwm.setPWM(9, 0, 135 + sliderValue[mode][2] * 40);  // Range of 0-4095 == 0-99%
            break;
    }
}

uint8_t getWait() {
    return sliderValue[mode][4] / 2; // range 0-49
}

void motorsOff() {
    for (int i=0; i< 10; i++ ) {
        pwm.setPWM(i, 0, 0);  // Range of 0-4095 == 0-99%
    }
}

/**
 * Returns table val of 0-255 for non-hammertime.
 * Returns 0 or 255 in hammerTime mode if val is under/over 180 threshold.
 */
uint8_t getWaveVal( const uint8_t* wave, int32_t idx ) {
    uint8_t val = wave[idx];
    if ( hammerTime ) {
        return val>180?255:0; // Hammered value
    }

    return val; // Normal smooth value
}
