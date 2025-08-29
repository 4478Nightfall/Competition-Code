#include "main.h"
#include "api.h"
#include "pros/motors.hpp"
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

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_motors({-8, -19, -12}, pros::MotorGearset::blue); // left motors use 600 RPM cartridges
pros::MotorGroup right_motors({3, 2, 10}, pros::MotorGearset::blue);    // right motors use 600 RPM cartridges
pros::Motor intakeLoop{12, pros::MotorGearset::blue};
pros::Motor frontStage{-10, pros::MotorGearset::green}; // front stage motor
pros::MotorGroup intake({-10, 12});                     // intake motors use 600 RPM cartridges
pros::Motor mbl(-12, pros::MotorGearset::blue);
pros::Rotation autonSelector(1);
pros::Rotation hTracker(-19);
pros::Imu imu(20);
pros::Optical colorSort(21);
adi::Port backGate('A', E_ADI_DIGITAL_OUT);
adi::Port frontGate('B', E_ADI_DIGITAL_OUT);
adi::Port matchloadMech('C', E_ADI_DIGITAL_OUT);
lemlib::TrackingWheel horizontal_tracking_wheel(&hTracker, lemlib::Omniwheel::NEW_275, 4, 1);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motors,               // left motor groupm
                              &right_motors,              // right motor group
                              13,                         // 10 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 4" omnis
                              450,                        // drivetrain rpm is 450
                              5                           // horizontal drift is 2 (for now)
);

lemlib::OdomSensors sensors(nullptr,                    // vertical tracking wheel 1, set to null
                            nullptr,                    // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr,                    // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu                        // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(20, // proportional gain (kP)
                                              0,  // integral gain (kI)
                                              15, // derivative gain (kD)
                                              0,  // anti windup
                                              0,  // small error range, in inches
                                              0,  // small error range timeout, in milliseconds
                                              0,  // large error range, in inches
                                              0,  // large error range timeout, in milliseconds
                                              15  // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(3,  // proportional gain (kP)
                                              0,  // integral gain (kI)
                                              17, // derivative gain (kD)
                                              0,  // anti windup
                                              0,  // small error range, in inches
                                              0,  // small error range timeout, in milliseconds
                                              0,  // large error range, in inches
                                              0,  // large error range timeout, in milliseconds
                                              20  // maximum acceleration (slew)
);

ExpoDriveCurve throttleCurve(3,    // joystick deadband out of 127
                             10,   // minimum output where drivetrain will move out of 127
                             1.019 // expo curve gain
);

// input curve for steer input during driver control
ExpoDriveCurve steerCurve(3,    // joystick deadband out of 127
                          10,   // minimum output where drivetrain will move out of 127
                          1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain,         // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors,            // odometry sensors
                        &throttleCurve,
                        &steerCurve);

TeamColor currentTeam = BLUE;