#include "capteurs.h"
#include <Arduino.h>

/******** PINS ********/

#define PIN_TEMP A0
#define PIN_HUM  A1
#define PIN_LUM  A2
#define PIN_MOUV 7
#define PIN_CHOC 6

void InitialiserCapteurs()
{
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_HUM, INPUT);
  pinMode(PIN_LUM, INPUT);
  pinMode(PIN_MOUV, INPUT);
  pinMode(PIN_CHOC, INPUT);
}

/******** LECTURES ********/

float LireTemperature()
{
  int valeur = analogRead(PIN_TEMP);
  float voltage = valeur * (5.0 / 1023.0);
  float temperature = voltage * 100;   // simulation
  return temperature;
}

float LireHumidite()
{
  int valeur = analogRead(PIN_HUM);
  float humidite = (valeur / 1023.0) * 100;
  return humidite;
}

int LireLuminosite()
{
  return analogRead(PIN_LUM);
}

bool DetecterMouvement()
{
  return digitalRead(PIN_MOUV);
}

bool DetecterChoc()
{
  return digitalRead(PIN_CHOC);
}