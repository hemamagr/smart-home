
void init_capteur(){
  for(int i = 0; i<N; i++){
    capteurs[i].type = VIDE;
    capteurs[i].instance = NULL;
  }
  temp_hum_init();
  digit_init();
}



//température
void temp_hum_init(){
  for (int i = 14; i < N; i ++){
    DHT* dht = new DHT(i, DHT11);
    dht->begin();
    delay(2000);
    float t = dht->readTemperature();
    float h = dht->readHumidity();
    if (!isnan(t) and !isnan(h)){
      capteurs[i].type = HUM_TEMP;
      capteurs[i].instance = dht;
    }
  }
}


void digit_init(){
  char texte[] = "vouler vous brancher des capteur ? taper 1 pour oui, 0 pour non";
  while (lire_entree(texte)){
    char texte2[] = "selectionner le numero du type de capteur a brancher: 1.CHOC 2.SON 0.annuler";
    int capteur = lire_entree(texte2);
    if (capteur != 0){
      char texte3[] = "dans quel pin vouler vous relier votre capteur ? ";
      int pin = lire_entree(texte3);
      capteurs[pin].type = capteur;
      Serial.write("capteur initialisé");
    }
  }
}
