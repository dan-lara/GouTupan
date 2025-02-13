//TEST carte fonctionnelle 

void setup() 
{
  pinMode(6, OUTPUT); // Définit la LED intégrée comme sortie
}

void loop() 
{
  digitalWrite(6, HIGH); // Allume la LED
  delay(1000); // Attendre 1 seconde
  digitalWrite(6, LOW); // Éteint la LED
  delay(1000); // Attendre 1 seconde
}