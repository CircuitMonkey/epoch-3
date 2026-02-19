// Pico 2 (RP2350) (ST7789 on SPI bus with 480x320 TFT)
//#define USER_SETUP_ID E3A

#define ST7796_DRIVER     // Configure all registers

// Auto Select Resolution
//#define TFT_WIDTH  320
//#define TFT_HEIGHT 240

// Unused
//#define CGRAM_OFFSET      // Library will add offsets required

// For the RP2350 processor define the SPI port channel used (default 0 if undefined)
#define TFT_SPI_PORT 1 // Set to 0 if SPI0 pins are used, or 1 if spi1 pins used
// For Pico 2 (RP2350)
#define TFT_SPI_PORT 1
#define TFT_MOSI 15
#define TFT_MISO 12
#define TFT_SCLK 14
#define TFT_CS 13   // Chip Select pin
#define TFT_DC 11   // Data Command control pin 
#define TFT_RST 10 // Set TFT_RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V
#define TFT_BL 8 // LED back-light (only for ST7789 with backlight control pin)
#define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)


#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
// #define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Uncomment if not done in User_Setup.h
//#define SMOOTH_FONT

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// TOUCH
// Must use same SPI pins as LCD.
#define TOUCH_CS 9 // Chip select pin (T_CS) of touch screen
//#define TOUCH_IRQ_PIN -1 // unused pin
