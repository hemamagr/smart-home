String lire_entree(char * texte){
  Serial.println(texte);
  
  while (Serial.available()== 0){
    }
  String result = Serial.readStringUntil('\n');

  Serial.println(result);
  return result;
}
