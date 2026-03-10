#include "communication.h"
#include <Arduino.h>

void InitialiserCommunication()
{
  Serial.begin(9600);
}

void EnvoyerJSON(float temp, float hum, int lum, bool mouv, bool choc)
{
  Serial.print("{ ");
  Serial.print("\"temperature\": ");
  Serial.print(temp);
  Serial.print(", \"humidite\": ");
  Serial.print(hum);
  Serial.print(", \"luminosite\": ");
  Serial.print(lum);
  Serial.print(", \"mouvement\": ");
  Serial.print(mouv);
  Serial.print(", \"choc\": ");
  Serial.print(choc);
  Serial.println(" }");
}

void LireCommande()
{
  if (Serial.available())
  {
    String commande = Serial.readStringUntil('\n');
    Serial.print("Commande reçue : ");
    Serial.println(commande);
  }
}