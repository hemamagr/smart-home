#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// --- CONFIGURATION DES BROCHES (PINS) ---
#define DHTPIN A0       // Capteur Température/Humidité
#define DHTTYPE DHT11
#define LUMINOSITE_PIN A2 // Capteur de lumière (Analogique)
#define MOUVEMENT_PIN 7   // Capteur PIR (Digital)
#define CHOC_PIN 6        // Capteur de choc (Digital)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); // Ouvre le tunnel vers ton script Python
  
  // Initialisation des capteurs
  dht.begin();
  pinMode(MOUVEMENT_PIN, INPUT);
  pinMode(CHOC_PIN, INPUT);
  
  // Petit message de démarrage pour le débug
  // (le script Python ignorera cette ligne car ce n'est pas du JSON)
  Serial.println("Système Domotique Initialisé...");
}

// ---  FONCTION DE COMMUNICATION ---
void EnvoyerJSON(float temp, float hum, int lum, bool mouv, bool choc) {
  Serial.print("{ ");
  Serial.print("\"temperature\": "); Serial.print(temp);
  Serial.print(", \"humidite\": "); Serial.print(hum);
  Serial.print(", \"luminosite\": "); Serial.print(lum);
  
  // On transforme 0/1 en vrai texte JSON true/false
  Serial.print(", \"mouvement\": "); Serial.print(mouv ? "true" : "false");
  Serial.print(", \"choc\": "); Serial.print(choc ? "true" : "false");
  Serial.println(" }"); 
}

void loop() {
  // 1. LECTURE DES DONNÉES REELLES
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int lum = analogRead(LUMINOSITE_PIN);
  bool mov = digitalRead(MOUVEMENT_PIN);
  bool chc = digitalRead(CHOC_PIN);

  // Vérification si le DHT11 répond bien
  if (isnan(h) || isnan(t)) {
    // Si le capteur est mal branché, on envoie des valeurs par défaut
    t = 0.0;
    h = 0.0;
  }

  // 2. ENVOI AUTOMATIQUE VERS PYTHON
  EnvoyerJSON(t, h, lum, mov, chc);

  // 3. ATTENTE (Toutes les 5 secondes pour Firebase)
  delay(5000);
}