//temperature et humidite
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

///////////////////////////Port des capteur
//pièce
#define SALON "salon"
#define CUISINE "cuisine"
#define SALLE_BAIN "salle_bain"
#define CHAMBRE "Chambre"
#define JARDIN "Jardin"


//type de capteur
#define VIDE 0
#define CHOC 1
#define SON 2
#define IR 3
#define MV 4
#define HUM_TEMP 5
#define HUM_SOL 6
#define EAU 7
#define LUX 8
#define BUTTON 9
#define VIDEO 10

struct capteur{
  int type;
  char* piece;
  void* instance;
  };



//Initialisation du tableau indiquant des port utilisable par les capteur
#define N 19
capteur capteurs[N];


//variable qui contiendra le temps passer depuis le démarage de l'arduino
unsigned long a;
int m;
int s;
unsigned long mtd;
unsigned long mtf;

///////////////////////////Initialisation
void setup() {
  Serial.begin(9600);
  Serial.println("WELCOME TO SMARTHOME !");
  Serial.println("une application vous permetant de gerer votre chez vous à distance");
  Serial.println();
  initialisation();
  configuration();
  a = millis();
  m = 0;  
  s = 0;
}

void loop() {
  lire_capteur();
  
}
