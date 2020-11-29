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
  static const uint8_t _NUM_OPTIONS = 4U;
  static const char _LINES[];

  Page *_pParent;
  WgSelect _Select;
};


#endif  // _PGCONFIG_H_