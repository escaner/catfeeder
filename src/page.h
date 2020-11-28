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
  PageAction(Page *pPage): pFocusPage(pPage), Action() {}

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
  Page(LiquidCrystal &Lcd): _Lcd(Lcd) {}

  // Abstract virtual methods
  virtual PageAction draw() = 0;
  virtual PageAction event(const Event &E) = 0;

protected:
  // Member data
  LiquidCrystal &_Lcd;
};

#endif  // _PAGE_H_
