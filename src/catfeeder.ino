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


/***********/
/* Methods */
/***********/


void setup()
{
#ifdef DEBUG_SERIAL
  Serial.begin(57600);
#endif

#ifdef DEBUG_SERIAL
  while (!Serial)
    ;
#endif
}

void loop()
{
}
