# epoch-3
Haptic Field Emitter using Vibration Motors

A follow on to [Epoch 2](https://github.com/CircuitMonkey/epoch-2) but in it's fifth generation.  Redesigned with [RaspberryPi Pico 2](https://www.raspberrypi.com/products/raspberry-pi-pico-2/) and a [4-inch TFT SPI resistive touch display](https://www.amazon.com/dp/B0CKRJ81B5).  Motors driven with [pca9685 PWM controller](https://learn.adafruit.com/adafruit-16-channel-pwm-servo-hat-for-raspberry-pi)/[Amazon PCA9685 Board](https://www.amazon.com/dp/B0D73811V8) and FET outputs in order to drive bigger motors. 

### Now 100% Python Free
Epoch-3 improves upon Epoch-2 by eliminating the use of Circuit Python, which ran extremely slow, cumbersome to program (no proper IDE) and caused many unexpected run-time errors. Epoch-3 uses proven Arduino C++ libraries and much easier to program/debug in for us old timers.

### Display Changes
The display has also been changed.  Epoch-2 used a IPS type display, and while it had more color rannge, it used nealy all the pins on the microcontroller and had slow update rates for most UI features. Simple things like the touch slider UI element were frustrating to use because of the slow reaction time. Epoch-3 uses a SPI type TFT display. While color range reminds us of those VGA days, the update rate is far faster.  

### Raspberry Pi Pico 2
For me, RaspberryPi Pico 2 has been a wonderful platform to develop with in other projects, especially when programming in Arduino.

### Ten Output Channels
This design outputs ten motor channels via 1/8-inch TRS jacks.  We elmininated the air pressure sensor input found on Epoch-2 as it was cumbersome to experiment with and yielded less than expected results.

### Refined User Interface
Epoch-2 had a much improved user interface over Epoch-1 and we will bring the best of those features into Epoch-3.  We hope to improve on that interface by adding more modes or at least a way to remember and review usage patterns.


## What is a "Haptic Field Emmiter" ?
The term haptic Field Emmiter is our cheeky term for Vibrator Controller.  This device is designed to drive up to ten vibration motors and intended for intimate bedroom use, usually by a solo male user.  The motors are embodied by a selection of attachments for specific zones of the male anatomy.  First, Epoch uses movable/removable rings, each with two small motors that attach on the male member. This edition will support up to three. Second, a similar, but different sized, ring is intented to be attached at the base of the member or around the testes. Again, containing two small motors.  Third, a common prostate massager toy (optional) has been selected and modified to work with epoch, also with two motors.  This "mesh" of motors forms the Haptic Field while in operation.  The Epoch device operates the motors in patterns to create the sensation of motion touching or stroking along a path.  Those patterns are controllable by sliders on the UI touch screen during use.  The UI can effect the intensiy of each motor group as well as the speed at which the patterns occur, offering a seemingly infinite palette of sensations over the time of use. The motors are of much-much lower intensity than common toys. They are for creating sensations and that's what leads to hours long euphoric sessions of use. This device is intended to get you into a grey zone of pleasure and keep you there.

## Clear Your Schedule
The designer has found that this device, when used properly, can lead to multiple-hours-long sessions of euphoric use.  Final orgasm is usually only the result of euphoria-fatigue or getting tired of using the device (or maybe finally your DoorDash showed up?). That said, the entire erogenous zone has a very short-fuse after that amount of time. Every inch of skin becomes a highly sensitive trigger point that is reactive to any novel sensations. Beware.

## For Research Purposes Only
Even in this fifth generation of careful design considerations, this device is experimental. It's not a solution for any known problem in the world and may not work for most users. It's also not for sale.  The design files are here for anyone who wants to try building one for themselves but you'll need enough skills to build all the components. Those skill include PCB assemly, 3D printing, silicone molding and other craft skills.  No kind of safety testing has been performed. Don't attach things onto or in your body if you don't understand the risks.