/////////////////////////// Fonction pour l'initialisation des capteur


void initialisation(){
  for(int i = 0; i<N; i++){
    capteurs[i].type = VIDE;
    capteurs[i].instance = NULL;
    capteurs[i].piece = "autre";
  }
}
  
void configuration(){
  )
  Serial.println("Pour commencer, quelle genre de configuration souhaitez vous faire?")
   int config = lire_entree("tapez le nombre corespondant? 0: Prédefinie, 1: au choix").toInt();
  while ((config != 0) && (config!= 1)){
    config = lire_entree("tapez le nombre corespondant? 0: Prédefinie, 1: au choix").toInt();
  }
  if (config == 0){
    config_predefini();
  }
  if (config == 1){
    config_au_choix();
  }
}

void config_predefini(){
  //température salle de bain
  capteurs[14].type = BUT;
  capteurs[14].instance = new DHT(14, DHT11);
  dht->begin();
  delay(2000);
  capteurs[i].piece = "salle_de_bain";
  //température cuisine
  capteurs[14].type = BUT;
  capteurs[14].instance = new DHT(14, DHT11);
  dht->begin();
  delay(2000);
  capteurs[i].piece = "salle_de_bain";
  
  
  capteurs[2].type = MV;
  capteurs[2].piece = SALON;
  pinMode(2, INPUT);
  capteurs[5].type = CHOC;
  capteurs[2].piece = SALON;
  pinMode(5, INPUT);
  capteurs[7].type = SON;
  capteurs[2].piece = CHAMBRE;
  pinMode(7, INPUT);
  capteurs[8].type = EAU;
  capteurs[2].piece = JARDIN;
  pinMode(8, INPUT);
  
}

















 
  temp_hum_init();
  digit_init();
  Serial.println("Fin Initialisation");
}

//température et humidité
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
      capteurs[i].piece = SALON;
    }
  }
}


void digit_init(){
  
 
}



void config_au_choix(){
  int branchement = lire_entree("vouler vous brancher des capteur ? 1: oui, 0: non").toInt();
  while ((branchement != 0) && (branchement != 1)){
    branchement = lire_entree("vouler vous brancher des capteur ? 1: oui, 0: non").toInt();
  }
  while (branchement == 1){
    int capteur = lire_entree("selectionner le numero du type de capteur a brancher: 1.CHOC 2.SON 4.MOUVE 6.HUM_SOL 8.LUX 0.annuler").toInt();
    while ((capteur < 0) || (capteur > 8)){
      capteur = lire_entree("selectionner le numero du type de capteur a brancher: 1.CHOC 2.SON 4.MOUVE 6.HUM_SOL 8.LUX 0.annuler").toInt();
    }
    int pin = lire_entree("dans quel pin vouler vous relier votre capteur ? : coisisser entre 2 et 13").toInt();
    while ((pin < 2) || (pin > 19)){
      pin = lire_entree("dans quel pin vouler vous relier votre capteur ? : coisisser entre 2 et 13").toInt();
    }
    pinMode(pin, INPUT);
    capteurs[pin].type = capteur;
    //Serial.println("capteur initialisé");
      
    branchement = lire_entree("vouler vous brancher des capteur ? 1: oui, 0: non").toInt();
    while ((branchement != 0) && (branchement != 1)){
      branchement = lire_entree("vouler vous brancher des capteur ? 1: oui, 0: non").toInt();
    }
  }
}
