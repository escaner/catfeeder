#ifndef _WGSELECT_H_
#define _WGSELECT_H_

#include "config.h"
#include <Arduino.h>
#include "widget.h"


/*
 *   Implements a widget to select 2 or 4 options on the display.
 */
class WgSelect: public Widget
{
public:
  WgSelect(LiquidCrystal &Lcd, bool FourOptions);
  virtual void focus();
  virtual int8_t event(const Event &E);

protected:
  // Cursor character
  static const char _Cursor = '*';
  // Coordinates where to draw the cursor in the LCD
  static const uint8_t _Coord2Opt[2][2];
  static const uint8_t _Coord4Opt[4][2];

  // Protected methods
  void _drawCursor() const;
  void _clearCursor() const;
  void _prevOption();
  void _nextOption();

  // Member data
  const uint8_t _NumOptions;  // Whether a 2 or 4 option select
  const uint8_t (* const _CoordOpt)[2];  // One of the _Coord?Opt arrays

  uint8_t _CurOption;  // Currently selected option
};


#endif  // _WGSELECT_H_
