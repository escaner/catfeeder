#include <cassert>
#include "pgmeal.h"


/********************/
/* Member constants */
/********************/

// Static tags in the display
const char PgMeal::_MEAL_TAG[] = "COMIDA#";
const char PgMeal::_TIME_TAG[] = "CANTIDAD";

// Arrays with single letter representations of the days ofthe week
const char PgMeal::_DOTW_CHAR_FALSE[DotwText::DAYS_IN_A_WEEK] =
  { 'l', 'm', 'x', 'j', 'v', 's', 'd' };
const char PgMeal::_DOTW_CHAR_TRUE[DotwText::DAYS_IN_A_WEEK] =
  { 'L', 'M', 'X', 'J', 'V', 'S', 'D' };


/***********/
/* Methods */
/***********/

/*
 *   Constructor. Initializes class and widgets.
 *  Parameters:
 *  * pParent: parent Page where to return the focus on exit
 *  * Lcd: reference to the lcd display that is being used.
 *  * Cols: number of columns in the LCD.
 *  * Rows: number of rows in the LCD.
 */
PgMeal::PgMeal(Page *pParent, LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows):
  Page(Lcd, Cols, Rows),
  _State(StOk),
  _pParent(pParent),
  _Initialized(false),
  _WgDotw(Lcd, Cols, Rows, _MEAL_DOTW_COL, _MEAL_ROW, _DOTW_CHAR_TRUE,
    _DOTW_CHAR_FALSE, DotwText::DAYS_IN_A_WEEK),
  _WgMeal(Lcd, Cols, Rows, _MEAL_MEAL_COL, _MEAL_ROW, _MEAL_MEAL_SIZE),
  _WgHour(Lcd, Cols, Rows, _TIME_HOUR_COL, _TIME_ROW, _TIME_HOUR_SIZE),
  _WgMinute(Lcd, Cols, Rows, _TIME_MINUTE_COL, _TIME_ROW, _TIME_MINUTE_SIZE),
  _WgQuantity(Lcd, Cols, Rows, _TIME_QUANTITY_COL, _TIME_ROW,
    _TIME_QUANTITY_SIZE)
{
  // Array for easy management of the time widgets
  _Widgets[WgDotw] = &_WgDotw;
  _Widgets[WgHour] = &_WgHour;
  _Widgets[WgMinute] = &_WgMinute;
  _Widgets[WgQuantity] = &_WgQuantity;
  _Widgets[WgMeal] = &_WgMeal;
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
    // Update state
    _State = StNeedMeal;

    // Request first meal
    PgAc.Action.Id = Action::AcNeedMeal;
    PgAc.Action.MealId = 0U;
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
  bool _DotwEn[DotwText::DAYS_IN_A_WEEK];

  switch (E.Id)
  {
  case Event::EvInit:
    // Initialize: display page for the first time
    return focus();

  case Event::EvSwitch:  // Switch has been pressed / released / something.
    uint16_t MealId = _ValMeal;  // Save meal id to later check if changed

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

        // Update Meal
        _pMeal->setTime(_ValHour, _ValMinute);
        _rearrangeDotwToEn(_DotwEn, _ValDotw);
        _pMeal->setDotw(_DotwEn);
        _pMeal->setQuantity(_ValQuantity);

        // Notify of the update
        Action A(Action::AcSetMeal);
        A.MealId = _ValMeal;
        return PageAction(A);
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

        // Update state
        _State = StNeedMeal;

        // Request new meal data
        PageAction PgAc(Action::AcNeedMeal);
        PgAc.Action.MealId = _ValMeal;
        return PgAc;
      }
      // No action
      break;
    }
    break;

  case Event::EvMeal:
    // Check if we requested it from focus() or from a change in the meal widget
    if (_State == StNeedMeal)
      // Yes, all data received, we can finish initialization
      _State = StOk;
    // Initialize page and widgets
    _init(E.pMeal);
    // We don't need any more data -> return no action
    break;
  }

  // Default: no action
  return PageAction();
}

/*
 *   Initializes page and widgets or just updates values if alredy initialized.
 *  Parameters:
 *  * Time: date and time in UTC to initialize values.
 */
void PgMeal::_init(const Meal *pMeal)
{
  uint8_t Hour, Minute;
  bool DotwEn[DotwText::DAYS_IN_A_WEEK];

  // Set time values
  pMeal->getTime(&Hour, &Minute);
  _ValHour = (uint16_t) Hour;
  _ValMinute = (uint16_t) Minute;
  _ValQuantity = pMeal->getQuantity();
  pMeal->getDotw(DotwEn);
  _rearrangeDotwFromEn(_ValDotw, DotwEn);

  // If the object was never initialized, do it
  if (!_Initialized)
  {
    // Initialize the object
    _Initialized = true;

    // Set value for the meal id
    _ValMeal = 0U;
  
    // Display tags in the LCD
    _Lcd.setCursor(_MEAL_TAG_COL, _MEAL_ROW);
    _Lcd.write(_MEAL_TAG);
    _Lcd.setCursor(_TIME_TAG_COL, _TIME_ROW);
    _Lcd.write(_TIME_TAG);

    // Initialize widgets
    _WgMeal.init(_MIN_MEALID, _MAX_MEALID, &_ValMeal);
    _WgDotw.init(_ValDotw);
    _WgHour.init(_MIN_HOUR, _MAX_HOUR, &_ValHour);
    _WgMinute.init(_MIN_MINUTE, _MAX_MINUTE, &_ValMinute);
    _WgQuantity.init(_MIN_QUANTITY, _MAX_QUANTITY, &_ValQuantity);

    // Set focus on meal id
    _FocusWidget = WgMeal;
    _WgMeal.focus();
  }
}


  void _focusMealWidget();
  void _focusCfgWidgets();
  void _focusNexCfgtWidget();

  void _rearrangeDotwToEn(bool *pDst, const bool *pSrc) const;
  void _rearrangeDotwFromEn(bool *pDst, const bool *pSrc) const;
