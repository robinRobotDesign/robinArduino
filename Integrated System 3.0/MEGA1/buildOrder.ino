/*
void setup() {
  nh.initNode();
  delay(100);
  nh.subscribe(sub);
  delay(100);
  nh.advertise(pub_status);
  // put your setup code here, to run once:

}*/
/*
void loop() {
  // put your main code here, to run repeatedly
  nh.spinOnce();
  delay(100);
  while(!orderlist.isEmpty()){
    orderlist.pop();
    order_status.data="popped order";
    
    pub_status.publish(&order_status);
    delay(10);
  }
  
  
  delay(1000);
  
}*/

void makeOrder(String list) {
  if(list.length()>8){
    for(int i = 8;i<list.length();i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
//      order_status.data = &letter;
//      pub_status.publish(&order_status);
      if(j>0){
      addItem(i,j);
      }
      
    }
    for(int i = 0;i<8;i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
//      order_status.data = &letter;
//      pub_status.publish(&order_status);
      if(j>0){
      addItem(i,j);
      }
    }
  } else {
    for(int i = 0;i<list.length();i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
      addItem(i,j);
    }
  }
}

void runLoop(int amount, int Drink) {

  for(int i = 0;i<amount;i++){
    orderlist.push(Drink);
  }
}

void addItem(int drink,int amount) {
  switch(drink) {
  case 0:
    runLoop(amount, COFFEE);
    order_status.data = "AddedCoffee";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 1:
    runLoop(amount, ESPRESSO);
    order_status.data = "AddedEspresso";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 2:
    runLoop(amount, CAPPUCCINO);
    order_status.data = "AddedCappu";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 3:
    runLoop(amount, CAFFELATTE);
    order_status.data = "AddedCaffeLatte";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 4:
    runLoop(amount, LATTEMACCHIATO);
    order_status.data = "AddedLatteMacchiato";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 5:
    runLoop(amount, HOTCHOCO);
    order_status.data = "AddedHotChoco";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 6:
    runLoop(amount, WIENERMELANGE);
    order_status.data = "AddedWienerMel";
    order_status.data+=amount;
    pub_status.publish(&order_status);
    break;
  case 7:
    runLoop(amount, TEA);
    order_status.data = "AddedTea";
    pub_status.publish(&order_status);
    break;
  case 8:
    runLoop(amount, Soda1);
    order_status.data = "AddedSoda1";
    pub_status.publish(&order_status);
    break;
  case 9:
    runLoop(amount, Soda2);
    order_status.data = "AddedSoda2";
    pub_status.publish(&order_status);
    break;
  case 10:
    runLoop(amount, Soda3);
    order_status.data = "AddedSoda3";
    pub_status.publish(&order_status);
    break;
  case 11:
    runLoop(amount, Soda4);
    order_status.data = "AddedSoda4";
    pub_status.publish(&order_status);
    break;

  }
}

