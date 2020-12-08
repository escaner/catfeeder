#ifndef _PGMEAL_H_
#define _PGMEAL_H_

#include "config.h"
#include <Arduino.h>
#include "config.h"
#include "page.h"
#include "wgint.h"
#include "wgabool.h"
#include "dotwtext.h"


/*
 *   Class to create a page in the display to visualize and configure
 *  the meal times.
 */
class PgMeal: public Page
{
public:
  PgMeal(Page *pParent, LiquidCrystal &Lcd);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  // Type for indexing the widgets
  enum WgId_t: int8_t
  {
    WgDotw=0, WgHour, WgMinute, WgQuantity, WgMeal
  };

  // To keep track of the page initialization state
  enum State_t: uint8_t
  {
    StOk = 0U,
    StNeedMeal = 1U
  };

  // Static constants

  // How many widgets a meal has (not including meal selector)
  static const uint8_t _NUM_WIDGETS_TIME = 4U;

  // Widget value limits
  static const uint8_t _MIN_MEALID = 0U;
  static const uint8_t _MAX_MEALID = _MIN_MEALID + NUM_MEALS - 1U ;
  static const uint8_t _MIN_HOUR = 0U;
  static const uint8_t _MAX_HOUR = 23U;
  static const uint8_t _MIN_MINUTE = 0U;
  static const uint8_t _MAX_MINUTE = 59U;
  static const uint8_t _MIN_QUANTITY = 0U;
  static const uint8_t _MAX_QUANTITY = 9U;

  // Positions of widgets and tags
  static const uint8_t _MEAL_ROW = 0U;
  static const uint8_t _MEAL_MEAL_COL = 7U;
  static const uint8_t _MEAL_MEAL_SIZE = 1U;
  static const uint8_t _MEAL_DOTW_COL = 9U;
  static const uint8_t _TIME_ROW = 1U;
  static const uint8_t _TIME_HOUR_COL = 0U;
  static const uint8_t _TIME_HOUR_SIZE = 2U;
  static const uint8_t _TIME_MINUTE_COL = 3U;
  static const uint8_t _TIME_MINUTE_SIZE = 2U;
  static const uint8_t _TIME_QUANTITY_COL = 15U;
  static const uint8_t _TIME_QUANTITY_SIZE = 1U;

  // Static tags in the display
  static const char _LINES[DISPLAY_ROWS][DISPLAY_COLS+1];

  // Arrays with single letter representations of indexes
  char _DOTW_CHAR_FALSE[DotwText::DAYS_IN_A_WEEK];
  char _DOTW_CHAR_TRUE[DotwText::DAYS_IN_A_WEEK];

  // Protected methods
  void _init(Meal *pMeal);
  PageAction _makeNeedMeal(uint8_t MealId);
  PageAction _makeSetMeal() const;
  void _focusMealWidget();
  void _focusTimeWidgets();
  void _focusNextTimeWidget();
  void _rearrangeDotwToEn(bool *pDst, const bool *pSrc) const;
  void _rearrangeDotwFromEn(bool *pDst, const bool *pSrc) const;

  // Member data
  State_t _State;     // Current initialization state
  Page *_pParent;     // Parent page
  bool _Initialized;  // Wether the class has been initialized
  // Widgets in this page
  WgAbool _WgDotw;
  WgInt _WgMeal;  // Meal Id widget
  WgInt _WgHour;
  WgInt _WgMinute;
  WgInt _WgQuantity;
  // Values for Widgets
  uint16_t _ValMeal;  // Meal Id
  uint16_t _ValHour;
  uint16_t _ValMinute;
  uint16_t _ValQuantity;
  bool _ValDotw[DotwText::DAYS_IN_A_WEEK];
  Widget *_Widgets[_NUM_WIDGETS_TIME+1];  // For easy management of widgets
  WgId_t _FocusWidget;  // Which widget has the focus
  Meal *_pMeal;  // The meal that we are displaying and modifying
};


#endif  // _PGMEAL_H_
