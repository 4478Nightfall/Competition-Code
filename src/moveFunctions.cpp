#include "main.h"
#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "devices.h"
#include "auton.h"
#include "moveFunctions.h"
#include <string>
#include <iostream>
#include <thread>
using namespace pros;
using namespace lemlib; 

void spinIntake(double time){
    intake.set_brake_mode(MOTOR_BRAKE_HOLD);
    intake.move(127);
    delay(time);
    intake.brake();
}
void clampOn(){
    clamp.set_value(HIGH);
}
void clampOff(){
    clamp.set_value(LOW);
}