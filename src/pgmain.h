#ifndef _PGMAIN_H_
#define _PGMAIN_H_

#include "config.h"
#include <Arduino.h>
#include "pgconfig.h"


/*
 *   Main page of the display. Show current time and next feed time and
 *  whether it is being skipped or not.
 */
class PgMain: public Page
{
public:
  PgMain(LiquidCrystal &Lcd);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  enum State_t: uint8_t
  {
    StOk = 0U,
    StNeedTime = 1U,
    StNeedNextMeal = 2U
  };

  static const uint8_t _TIME_COL = 0U;
  static const uint8_t _TIME_ROW = 0U;
  static const uint8_t _NEXTMEAL_COL = 0U;
  static const uint8_t _NEXTMEAL_ROW = 1U;
  static const uint8_t _NEXTMEAL_STATUS_SIZE = 5U;
  static const char _LINE0[] PROGMEM;
  static const char _LINE1[] PROGMEM;
  static const char _STATUS_TEXT[][_NEXTMEAL_STATUS_SIZE+1U];

  // Protected methods
  void _drawTime(const DateTime &Time) const;
  void _drawNextMeal(const Event::NextMeal_t &NextMeal) const;

  // Member data
  State_t _State;      // Current initialization state
  bool _ManFeeding;    // Lock page switch while manual feeding happens
  PgConfig _PgConfig;  // Config page instance
};

#endif  // _PGMAIN_H_