#include "../system/Helm.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#define DISTANCE_TOLERANCE 1
#define ROTATION_TOLERANCE 5

pthread_t helm_test, reader;

bool checkTolerance( float testValue, int   expectedValue, int tolerance );
bool checkTolerance( int   testValue, int   expectedValue, int tolerance );
void doTest( bool value );
int passedTests = 0;
int failedTests = 0;

void* helmet( void* ptr ) {
  
  xyz testLocation;
  float testRotation;
  
  Helm wesley;
  
  std::cout << "Go +40 cm" << std::endl;
  wesley.goDistance(40);
  testLocation = wesley.getLocation();
  doTest ( checkTolerance ( testLocation.y, 40, DISTANCE_TOLERANCE ) );
  doTest ( checkTolerance ( testLocation.x, 0, DISTANCE_TOLERANCE ) );
  
  std::cout << "Go -40 cm" << std::endl;
  wesley.goDistance(-40);
  testLocation = wesley.getLocation();
  doTest ( checkTolerance ( testLocation.y, 0, DISTANCE_TOLERANCE ) );
  doTest ( checkTolerance ( testLocation.x, 0, DISTANCE_TOLERANCE ) );
  
  std::cout << "Rotate +90 degrees" << std::endl;
  wesley.rotate(90);
  testRotation = wesley.getRotation();
  doTest ( checkTolerance ( testRotation, 90, ROTATION_TOLERANCE ) );
  
  std::cout << "Rotate -90 degrees" << std::endl;
  wesley.rotate(-90);
  testRotation = wesley.getRotation();
  doTest ( checkTolerance ( testRotation, 0, ROTATION_TOLERANCE ) );
  
}

void doTest ( bool value ) {
  if ( value ) {
    std::cout << "Passed test." << std::endl;
    passedTests++;
  } else {
    std::cout << "Failed test." << std::endl;
    failedTests++;
  }
}

bool checkTolerance ( float testValue, int expectedValue, int tolerance ) {
  
  float relError = abs( expectedValue - testValue ) / abs ( expectedValue );
  
  if ( relError > tolerance ) {
    return false;
  } else {
    return true;
  }
  
}

bool checkTolerance ( int testValue, int expectedValue, int tolerance ) {
  
  float relError = abs( expectedValue - testValue ) / abs ( expectedValue );
  
  if ( relError > tolerance ) {
    return false;
  } else {
    return true;
  }
  
}


/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}



void* read(void* ptr)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  char ** argv;
  int argc=1;
  ros::init(argc, argv, "listener");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("posReporter", 1000, chatterCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
  ros::spin();
  
}

int main(int argc, char **argv) {
  
  char* msg1 = "hello";
  
  pthread_create( &helm_test, NULL, &helmet, (void*) msg1 );
  pthread_create( &reader,    NULL, &read  , (void*) msg1 );
  
  sleep(60);
  
  pthread_join( helm_test, NULL );
  pthread_join( reader, NULL );
  
  return 0;
  
}