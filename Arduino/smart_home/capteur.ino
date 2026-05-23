//Fonction de lecteure des capteur

void lire_capteur(){
  //Pour tous les pin
  for (int i = 2; i < N; i ++){
    //Si il est brancher a un capteur
    if (capteurs[i].type != VIDE){
      char data[64]= ""; 
      unsigned long b = millis(); 
      if ((b-a) > 5000){
        if (capteurs[i].type == HUM_TEMP){
          float value[2];
          temperature_humidity(i, value);
          char temp[8];
          char hum[8];
          dtostrf(value[0], 6, 2, temp);
          dtostrf(value[1], 6, 2, hum);
          snprintf(data, sizeof(data),"température : %s , humidité : %s", temp, hum);
          a = b;
        }
        if((capteurs[i].type == EAU) && (eau(i)== 1)){
        strcpy(data,"eau : True");
        }
      }
      if((capteurs[i].type == HUM_SOL) && (h_sol(i)== 1)){
        strcpy(data,"sol : True");
      }
      if((capteurs[i].type == LUX) && (digitalRead(i)== 1)){
        strcpy(data,"eau : True");
      }
      
      if((capteurs[i].type == CHOC) && (digitalRead(i)== LOW)){
        strcpy(data,"choc : True");
      }
      if((capteurs[i].type == SON) && (analogRead(i) == HIGH)){
        int bruit = digitalRead(i);
        if((bruit == HIGH)&& (s==0)){
        strcpy(data,"son : True");
        s = 1;
        mtd = millis();
        }
        if((bruit == LOW) && (s==1)){
          s=0;
          mtf = millis();
          Serial.println(mtf-mtd);
        }
      }
      if(capteurs[i].type == MV){
        int mouve = digitalRead(i);
        if((mouve == HIGH)&& (m==0)){
        strcpy(data,"mouvement : True");
        m = 1;
        mtd = millis();
        }
        if((mouve == LOW) && (m==1)){
          m=0;
          mtf = millis();
          Serial.println(mtf-mtd);
        }
      }
      if (strlen(data) > 0){
      Serial.print("{piece :");
      Serial.print(capteurs[i].piece);
      Serial.print(",");
      Serial.print(data);
      Serial.println("}");
      }
    }
  }    
}     

void temperature_humidity(int i, float* tab){
  DHT* dht = (DHT*)capteurs[i].instance;
  tab[0] = dht->readTemperature();
  tab[1] = dht->readHumidity();
}



//hum_sol
int h_sol(int i){
  int val = analogRead(i);
  Serial.println(val);
  if (val>=5000){
    return 1;   
  }else{
    return 0;
  }
}

//hum_sol
int eau(int i){
  
  int val = analogRead(i);
  Serial.print(val);
  if (val>=5000){
    return 1;   
  }else{
    return 0;
  }
}
