#ifndef COMMUNICATION_H
#define COMMUNICATION_H

// Initialise la communication série
void InitialiserCommunication();

// Envoie données au format JSON
void EnvoyerJSON(float temp, float hum, int lum, bool mouv, bool choc);

// Lit commandes entrantes
void LireCommande();

#endif