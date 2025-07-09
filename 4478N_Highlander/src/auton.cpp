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

void skills(){
    chassis.moveToPoint(20, 15, 4000, {.forwards = true}, true);
}

void elimBallRush(int side){
chassis.setPose(-54,(side*9),45);
chassis.moveToPose(32,side*29,45,2000,{.minSpeed = 100, .earlyExitRange = 5},true);

}

void odomTesting(){

    chassis.setPose(22.631, -66.417, 0,false);
    chassis.moveToPoint(45.984,-35.369,2000,{},true);
    chassis.moveToPoint(47.045,45.57,1000,{},false);

}

// Forward declaration for datalogging functions
void start_datalogging();
void stop_datalogging();

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