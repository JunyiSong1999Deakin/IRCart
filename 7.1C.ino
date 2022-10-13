#include <IRremote.h>
#include "4WD_Car_for_Arduino.h"

//define key, the code can not be changed.
#define IR_REMOTE_KEYCODE_UP      0xFF02FD
#define IR_REMOTE_KEYCODE_DOWN    0xFF9867
#define IR_REMOTE_KEYCODE_LEFT    0xFFE01F
#define IR_REMOTE_KEYCODE_RIGHT   0xFF906F
#define IR_REMOTE_KEYCODE_CENTER  0xFFA857

#define IR_UPDATE_TIMEOUT     120
#define IR_CAR_SPEED          180

IRrecv irrecv(PIN_IRREMOTE_RECV);
decode_results results;
u32 currentKeyCode, lastKeyCode;
bool isStopFromIR = false;
u32 lastIRUpdateTime = 0;

float batteryVoltage = 0;
bool isBuzzered = false;

void pinsSetup() {
  pinMode(PIN_DIRECTION_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT);
  pinMode(PIN_DIRECTION_RIGHT, OUTPUT);
  pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT);

  pinMode(PIN_SONIC_TRIG, OUTPUT);// set trigPin to output mode
  pinMode(PIN_SONIC_ECHO, INPUT); // set echoPin to input mode

  pinMode(PIN_TRACKING_LEFT, INPUT); // 
  pinMode(PIN_TRACKING_RIGHT, INPUT); // 
  pinMode(PIN_TRACKING_CENTER, INPUT); // 

  setBuzzer(false);
}

void motorRun(int speedl, int speedr) {
  int dirL = 0, dirR = 0;
  if (speedl > 0) {
    dirL = 0;
  }
  else {
    dirL = 1;
    speedl = -speedl;
  }


void setup() {
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    isStopFromIR = false;
    currentKeyCode = results.value;
    if (currentKeyCode != 0xFFFFFFFF) {
      lastKeyCode = currentKeyCode;
    }
    switch (lastKeyCode) {
      case IR_REMOTE_KEYCODE_UP:
        motorRun(IR_CAR_SPEED, IR_CAR_SPEED); //move forward
        break;
      case IR_REMOTE_KEYCODE_DOWN:
        motorRun(-IR_CAR_SPEED, -IR_CAR_SPEED); //move back
        break;
      case IR_REMOTE_KEYCODE_LEFT:
        motorRun(-IR_CAR_SPEED, IR_CAR_SPEED);  //turn left
        break;
      case IR_REMOTE_KEYCODE_RIGHT:
        motorRun(IR_CAR_SPEED, -IR_CAR_SPEED);  //turn right
        break;
      case IR_REMOTE_KEYCODE_CENTER:
        setBuzzer(true);              //turn on buzzer
        break;
      default:
        break;
    }
    irrecv.resume(); // Receive the next value
    lastIRUpdateTime = millis(); //write down current time
  }
  else {
    if (millis() - lastIRUpdateTime > IR_UPDATE_TIMEOUT) {
      if (!isStopFromIR) {
        isStopFromIR = true;
        motorRun(0, 0);
        setBuzzer(false);
      }
      lastIRUpdateTime = millis();
    }
  }
}
