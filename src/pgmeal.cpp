#include "config.h"
#include <assert.h>
#include "pgmeal.h"


/********************/
/* Member constants */
/********************/

// Static tags in the display
const char PgMeal::_LINE0[DISPLAY_COLS+1] PROGMEM = "COMIDA#         ";
const char PgMeal::_LINE1[DISPLAY_COLS+1] PROGMEM = "  :   CANTIDAD  ";
const char *const PgMeal::_LINES[DISPLAY_ROWS] PROGMEM = { _LINE0, _LINE1 };


/***********/
/* Methods */
/***********/

/*
 *   Constructor. Initializes class and widgets.
 *  Parameters:
 *  * pParent: parent Page where to return the focus on exit
 *  * Lcd: reference to the lcd display that is being used.
 */
PgMeal::PgMeal(Page *pParent, LiquidCrystal &Lcd):
  Page(Lcd),
  _State(StOk),
  _pParent(pParent),
  _Initialized(false),
  _WgDotw(Lcd, _MEAL_DOTW_COL, _MEAL_ROW, _DOTW_CHAR_TRUE, _DOTW_CHAR_FALSE,
    DotwText::DAYS_IN_A_WEEK),
  _WgMeal(Lcd, _MEAL_MEAL_COL, _MEAL_ROW, _MEAL_MEAL_SIZE),
  _WgHour(Lcd, _TIME_HOUR_COL, _TIME_ROW, _TIME_HOUR_SIZE),
  _WgMinute(Lcd, _TIME_MINUTE_COL, _TIME_ROW, _TIME_MINUTE_SIZE),
  _WgQuantity(Lcd, _TIME_QUANTITY_COL, _TIME_ROW, _TIME_QUANTITY_SIZE)
{
  // Array for easy management of the time widgets
  _Widgets[WgDotw] = &_WgDotw;
  _Widgets[WgHour] = &_WgHour;
  _Widgets[WgMinute] = &_WgMinute;
  _Widgets[WgQuantity] = &_WgQuantity;
  _Widgets[WgMeal] = &_WgMeal;

  // Prepare the character representations of the DOTW in ES locale
  for (uint8_t Dotw=0U; Dotw<DotwText::DAYS_IN_A_WEEK; Dotw++)
  {
    uint8_t DotwEn = DotwText::add(Dotw, 1U);
    _DOTW_CHAR_TRUE[Dotw] = toupper(DotwText::DotwCharEs[DotwEn]);
    _DOTW_CHAR_FALSE[Dotw] = tolower(DotwText::DotwCharEs[DotwEn]);
  }
}


/*
 *   Starts a series of actions and events to draw this page in the display.
 *  It first requests the first meal data. Uses _State to track the sequence of
 *  events.
 *  Returns:
 *  * PageAction requesting the first meal or no action when
 *    the process was already unchained and not finished.
 */
PageAction PgMeal::focus()
{
  PageAction PgAc;

  // Only makes sense at StOk
  if (_State == StOk)
  {
    // Not initialized: need to draw page and set values to widgets
    _Initialized = false;

    // Request first meal (#0)
    _ValMeal = 0U;
    PgAc = _makeNeedMeal(_ValMeal);
  }

  return PgAc;
}


/*
 *   Manage events received by page.
 *  Parameters:
 *  * E: event data
 *  Returns:
 *  * PageAction with instructions on how to proceed.
 */
PageAction PgMeal::event(const Event &E)
{
  uint16_t MealId;

#pragma GCC diagnostic push
// Disable: warning: enumeration value not handled in switch
#pragma GCC diagnostic ignored "-Wswitch"

  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return focus();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    MealId = _ValMeal;  // Save meal id to later check if changed

    // Pass event to current widget with the focus
    switch (_Widgets[_FocusWidget]->event(E))
    {
    case Widget::AcBack:
      // Meal or one of the time widgets?
      if (_FocusWidget == WgMeal)
      {
        // Meal: go to previous page
        return PageAction(_pParent);
      }
      else
      {
        // Time: go back to Meal widget
        _focusMealWidget();

        // Request a set meal
        return _makeSetMeal();
      }
      break;
    case Widget::AcOk:
      // Meal or one of the time widgets?
      if (_FocusWidget == WgMeal)
      {
        // Meal: move focus to first time widget and return no action
        _focusTimeWidgets();
      }
      else
      {
        // Time: move focus to next widget
        _focusNextTimeWidget();
      }
      // No action
      break;
    case Widget::AcNone:
      // Has the meal id changed? (only possible when at meal widget)
      if (MealId != _ValMeal)
      {
        assert(_FocusWidget == WgMeal);
        assert(_State == StOk);

        // Request selected meal id
        return _makeNeedMeal(_ValMeal);
      }
      // No action
      break;
    }
    break;

  case Event::EvMeal:
    // Check if we requested it from focus() or from a change in the meal widget
    if (_State == StNeedMeal)
    {
      // Yes, all data received, we can finish initialization
      _State = StOk;
      // Initialize page and widgets
      _init(E.pMeal);
    }
    // We don't need any more data -> return no action
    break;
  }

#pragma GCC diagnostic pop

  // Default: no action
  return PageAction();
}


/*
 *   Initializes page and widgets or just updates values if alredy initialized.
 *  Parameters:
 *  * Time: date and time in UTC to initialize values.
 */
void PgMeal::_init(Meal *pMeal)
{
  uint8_t Hour, Minute;
  bool DotwEn[DotwText::DAYS_IN_A_WEEK];

  // Save pointer to the meal
  _pMeal = pMeal;

  // _ValMeal value already set

  // Set meal time values
  pMeal->getTime(&Hour, &Minute);
  _ValHour = (uint16_t) Hour;
  _ValMinute = (uint16_t) Minute;
  _ValQuantity = pMeal->getQuantity();
  pMeal->getDotw(DotwEn);
  _rearrangeDotwFromEn(_ValDotw, DotwEn);

  // If the object was not initialized since focus(), draw the page
  if (!_Initialized)
  {
    // Initialize the object
    _Initialized = true;

    // Draw page
    _Lcd.setCursor(0, _MEAL_ROW);
    _Lcd.print((const __FlashStringHelper *) pgm_read_ptr(_LINES + _MEAL_ROW));
    _Lcd.setCursor(0, _TIME_ROW);
    _Lcd.print((const __FlashStringHelper *) pgm_read_ptr(_LINES + _TIME_ROW));
  }

  // Initialize and draw widgets
  _WgMeal.init(_MIN_MEALID, _MAX_MEALID, &_ValMeal);
  _WgDotw.init(_ValDotw);
  _WgHour.init(_MIN_HOUR, _MAX_HOUR, &_ValHour);
  _WgMinute.init(_MIN_MINUTE, _MAX_MINUTE, &_ValMinute);
  _WgQuantity.init(_MIN_QUANTITY, _MAX_QUANTITY, &_ValQuantity);

  // Set focus on meal id
  _focusMealWidget();
}


/*
 *   Configures the class to wait for an event with meal data and prepares
 *  the PageAction object with that request to return.
 *  Parameters:
 *  * MealId: which meal we want.
 *  Returns: the need meal action request.
 */
PageAction PgMeal::_makeNeedMeal(uint8_t MealId)
{
  // Update state: waiting for meal event
  _State = StNeedMeal;

  // Create action to request new meal data
  PageAction PgAc(Action::AcNeedMeal);
  PgAc.MainAction.MealId = MealId;

  return PgAc;
}


/*
 *   Updates the Meal through the _pMeal pointer and prepares the PageAction
 *  object with a request of update to return.
 *  Returns: the set meal action request.
 */
PageAction PgMeal::_makeSetMeal() const
{
  bool _DotwEn[DotwText::DAYS_IN_A_WEEK];

Serial.println(F("PgMeal::_makeSetMeal"));
Serial.flush();

  // Update _Meal object with time widget values
  _pMeal->setTime(_ValHour, _ValMinute);
  _rearrangeDotwToEn(_DotwEn, _ValDotw);
  _pMeal->setDotw(_DotwEn);
  _pMeal->setQuantity(_ValQuantity);

  // Create action to notify of the update
  PageAction PgAc(Action::AcSetMeal);
  PgAc.MainAction.MealId = _ValMeal;

  return PgAc;
}


/*
 *   Moves focus from a time widget to the meal widget.
 */
void PgMeal::_focusMealWidget()
{
  _FocusWidget = WgMeal;
  _WgMeal.focus();
}


/*
 *   Moves focus from  the meal widget to the time widgets.
 */
void PgMeal::_focusTimeWidgets()
{
  // First widget in time set is Dotw
  _FocusWidget = WgDotw;
  _WgDotw.focus();
}


/*
 *   Moves focus from a time widget to the next one.
 */
void PgMeal::_focusNextTimeWidget()
{
  // Update widget id; turn back to 0 after the last one
  _FocusWidget =
    WgId_t((uint8_t(_FocusWidget) + uint8_t(1U)) % _NUM_WIDGETS_TIME);
  _Widgets[_FocusWidget]->focus();
}


/*
 *   Rearranges a bool array with days of the week from EN to ES order.
 *  Parameters:
 *  * pDst: destination array, with first day being Sunday.
 *  * pSrc: source array, with first day being Monday.
 */
void PgMeal::_rearrangeDotwToEn(bool *pDst, const bool *pSrc) const
{
Serial.println(F("rearrange->En"));
Serial.print(F("src:"));
for (int i=0; i<DotwText::DAYS_IN_A_WEEK; i++)
Serial.print(pSrc[i]);
Serial.println();

  // Copy all the correlative ones
  memcpy(pDst+1, pSrc, (DotwText::DAYS_IN_A_WEEK - 1U) * sizeof (bool));

  // Copy the missing one
  pDst[0] = pSrc[DotwText::DAYS_IN_A_WEEK - 1U];
Serial.print(F("Dst:"));
for (int i=0; i<DotwText::DAYS_IN_A_WEEK; i++)
Serial.print(pDst[i]);
Serial.println();
}


/*
 *   Rearranges a bool array with days of the week from ES to EN order.
 *  Parameters:
 *  * pDst: destination array, with first day being Monday.
 *  * pSrc: source array, with first day being Sunday.
 */
void PgMeal::_rearrangeDotwFromEn(bool *pDst, const bool *pSrc) const
{
Serial.println(F("rearrange<-En"));
Serial.println(F("src:"));
for (int i=0; i<DotwText::DAYS_IN_A_WEEK; i++)
Serial.print(pSrc[i]);
Serial.println();
  // Copy all the correlative ones
  memcpy(pDst, pSrc+1, (DotwText::DAYS_IN_A_WEEK - 1U) * sizeof (bool));

  // Copy the missing one
  pDst[DotwText::DAYS_IN_A_WEEK - 1U] = pSrc[0];
Serial.println(F("Dst:"));
for (int i=0; i<DotwText::DAYS_IN_A_WEEK; i++)
Serial.print(pDst[i]);
Serial.println();
}
