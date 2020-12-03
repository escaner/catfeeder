#ifndef _PGCONFIG_H_
#define _PGCONFIG_H_

#include "page.h"
#include "wgselect.h"


/*
 *   Config page of the display. Show current time and next feed time and
 *  whether it is being skipped or not.
 */
class PgConfig: public Page
{
public:
  PgConfig(Page *pParent, LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows);
  virtual PageAction focus();
  virtual PageAction event(const Event &E);

protected:
  // Static constants
  static const uint8_t _NUM_OPTIONS = 4U;
  static const char _LINES[];

  // Member data
  Page *_pParent;    // Parent page
  WgSelect _Select;  // Widget in this page
};


#endif  // _PGCONFIG_H_