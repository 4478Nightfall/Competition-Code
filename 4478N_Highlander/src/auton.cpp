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

void elimBallRush(int side, int color){
    if (color == 1){
        currentTeam = BLUE;
    }
    else if (color == -1){
        currentTeam = RED;
    }

chassis.setPose(-47.5,(side*8),45);
spinIntake(1);
drivePID(50);
if (side ==1){
chassis.swingToHeading(30,DriveSide::RIGHT,2000);
}
else if (side == -1){
chassis.swingToHeading(30,DriveSide::LEFT,2000);
}
handleMLMech();
delay(1000);
drivePID(-7);
handleMLMech();
drivePID(7);
chassis.turnToPoint(-23,side*23,2000);
drivePID(25);
turnPID(315);
drivePID(40);
turnPID(270);
drivePID(20);
delay(2000);
chassis.moveToPoint(-25,side*47,2000);
handleFrontGate();
}

void soloAWP(){
    
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