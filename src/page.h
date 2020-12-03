#ifndef _PAGE_H_
#define _PAGE_H_

#include <LiquidCrystal.h>
#include "event.h"
#include "action.h"


/*
 *   Data structure to handle actions within the Display and Page area.
 *  Is is basically the standard Action plus a focus management Page pointer.
 */
class PageAction
{
public:
  // Constructors
  PageAction(): pFocusPage(nullptr), Action() {}
  PageAction(Action::ActionId Id): pFocusPage(nullptr), Action(Id) {}
  // PageAction(const Action &Ac): pFocusPage(nullptr), Action(Ac) {}
  PageAction(Page *pPage): pFocusPage(pPage), Action() {}
  PageAction(Page *pPage, const Action &A): pFocusPage(pPage), Action(A) {}

  Page *pFocusPage;  // nullprt or new focus Page
  Action Action;
};


/*
 *   Abstract class to handle the display pages and widgets.
 */
class Page
{
public:
  // Constructor definition
  Page(LiquidCrystal &Lcd, uint8_t Cols, uint8_t Rows):
    _Lcd(Lcd),
    _COLS(Cols),
    _ROWS(Rows)
  {
  }

  // Abstract virtual methods
  virtual PageAction focus() = 0;
  virtual PageAction event(const Event &E) = 0;

protected:
  // Member data
  const uint8_t _COLS;
  const uint8_t _ROWS;
  LiquidCrystal &_Lcd;
};

#endif  // _PAGE_H_
