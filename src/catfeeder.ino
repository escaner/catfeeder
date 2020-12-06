// #define NDEBUG

#include "config.h"
#include "feeds.h"
#include "switchpnl.h"
#include "clock.h"
#include "display.h"
#include "auger.h"
#include "dbgserial.h"


/*************/
/* Constants */
/*************/

// Swtich & encoder constants
static const uint8_t ENC_NUM_PINS = 2U;  // Number of pins per encoder
static const uint8_t NUM_BUTTONS = 2U;  // Number of swtiches of type button

// ARDUINO NANO connections
// Switches
static const uint8_t PIN_ENC[ENC_NUM_PINS] = { 2, 3 };
static const uint8_t PIN_BTN_ENT = 0; /* RX0 */
static const uint8_t PIN_BTN_BCK = 1; /* TX1 */
// Display
static const uint8_t PIN_LCD_RS = 12;
static const uint8_t PIN_LCD_E = 11;
static const uint8_t PIN_LCD_D4 = A3;
static const uint8_t PIN_LCD_D5 = A2;
static const uint8_t PIN_LCD_D6 = A1;
static const uint8_t PIN_LCD_D7 = A0;
// RTC
static const uint8_t PIN_RTC_SDA = A4;
static const uint8_t PIN_RTC_SCL = A5;
// EasyDriver with stepper motor
static const uint8_t PIN_ED_ENABLE = 8;
static const uint8_t PIN_ED_MS1 = 6;
static const uint8_t PIN_ED_MS2 = 7;
static const uint8_t PIN_ED_DIR = 5;
static const uint8_t PIN_ED_STEP = 4;


/*************/
/* Variables */
/*************/

// Object to control feeding times
Feeds FeedData;

// Object to manage the input buttons and rotary encoder
static SwitchPnl SwitchPanel(PIN_ENC[0], PIN_ENC[1], PIN_BTN_ENT, PIN_BTN_BCK);

// Object to manage time and conversions
static Clock Rtc(TIMEZONE_DIFF, ENABLE_DST);

// Object to manage the LCD display
static Display Lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6,
  PIN_LCD_D7);

// Object to control the auger on the EasyDriver stepper motor
static Auger Edsm(PIN_ED_STEP, PIN_ED_DIR, PIN_ED_MS1, PIN_ED_MS2,
  PIN_ED_ENABLE, AUGER_RPM, AUGER_EIGHTH_REVS_PER_MEAL_QTY);

Action A;



/***********/
/* Methods */
/***********/


/*
 *   Sends an event E to the LCD display and handles actions unchained by it.
 *  Parameters:
 *  * E: event to send to the LCD display
 *  Returns:
 *  * false: we are not manually feeding
 *  * true: we are manually feeding
 */
static bool sendEventAndHandleActions(Event E)
{
  bool End = false;
  bool Feeding = false;
  Event E;

  // Loop until no more actions requested or no more events needed
  do
  {
    // Send event and record new requested action
    A = Lcd.event(E);
  
    // Check the requested action, perform it and prepare new event when needed
    switch (A.Id)
    {
    case Action::AcNone:
      End = true;
      break;
    case Action::AcNeedTime:
      E = eventTime();
      break;
    case Action::AcNeedNextMeal:
      E = eventNextMeal();
      break;
    case Action::AcNeedTimeUtc:
      E.Id = Event::EvTimeUtc;
      E.Time = Rtc.getUtc();
      break;
    case Action::AcNeedMeal:
      E.Id = Event::EvMeal;
      E.pMeal = FeedData.getMeal(A.MealId);
      break;
    case Action::AcSetTimeUtc:
      Rtc.setUtc(A.Time);
      End = true;
      break;
    case Action::AcSetMeal:
      FeedData.reset(Rtc.getOfficial());
      End = true;
      break;
    case Action::AcManualFeedStart:
      Edsm.startFeeding();
      Feeding = true;
      break;
    case Action::AcManualFeedContinue:
      Edsm.keepFeeding();
      Feeding = true;
      break;
    case Action::AcManualFeedEnd:
      Edsm.endFeeding();
      // Feeding = false;
      break;
    case Action::AcSkipMeal:
      FeedData.skipNext();
      End = true;
      break;
    case Action::AcUnskipMeal:
      FeedData.unskipNext();
      End = true;
      break;
    }
  } while (!End);

  // Return whether we are manually feeding
  return Feeding;
}


/*
 *   Prepares an event to update the time in the LCD.
 */
static Event eventTime()
{
  // Create an event that contains the official time from the RTC
  Event E(Event::EvTime);
  E.Time = Rtc.getOfficial();

  // Return the event
  return E;
}


/*
 *   Checks whether it is time for a meal, serves it and updates the
 *  next meal in the FeedData object and LCD.
 *  Returns:
 *  * true: when a meal is served next meal needs to be updated in the LCD
 *    in a minute at lease.
 *  * false: no meal was served, no further updates required.
 */
static bool checkFeedTime()
{
  uint8_t Quantity;

  // Get current official time from the RTC and check whether it is meal time
  Quantity = FeedData.check(Rtc.getOfficial());

  // It is meal time when the quantity > 0
  if (Quantity)
  {
    // Deliver meal
    Edsm.feed(Quantity);

    // Update LCD
    sendEventAndHandleActions(eventNextMeal());
  }
}


/*
 *   Prepares an event to update LCD display next meal info
 */
static Event eventNextMeal()
{
  // Create event to update the next meal in the LCD 
  Event E(Event::EvNextMeal);
  E.NextMeal.Status = FeedData.timeOfNext(&E.NextMeal.Dotw, &E.NextMeal.Hour,
    &E.NextMeal.Minute);

  // Return the event
  return E;
}


/*
 *   Arduino initialization function.
 */
void setup()
{
  DBGINIT();

  // Initialize buttons object
  SwitchPanel.init();

  // Initialize RTC and check for errors
  if (Rtc.init())
  {
    Lcd.error("Rtc.init()");
    abort();
  }

  // Send event to initialize display
  sendEventAndHandleActions(Lcd.event(Event(Event::EvInit)));
}


/*
 *   Arduino loop function.
 */
void loop()
{
  static unsigned long LastCheckFeed = 0UL;
  static unsigned long LastUpdateTime = 0UL;
  static unsigned long LastMealTime = 0UL;
  bool UpdateMealTime = false;
  unsigned long CurTime;

  // Get current time
  CurTime = millis();

  // Check update time (counter overflow works well)
  if (CurTime - LastUpdateTime >= TIME_UPDATE_INTERVAL)
  {
    LastUpdateTime = CurTime;
    sendEventAndHandleActions(eventTime());
  }

  // Check feed time every FEED_CHECK_INTERVAL ms (counter overflow works well)
  if (CurTime - LastCheckFeed >= FEED_CHECK_INTERVAL)
  {
    LastCheckFeed = CurTime;
    if (checkFeedTime())
    {
      LastMealTime = CurTime;
      UpdateMealTime = true;
    }
  }

  // MEAL_UPDATE_DELAY ms after serving a meal, the LCD next meal is updated
  if (UpdateMealTime && CurTime - LastMealTime >= MEAL_UPDATE_DELAY)
  {
    UpdateMealTime = false;
    sendEventAndHandleActions(eventNextMeal());
  }

  // Loop checking switch panel
  for (uint16_t SwIdx=SWITCH_LOOP_COUNT; SwIdx; SwIdx--)
  {
    Event::SwitchEvent SwE;
    bool ManuallyFeeding = false;

    // Loop for manual feeding (synchronous interactive task)
    do
    {
      // Did we get a switch event?
      if ((SwE = SwitchPanel.check()) != Event::SwEvNone)
      {
        // Build event
        Event E(Event::EvSwitch);
        E.Switch = SwE;

        // Notify event and handle unchained actions
        ManuallyFeeding = sendEventAndHandleActions(Lcd.event(E));
      }
      else if (ManuallyFeeding)
        // Keep feeding when no switches registered
        Edsm.keepFeeding();
    }
    while (ManuallyFeeding);
  }
}
