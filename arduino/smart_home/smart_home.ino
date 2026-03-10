/**************************************************************
 * PROJET DOMOTIQUE - MAIN
 * 
 * Ce fichier orchestre tout le système :
 * - Initialise modules
 * - Lit capteurs
 * - Envoie données
 * - Vérifie commandes
 **************************************************************/

#include "capteurs.h"
#include "actionneurs.h"
#include "communication.h"

void setup() {

  InitialiserCapteurs();
  InitialiserActionneurs();
  InitialiserCommunication();
}

void loop() {

  // 1️ Lire capteurs
  float temp = LireTemperature();
  float hum = LireHumidite();
  int lum = LireLuminosite();
  bool mouv = DetecterMouvement();
  bool choc = DetecterChoc();

  // 2️ Envoyer données
  EnvoyerJSON(temp, hum, lum, mouv, choc);

  // 3️ Vérifier commandes
  LireCommande();

  delay(5000);  // Attendre 5 secondes
}