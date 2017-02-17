#include <QueueList.h>


QueueList<int> orderlist;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  makeOrder("100034072");
  while(!orderlist.isEmpty()){
    Serial.println(orderlist.pop());
  }
  delay(10000);

}



void makeOrder(String list) {
  if(list.length()>8){
    for(int i = 8;i<list.length();i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
//      //order_status.data = &letter;
//      //pub_status.publish(&//order_status);
      addItem(i,j);
    }
    for(int i = 0;i<8;i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
      //order_status.data = &letter;
      //pub_status.publish(&//order_status);
      addItem(i,j);
    }
  } else {
    for(int i = 0;i<list.length();i++){
      char letter = list.charAt(i);
      int j = atoi(&letter);
//      //order_status.data = &letter;
//      //pub_status.publish(&//order_status);
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
//    //order_status.data = "AddedCoffee";
//    //pub_status.publish(&//order_status);
    break;
  case 1:
    runLoop(amount, ESPRESSO);
    //order_status.data = "AddedEspresso";
    //pub_status.publish(&//order_status);
    break;
  case 2:
    runLoop(amount, CAPPUCCINO);
    //order_status.data = "AddedCappu";
    //pub_status.publish(&//order_status);
    break;
  case 3:
    runLoop(amount, CAFFELATTE);
    //order_status.data = "AddedCaffeLatte";
    //pub_status.publish(&//order_status);
    break;
  case 4:
    runLoop(amount, LATTEMACCHIATO);
    //order_status.data = "AddedLatteMacchiato";
    //pub_status.publish(&//order_status);
    break;
  case 5:
    runLoop(amount, HOTCHOCO);
    //order_status.data = "AddedHotChoco";
    //pub_status.publish(&//order_status);
    break;
  case 6:
    runLoop(amount, WIENERMELANGE);
    //order_status.data = "AddedWienerMel";
    //pub_status.publish(&//order_status);
    break;
  case 7:
    runLoop(amount, TEA);
    //order_status.data = "AddedTea";
    //pub_status.publish(&//order_status);
    break;
  case 8:
    runLoop(amount, SODA1);
    break;
  case 9:
    runLoop(amount, SODA2);
    break;
  case 10:
    runLoop(amount, SODA3);
    break;
  case 11:
    runLoop(amount, SODA4);
    break;

  }
}

