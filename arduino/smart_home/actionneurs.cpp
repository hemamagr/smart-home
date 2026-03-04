#include "actionneurs.h"
#include <Arduino.h>

/******** PINS ********/

#define PIN_LED 9
#define PIN_BUZZER 10
#define PIN_VENTILATEUR 11
#define PIN_CHAUFFAGE 12

void InitialiserActionneurs()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_VENTILATEUR, OUTPUT);
  pinMode(PIN_CHAUFFAGE, OUTPUT);
}

/******** LED ********/

void AllumerLED()
{
  digitalWrite(PIN_LED, HIGH);
}

void EteindreLED()
{
  digitalWrite(PIN_LED, LOW);
}

/******** BUZZER ********/

void ActiverBuzzer()
{
  tone(PIN_BUZZER, 2000);
}

void DesactiverBuzzer()
{
  noTone(PIN_BUZZER);
}

/******** VENTILATEUR ********/

void ActiverVentilateur()
{
  digitalWrite(PIN_VENTILATEUR, HIGH);
}

void DesactiverVentilateur()
{
  digitalWrite(PIN_VENTILATEUR, LOW);
}

/******** CHAUFFAGE ********/

void ActiverChauffage()
{
  digitalWrite(PIN_CHAUFFAGE, HIGH);
}

void DesactiverChauffage()
{
  digitalWrite(PIN_CHAUFFAGE, LOW);
}