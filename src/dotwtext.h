#ifndef _DOTWTEXT_H_
#define _DOTWTEXT_H_


/*
 *   Static class to translate day of the week indexes into Spansh text.
 *  Defaults to index 0 representing Sunday. For localization purposes,
 *  constants with the local first day of the week are provided.
 */
class DotwText
{
public:
  // Public Constants
  static const uint8_t DAYS_IN_A_WEEK = 7U;
  static const uint8_t FIRST_DOTW_ES = 1U;
  static const uint8_t FIRST_DOTW_EN = 0U;

  // Arrays with single letter representations of indexes
  static const char DotwCharEn[DAYS_IN_A_WEEK];
  static const char DotwCharEs[DAYS_IN_A_WEEK];

  // Methods
  static inline uint8_t incr(uint8_t &DotwId, uint8_t Increment);
  static inline uint8_t add(uint8_t DotwId, uint8_t Increment);
};


#endif  // _DOTWTEXT_H_
