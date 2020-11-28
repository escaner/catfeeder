#ifndef _Display_H_
#define _Display_H_

#include <LiquidCrystal.h>
#include "page.h"
#include "pgmain.h"


/*
 *   Class to interface with the LCD display and manage all the pages and menus.
 */
class Display
{
public:
  Display(uint8_t PinRs, uint8_t PinEnable, uint8_t PinD4, uint8_t PinD5,
    uint8_t PinD6, uint8_t PinD7);
  Action event(const Event &E);

protected:
  // Display constants
  static const uint8_t _COLS = 16U;
  static const uint8_t _ROWS = 2U;

  // Member data
  LiquidCrystal _Lcd;  // LCD control class
  PgMain _PgMain;        // Main page
  Page *_pFocusPage;   // Page currently having the focus to pass events
};


#endif  // _DISPLAY_H_
