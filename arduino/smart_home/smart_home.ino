#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A0
#define DHTTYPE DHT11

#define LUMINOSITE_PIN A2
#define MOUVEMENT_PIN 7
#define CHOC_PIN 6

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(MOUVEMENT_PIN, INPUT);
  pinMode(CHOC_PIN, INPUT);

  Serial.println("Systeme OK");
}

// ===============================
// Fonction JSON
// ===============================
void EnvoyerJSON(float temp, float hum, int lum, bool mouv, bool choc, String piece) {
  Serial.print("{");

  Serial.print("\"temperature\":"); Serial.print(temp);
  Serial.print(",\"humidite\":"); Serial.print(hum);
  Serial.print(",\"luminosite\":"); Serial.print(lum);
  Serial.print(",\"mouvement\":"); Serial.print(mouv ? "true" : "false");
  Serial.print(",\"choc\":"); Serial.print(choc ? "true" : "false");

  Serial.print(",\"piece\":\""); Serial.print(piece); Serial.print("\"");

  Serial.println("}");
}

// ===============================
// LOOP
// ===============================
void loop() {

  // Lecture capteurs REELS
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int lum = analogRead(LUMINOSITE_PIN);
  bool mov = digitalRead(MOUVEMENT_PIN);
  bool choc = digitalRead(CHOC_PIN);

  // sécurité DHT
  if (isnan(h) || isnan(t)) {
    h = 0;
    t = 0;
  }

  // ==========================
  // SALON (données réelles)
  // ==========================
  EnvoyerJSON(t, h, lum, mov, choc, "salon");
  delay(2000);

  // ==========================
  // CHAMBRE (simulation)
  // ==========================
  EnvoyerJSON(t - 2, h - 5, lum - 10, mov, choc, "chambre");
  delay(2000);
}