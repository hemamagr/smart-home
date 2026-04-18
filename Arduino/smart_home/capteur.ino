//Fonction de lecteure des capteur

void lire_capteur(){
  for (int i = 2; i < N; i ++){
    if (capteurs[i].type != VIDE){
      Serial.print("{carte : arduino_uno , PIN : ");
      Serial.print(i);
      if (capteurs[i].type == HUM_TEMP){
        int b = millis(); 
        if ((b-a) > 2000){
          temperature_humdity(i);
          a = b;
        }
      }
      if(capteurs[i].type == CHOC){
        choc(i);
      }
      if(capteurs[i].type == SON){
        son(i);
      }
      Serial.println("}");
    }
  }    
}     

void temperature_humdity(int i){
  DHT* dht = (DHT*)capteurs[i].instance;
  Serial.print(", température : ");
  Serial.print(dht->readTemperature());
  Serial.print(", humidité : ");
  Serial.print(dht->readHumidity());
}

//choc
void choc(int i){
  int chok = i;
  int val;
  pinMode(chok, INPUT);
  val = digitalRead(chok);
  if (val==HIGH){
    
  }else{
  
    Serial.println("choc");
    Serial.println(val);
  }
}

//son
void son(int i){
  
  int bruit = i;
  int val;
  pinMode(bruit, INPUT);
  val = digitalRead(bruit);
  if (val==HIGH){
    Serial.println("son");
    Serial.println(val);
    
  }else{
    
  }
}
