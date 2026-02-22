
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(115200);
  Serial.println("16 channel PWM test!");

  pwm.begin();

  pwm.setOscillatorFrequency(27000000); // See Adafruit PWM examples for details.

  pwm.setPWMFreq(60);  // This is the PWM frequency. Low for motor driving.
  // TODO: Use scope to optimize value.

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  Wire.setClock(400000); // Fast 400kHz. Lower value if issues.

}

void loop() {
  // Drive each PWM in a 'wave'
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
    }
  }
}
