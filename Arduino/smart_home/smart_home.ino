//temperature et humidite
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

///////////////////////////Port des capteur

#define VIDE 0
#define CHOC 1
#define SON 2
#define IR 3
#define MV 4
#define HUM_TEMP 5

struct capteur{
  int type;
  void* instance;
  };



// Définition du tableau indiquant des port utilisable par les capteur digital
#define N 19
capteur capteurs[N];
int a;

void setup() {
  
  Serial.begin(9600);
  init_capteur();
  a = millis();
}

void loop() {
  lire_capteur();

}
