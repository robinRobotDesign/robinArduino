#include <ros.h>
#define USE_USBCON
#include <Arduino.h>
#include <std_msgs/String.h>
#include <QueueList.h>
#define HOTCHOCO 5
#define LATTEMACCHIATO 6
#define WIENERMELANGE 7
#define TEA 8
#define CAFFELATTE 9
#define CAPPUCCINO 10
#define COFFEE 11
#define ESPRESSO 12
#define Soda1 50
#define Soda2 51
#define Soda3 52
#define Soda4 53

QueueList<int> orderlist;

ros::NodeHandle nh;
char channel[] = "set_msg";
int Drink;
//char message[] = "";
std_msgs::String order_status;
ros::Publisher pub_status("orderStatus", &order_status);
void messageCb( const std_msgs::String& toggle_msg){
  String a = toggle_msg.data;
//  char message[20] = ;
  char message[a.length()+1];
  a.toCharArray(message, a.length()+1);
  order_status.data = "Received";
  pub_status.publish(&order_status);
  if(message[a.length()-1]=='A'){
    order_status.data = "Station A";
    pub_status.publish(&order_status);
    makeOrder(a.substring(0,a.length()-1));
  } else if(message[a.length()-1]=='B'){
    order_status.data="Station B";
    pub_status.publish(&order_status);
  } else if(message[a.length()-1]=='C'){
    order_status.data="Station C";
    pub_status.publish(&order_status);
  } else if(message[a.length()-1]=='D'){
    order_status.data="Station D";
    pub_status.publish(&order_status);
  }
/*  if (a.equalsIgnoreCase("000001a")) {
    digitalWrite(13, LOW);
  } else {*/
//  coffee_status.data = "NO COFFEE";
//  pub_coffee.publish(&coffee_status);
//  digitalWrite(13,HIGH-digitalRead(13));
//  }
}

ros::Subscriber<std_msgs::String> sub(channel, &messageCb );

void setup() {
 // nh.getHardware()->setBaud();
  pinMode(13, OUTPUT);
  nh.initNode();
  delay(100);
  nh.subscribe(sub);
  delay(100);
  //nh.advertise(pub_coffee);
  nh.advertise(pub_status);
//  nh.advertise(pub_error);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly
  nh.spinOnce();
  delay(100);
  //makeOrder("100034042");
  while(!orderlist.isEmpty()){
    orderlist.pop();
    order_status.data="popped order";
    
    pub_status.publish(&order_status);
    delay(10);
    Serial.println("TEST");
  }
  order_status.data="Order completed";
  pub_status.publish(&order_status);
  
  
  delay(100);
  
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
    order_status.data = "AddedCoffee";
    pub_status.publish(&order_status);
    break;
  case 1:
    runLoop(amount, ESPRESSO);
    order_status.data = "AddedEspresso";
    pub_status.publish(&order_status);
    break;
  case 2:
    runLoop(amount, CAPPUCCINO);
    order_status.data = "AddedCappu";
    pub_status.publish(&order_status);
    break;
  case 3:
    runLoop(amount, CAFFELATTE);
    order_status.data = "AddedCaffeLatte";
    pub_status.publish(&order_status);
    break;
  case 4:
    runLoop(amount, LATTEMACCHIATO);
    order_status.data = "AddedLatteMacchiato";
    pub_status.publish(&order_status);
    break;
  case 5:
    runLoop(amount, HOTCHOCO);
    order_status.data = "AddedHotChoco";
    pub_status.publish(&order_status);
    break;
  case 6:
    runLoop(amount, WIENERMELANGE);
    order_status.data = "AddedWienerMel";
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

