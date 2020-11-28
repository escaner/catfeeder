#ifndef _PGMAIN_H_
#define _PGMAIN_H_

#include "page.h"


/*
 *   Main page of the display. Show current time and next feed time and
 *  whether it is being skipped or not.
 */
class PgMain: public Page
{
public:
  PgMain(LiquidCrystal &Lcd);
  virtual PageAction draw();
  virtual PageAction event(const Event &E);

protected:
  enum State_t: uint8_t
  {
    StOk = 0U,
    StNeedTime = 1U,
    StNeedNextMeal = 2U
  };

  // Protected methods
  void _drawTime(const DateTime &Time) const;
  void _drawNextMeal(const Event::NextMeal_t &NextMeal) const;

  // Member data
  State_t _State;
  bool _ManFeeding;  // Lock page switch while manual feeding happens
  // PgConfig _PgConfig;
};

#endif  // _PGMAIN_H_