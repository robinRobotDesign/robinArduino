/**
 * Starts the soda machine to make a soda
 * drink = the drink value
 */
void makeSoda(int Drink) {
  switch (Drink) {
  case Soda1:
    digitalWrite(Soda1, LOW);
    delay(2900);
    digitalWrite(Soda1, HIGH);
    delay(2300);
    break;
  case Soda2:
    digitalWrite(Soda2, LOW);
    delay(3000);
    digitalWrite(Soda2, HIGH);
    delay(3000);
    break;
  case Soda3:
    digitalWrite(Soda3, LOW);
    delay(2000);
    digitalWrite(Soda3, HIGH);
    delay(3000);
    break;
  case Soda4:
    digitalWrite(Soda4, LOW);
    delay(2400);
    digitalWrite(Soda4, HIGH);
    delay(3000);
    break;
  }
}
