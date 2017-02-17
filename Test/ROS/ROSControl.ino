/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;

int controlPin = 7;
//char *channel = "ctrl_msg";
char *channel = "set_msg";

char *message = "";

void messageCb( const std_msgs::String& toggle_msg){
  String a = toggle_msg.data;
  a.toCharArray(message, a.length()+1);
  if (a.equalsIgnoreCase("activate")) {
    digitalWrite(controlPin, HIGH);
  } else {
    digitalWrite(controlPin, LOW);
  }
  //digitalWrite(controlPin, HIGH-digitalRead(controlPin));   // blink the led
}

ros::Subscriber<std_msgs::String> sub(channel, &messageCb );

void setup()
{ 
  pinMode(controlPin, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{  
  nh.spinOnce();
  delay(1);
}

