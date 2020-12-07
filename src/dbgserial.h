#ifndef _DBGSERIAL_H_
#define _DBGSERIAL_H

/*
 *  Macros for debugging
 */

# ifndef NDEBUG_SERIAL
#   define DBGINIT() { Serial.begin(9600); while (!Serial) ; }
#   define DBGPRINT(STR) Serial.print(F(STR))
#   define DBGPRINTLN(STR) Serial.println(F(STR))
# else  // NDEBUG_SERIAL
#   define DBGINIT() ((void) (0))
#   define DBGPRINT(STR) ((void) (0))
#   define DBGPRINTLN(STR) ((void) (0))
# endif  // NDEBUG_SERIAL


#endif  // _DBGSERIAL_H_
