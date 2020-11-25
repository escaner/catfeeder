#ifndef _Display_H_
#define _Display_H_

#include <LiquidCrystal.h>


/*
 *   Class to interface with the LCD display and manage all the menus.
 */
class Display
{
public:
  // Display constants
  static const uint8_t COLS = 16U;
  static const uint8_t ROWS = 2U;

  Display(uint8_t PinRs, uint8_t PinEnable, uint8_t PinD4, uint8_t PinD5,
    uint8_t PinD6, uint8_t PinD7);
  void init();

protected:
  LiquidCrystal Lcd;
};


#endif  // _DISPLAY_H_
