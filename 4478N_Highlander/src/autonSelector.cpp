#include "main.h"
#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "devices.h"
#include "auton.h"
#include "moveFunctions.h"
#include "autonSelector.h"
#include <string>
#include <iostream>
#include <thread>
using namespace pros;
using namespace lemlib; 

const int NUM_AUTONS = 7; // Now set to 7 different routines

// Function to get the current autonomous selection
int getAutonSelection() {
    // Read the rotation sensor value in degrees (0-360)
    double rotation = autonSelector.get_position() / 100.0;
    
    // Normalize to positive value in case of negative readings
    while (rotation < 0) rotation += 360;
    rotation = fmod(rotation, 360);
    
    // Map the rotation to an autonomous routine
    // Divide the 360 degrees by the number of autonomous routines
    int segmentSize = 360 / NUM_AUTONS;
    return (int)(rotation / segmentSize);
}

// Function to display current selection on the brain
void displaySelection() {
    int selection = getAutonSelection();
    pros::lcd::set_text(3, "Autonomous Selector");
    
    std::string autonName;
    switch (selection) {
        case 0:
            autonName = "Left AWP";
            break;
        case 1:
            autonName = "Right AWP";
            break;
        case 2:
            autonName = "Left goal rush";
            break;
        case 3:
            autonName = "Right goal rush";
            break;
        case 4:
            autonName = "skills";
            break;
        case 5:
            autonName = "Odom testing";
            break;
        case 6:
            autonName = "sensor testing";
            break;
        default:
            autonName = "odom testing";
    }
    
    pros::lcd::set_text(4, "Selected: " + autonName);
    pros::lcd::set_text(6, ("degrees: " + std::to_string(autonSelector.get_position())).c_str());
}