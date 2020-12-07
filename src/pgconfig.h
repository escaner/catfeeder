#ifndef _PGCONFIG_H_
#define _PGCONFIG_H_

#include "config.h"
#include <Arduino.h>
#include "page.h"
#include "pgtime.h"
#include "pgmeal.h"
#include "wgselect.h"


/*
 *   Config page of the display. Show current time and next feed time and
 *  whether it is being skipped or not.
 */
class PgConfig: public Page
{
public:
  PgConfig(Page *pParent, LiquidCrystal &Lcd);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  // Static constants
  static const uint8_t _NUM_OPTIONS = 4U;
  static const char *const _LINES[];

  // Member data
  Page *_pParent;    // Parent page
  WgSelect _Select;  // Widget in this page
  PgMeal _PgMeal;    // Page instance for meal configuration
  PgTime _PgTime;    // Page instance for time configuration
};


#endif  // _PGCONFIG_H_