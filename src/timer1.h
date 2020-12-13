#ifndef _TIMER1_H_
#define _TIMER1_H_

#include "config.h"


/*
 *   Handles timer1 and allows switching the Interrupt
 *  Server Routine.
 *   As ISR() macro does not work inside of a class, we need to implement
 *  this as a module.
 */

void enableIsr(void (*pIsrFunction)());
void disableIsr();


#endif  // _TIMER1_H_
