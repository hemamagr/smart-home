#ifndef CAPTEURS_H
#define CAPTEURS_H

// Initialise tous les capteurs
void InitialiserCapteurs();

// Fonctions de lecture
float LireTemperature();
float LireHumidite();
int LireLuminosite();
bool DetecterMouvement();
bool DetecterChoc();

#endif