
/***********/
/* Methods */
/***********/


void setup()
{
#ifdef DEBUG_SERIAL
  Serial.begin(57600);
#endif

#ifdef DEBUG_SERIAL
  while (!Serial)
    ;
#endif
}

void loop()
{
}
