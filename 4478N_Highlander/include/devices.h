#ifndef __DEVICES__
#define __DEVICES__

// required files for devices
#include "main.h" 

// namespace for declarations
using namespace pros;
using namespace lemlib;

extern Controller controller;
extern MotorGroup left_motors;
extern MotorGroup right_motors;
extern Motor intake;
extern Motor Overclock;
extern Rotation rotation;
extern Rotation autonSelector;
extern Rotation hTracker;
extern Imu imu;
extern adi::Port clamp;
extern adi::Port B4Bar;
extern adi::Port Redirect;
extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern Drivetrain drivetrain;
extern OdomSensors sensors;
extern ControllerSettings lateral_controller;
extern ControllerSettings angular_controller;
extern ExpoDriveCurve throttleCurve;
extern ExpoDriveCurve steerCurve;
extern Chassis chassis;

#endif