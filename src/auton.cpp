#include "main.h"
#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "devices.h"
#include "auton.h"
#include "sensor.h"
#include "moveFunctions.h"
#include "autonSelector.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <numeric>
using namespace pros;
using namespace lemlib; 

ASSET(blueRight_txt);
void skills(){
    chassis.moveToPoint(20, 15, 4000, {.forwards = true}, true);
}


void AWPLeft(){
    chassis.moveToPoint(10, 15, 4000, {.forwards = true}, true);
}

void AWPRight(){
    chassis.moveToPoint(100, 15, 4000, {.forwards = true}, true);
}

void goalRushLeft(){
    chassis.moveToPoint(8, 50, 4000, {.forwards = true}, true);
}
void goalRushRight(){


}

void odomTesting(){

    chassis.setPose(22.631, -66.417, 0,false);
    chassis.moveToPoint(45.984,-35.369,2000,{},true);
    chassis.moveToPoint(47.045,45.57,1000,{},false);

}

void sensorTesting(){
    // Start datalogging
    start_datalogging();

 chassis.setPose(0, 0, 0);

    // Move forward 24 inches
    chassis.moveToPoint(0, 24, 5000);


    // Turn 90 degrees to the right
    chassis.turnToHeading(90, 2000);


    // Move forward another 24 inches
    chassis.moveToPoint(24, 24, 5000);


    // Stop datalogging
    stop_datalogging();
}