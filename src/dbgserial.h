#ifndef _DBGSERIAL_H_
#define _DBGSERIAL_H

/*
 *  Macros for debugging
 */

# ifndef NDEBUG
#   define DBGINIT() { Serial.begin(9600); while (!Serial) ; }
#   define DBGPRINT(STR) Serial.print(STR)
#   define DBGPRINTLN(STR) Serial.println(STR)
# else  // NDEBUG
#   define DBGINIT() ((void) (0))
#   define DBGPRINT(STR) ((void) (0))
#   define DBGPRINTLN(STR) ((void) (0))
# endif  // NDEBUG


#endif  // _DBGSERIAL_H_
