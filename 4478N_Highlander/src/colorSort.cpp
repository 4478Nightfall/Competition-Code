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

void colorLoop(){
    bool ballDetected = false;
    int count = 0; 
    if (colorSort.get_hue() <250 && colorSort.get_hue() < 180 ){
        // Blue detected
        pros::lcd::print(4, "Blue Detected");
        ballDetected = true;
    }
    if (ballDetected){
        count ++;
    }
    else if (colorSort.get_hue() > 100 && colorSort.get_hue() < 200){
        // red detected
        pros::lcd::print(4, "Blue Detected");
    }
}