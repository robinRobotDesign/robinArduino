/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle  nh;

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);
char *hello = "Welkom";

boolean activate = true;


void messageCb( const std_msgs::String& msg) {
  String a = msg.data;
  a.toCharArray(hello, a.length()+1);
  if (a.equalsIgnoreCase("activate")) {
    activate = false;
  }
}
ros::Subscriber<std_msgs::String> sub("set_msg", &messageCb );


void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
  
  
  nh.spinOnce();
  delay(18000);
  
  nh.spinOnce();
  
  hello = "000";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  //delay(8000);
  
  hello = "001";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  nh.spinOnce();
  delay(1000);
  
  
  
  hello = "002";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  hello = "003";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  //delay(17000);
  
  hello = "004";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  hello = "005";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  hello = "006";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
}

void loop()
{
  if (activate == true) {
    str_msg.data = hello;
    chatter.publish( &str_msg );
    nh.spinOnce();
    delay(1000);
  }   
  /*
  hello = "001 --once";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  
  hello = "002 --once";
  str_msg.data = hello;
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(1000);
  */
}
