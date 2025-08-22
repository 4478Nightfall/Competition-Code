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
#include <thread>
using namespace pros;
using namespace lemlib;
    int count = 0; 
    bool ballDetected = false;

void colorLoop(){
    if (colorSort.get_hue() <250 && colorSort.get_hue() < 180 ){
        // Blue detected
        pros::lcd::print(4, "Blue Detected");
        ballDetected = true;
    }
    if (ballDetected){
        count ++;
    }
    if (count == 5){
        backGate.set_value(HIGH);
    }
    if (count >= 8){
        backGate.set_value(LOW);
        count = 0;
        ballDetected = false;
    } 
}