/**
 * Checks if there are enough cups ready to fill the train with drinks
 */
bool checkEnoughCupsReady() {
  Serial.print("OrderList count is: ");
  Serial.println(orderlist.count());
  Serial.print("cups placed is: ");
  Serial.println(checkCupsPlaced());
  bool b = orderlist.count()<=checkCupsPlaced()||checkCupsPlaced()==4;
  return b;
}
/**
 * checks which cup can be filled
 */
void setCupPosition() {
  for(int i = 0;i<4;i++){
    if(cup[i]==1){
      currentCup = i;
      return;
    }
  }
  currentCup=-1;
  return;
}

/**
 * sets cup i to an empty cup
 */
void setCupPlaced(int i) {
  cup[i]=1;
}

/**
 * checks if there are enough cups placed
 */
int checkCupsPlaced() {
  int amount = 0;
  for(int i = 0;i<4;i++){
    if(cup[i]>0){
      amount++;
    }
  }
  return amount;
}

/**
 * Checks how many cups are filled
 */
int checkFilledCups() {
  int amount = 0;
  for(int i = 0;i<4;i++){
    if(cup[i]==2){
      amount++;
    }
  }
  return amount;
}

/**
 * Sets all cup positions to empty
 */
void cleanCups() {
  for(int i = 0;i<4;i++){
    cup[i]=0;
  }
}

/**
 * Checks if all  cups are empty
 */
bool checkEmptyCups() {
  for(int i = 0;i<4;i++){
    if(cup[i]>0){
      return false;
    }
  }
  return true;
}

/**
 * Sets cup i as filled
 */
void setCupFilled(int i) {
  cup[i]=2;
}


