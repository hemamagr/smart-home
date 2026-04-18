int lire_entree(char * texte){
  Serial.write(texte);
  while (Serial.available()<= 0){
    }
  int tm = Serial.available();
  char * entre = new char[tm-1]; 
  int i = 0; 
  while (i<tm-1){
    entre[i] = Serial.read();
    i = i+1;
  }
  int result = (int) entre;
  Serial.write(entre);
  delete[] entre;
  return result;
}
