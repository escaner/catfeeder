// #define NDEBUG
// #define DEBUG_SERIAL

#include "switchpnl.h"
#include "clock.h"
#include "lcd.h"


/*************/
/* Constants */
/*************/

// Swtich & encoder constants
static const uint8_t ENC_NUM_PINS = 2;  // Number of pins per encoder
static const uint8_t NUM_BUTTONS = 2;  // Number of swtiches of type button

// ARDUINO NANO connections
// Switches
static const uint8_t PIN_ENC[ENC_NUM_PINS] = { 2, 3 };
static const uint8_t PIN_BTN_ENT = 0; /* RX0 */
static const uint8_t PIN_BTN_BCK = 1; /* TX1 */
// Display
static const uint8_t PIN_DSP_RS = 12;
static const uint8_t PIN_DSP_E = 11;
static const uint8_t PIN_DSP_D4 = A3;
static const uint8_t PIN_DSP_D5 = A2;
static const uint8_t PIN_DSP_D6 = A1;
static const uint8_t PIN_DSP_D7 = A0;
// RTC
static const uint8_t PIN_RTC_SDA = A4;
static const uint8_t PIN_RTC_SCL = A5;
// EasyDriver
static const uint8_t PIN_ED_ENABLE = 8;
static const uint8_t PIN_ED_MS1 = 6;
static const uint8_t PIN_ED_MS2 = 7;
static const uint8_t PIN_ED_DIR = 5;
static const uint8_t PIN_ED_STEP = 4;


/*************/
/* Variables */
/*************/

// Object to manage the input buttons and rotary encoder
static SwitchPnl SwitchPanel(PIN_ENC[0], PIN_ENC[1], PIN_BTN_ENT, PIN_BTN_BCK);
// Object to manage time and conversions
static Clock Rtc;
// Object to manage the LCD display
static Lcd Display;

/***********/
/* Methods */
/***********/


void setup()
{
#ifdef DEBUG_SERIAL
  Serial.begin(57600);
#endif

  // Initialize buttons object
  SwitchPanel.init();

  // Initialize display object
  Lcd.init();

  // Initialize RTC and check for errors
  if (Rtc.init())
    Lcd.error("·");

#ifdef DEBUG_SERIAL
  while (!Serial)
    ;
#endif
}

void loop()
{
}
