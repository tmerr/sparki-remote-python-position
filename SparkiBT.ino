/*
 * Sparki code to accept commands over Bluetooth and respond.
 * Wheel values are sent continuously, other sensor values can be requested.
 * Works in tandem with Sparki.java v.1
 *
 * Zack Butler, Kedarnath Calangutkar, Ruturaj Hagawane, zde
 */
 
#include <Sparki.h>  // include the sparki library
#include "PositionTracker.h"

#define STATUS_OK 0
#define MOVE_FORWARD 1
#define MOVE_BACKWARD 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4
#define SERVO 5
#define REQ_PING 6
#define REQ_WHEELS 7
#define MOVE_STOP 8
#define REQ_LINESENS 9
#define MOVE_FORWARD_DIST 10
#define REQ_POS 11
#define MOVE_BACKWARD_DIST 12
#define MOVE_RIGHT_DEG 13
#define MOVE_LEFT_DEG 14

PositionTracker p = PositionTracker();

void setup()
{
  Serial1.begin(9600);
  sparki.servo(SERVO_CENTER);
  sparki.clearLCD();
  sparki.println("Starting...");
  sparki.updateLCD();
}

void sendTravel() {
  sparki.print(sparki.totalTravel(0));
  sparki.print(" ");
  sparki.println(sparki.totalTravel(1));
  sparki.updateLCD();
  Serial1.print(sparki.totalTravel(0));
  Serial1.print(" ");
  Serial1.print(sparki.totalTravel(1));
  Serial1.print("*");
}

void writePosition() {
  float x = p.getCenter().x;
  float y = p.getCenter().y;
  float angle = p.getAngle();
  Serial1.print(x);
  Serial1.print(" ");
  Serial1.print(y);
  Serial1.print(" ");
  Serial1.print(angle);
  Serial1.print("*");
}

void loop()
{
  if (Serial1.available()) 
  {
    byte opcode = Serial1.read();
    sparki.print(opcode);
    sparki.print("-");
    sparki.updateLCD();
    int ping, angle;
    
    switch(opcode) {
      case MOVE_FORWARD:
          sparki.println("MOVE_FORWARD");
          sparki.updateLCD();
          sparki.moveForward();
        break;
      case MOVE_FORWARD_DIST:
        {
          float dist = 0;
          sparki.print("MOVE_FORWARD_DIST: ");
          char* distbytes = (char* ) &dist;
          for (int i=0; i<=3; i++) {
            while(!Serial1.available());
            distbytes[i] = Serial1.read();
          }
          sparki.println(dist);
          sparki.updateLCD();
          sparki.moveForward(dist);
        }
        break;
      case MOVE_BACKWARD:
          sparki.println("MOVE_BACKWARD");
          sparki.updateLCD();
          sparki.moveBackward();
        break;
      case MOVE_BACKWARD_DIST:
        {
          float dist = 0;
          sparki.print("MOVE_BACKWORD_DIST: ");
          char* distbytes = (char* ) &dist;
          for (int i=0; i<=3; i++) {
            while(!Serial1.available());
            distbytes[i] = Serial1.read();
          }
          sparki.println(dist);
          sparki.updateLCD();
          sparki.moveBackward(dist);
        }
        break;
      case MOVE_LEFT:
          sparki.println("MOVE_LEFT");
          sparki.updateLCD();
          sparki.moveLeft();
        break;
      case MOVE_LEFT_DEG:
        {
          float deg = 0;
          sparki.print("MOVE_LEFT_DEG: ");
          char* degbytes = (char* ) &deg;
          for (int i=0; i<=3; i++) {
            while(!Serial1.available());
            degbytes[i] = Serial1.read();
          }
          sparki.println(deg);
          sparki.updateLCD();
          sparki.moveLeft(deg);
        }
        break;
      case MOVE_RIGHT:
          sparki.println("MOVE_RIGHT");
          sparki.updateLCD();
          sparki.moveRight();
        break;
      case MOVE_RIGHT_DEG:
        {
          float deg = 0;
          sparki.print("MOVE_RIGHT_DEG: ");
          char* degbytes = (char* ) &deg;
          for (int i=0; i<=3; i++) {
            while(!Serial1.available());
            degbytes[i] = Serial1.read();
          }
          sparki.println(deg);
          sparki.updateLCD();
          sparki.moveRight(deg);
        }
        break;
      case MOVE_STOP:
          sparki.println("MOVE_STOP");
          sparki.updateLCD();
          sparki.moveStop();
        break;
      case SERVO:
          sparki.print("SERVO: ");
          while(!Serial1.available());
          angle = Serial1.read();
          angle -= 90;
          sparki.println(angle);
          sparki.updateLCD();
          sparki.servo(angle);
        break;
      case REQ_PING:
          sparki.print("PING: ");
          ping = sparki.ping();
          sparki.println(ping);
          sparki.updateLCD();
          Serial1.print(ping);
          Serial1.print("*");
        break;
      case REQ_LINESENS:
          sparki.print("Sensors: ");
          int svals[5];
          svals[0] = sparki.edgeLeft();
          svals[4] = sparki.edgeRight();
          svals[1] = sparki.lineLeft();   // measure the left IR sensor
          svals[2] = sparki.lineCenter(); // measure the center IR sensor
          svals[3] = sparki.lineRight();  // measure the right IR sensor
          for (int s = 0; s < 5; s++) {
            sparki.print(svals[s]);
            sparki.print(" ");
          }
          sparki.println();
          sparki.updateLCD();
          for (int s = 0; s < 5; s++) {
            Serial1.print(svals[s]);
            Serial1.print(" ");
          }
          Serial1.print("*");
        break;
      case REQ_WHEELS:
          sparki.print("WHEELS: ");
          sparki.updateLCD();
          sendTravel();
        break;
      case REQ_POS:
          sparki.println("POS");
          sparki.updateLCD();
          writePosition();
        break;          
      default:
        sparki.println("Wrong OPCODE");
        sparki.updateLCD();
    }
    p.update();
  }
}
