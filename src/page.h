#ifndef _PAGE_H_
#define _PAGE_H_

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "event.h"
#include "action.h"


class Page;


/*
 *   Data structure to handle actions within the Display and Page area.
 *  Is is basically the standard Action plus a focus management Page pointer.
 */
class PageAction
{
public:
  // Constructors
  PageAction(): pFocusPage(nullptr), MainAction() {}
  PageAction(Action::ActionId Id): pFocusPage(nullptr), MainAction(Id) {}
  // PageAction(const Action &Ac): pFocusPage(nullptr), MainAction(Ac) {}
  PageAction(Page *pPage): pFocusPage(pPage), MainAction() {}
  PageAction(Page *pPage, const Action &A): pFocusPage(pPage), MainAction(A) {}

  Page *pFocusPage;  // nullprt or new focus Page
  Action MainAction;
};


/*
 *   Abstract class to handle the display pages and widgets.
 */
class Page
{
public:
  // Constructor definition
  Page(LiquidCrystal &Lcd):
    _Lcd(Lcd)
  {
  }

  // Abstract virtual methods
  virtual PageAction focus() = 0;
  virtual PageAction event(const Event &E) = 0;

protected:
  // Member data
  LiquidCrystal &_Lcd;
};

#endif  // _PAGE_H_
