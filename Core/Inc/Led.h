#ifndef LED_H
#define LED_H

typedef enum{
  ON,OFF
} OnOffState;

void turnLed(OnOffState state);
#endif // LED_H
