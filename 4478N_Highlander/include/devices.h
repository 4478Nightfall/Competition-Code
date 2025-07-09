#ifndef __DEVICES__
#define __DEVICES__

// required files for devices
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

// namespace for declarations
using namespace pros;
using namespace lemlib;

extern Controller controller;
extern MotorGroup left_motors;
extern MotorGroup right_motors;
extern Motor intake;
extern Rotation autonSelector;
extern Rotation hTracker;
extern Imu imu;
extern pros::Motor mbl;
extern adi::Port clamp;
extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::Drivetrain drivetrain;
extern lemlib::OdomSensors sensors;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::ExpoDriveCurve throttleCurve;
extern lemlib::ExpoDriveCurve steerCurve;
extern lemlib::Chassis chassis;


#endif