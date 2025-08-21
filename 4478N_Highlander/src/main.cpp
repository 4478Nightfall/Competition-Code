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

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
    pros::lcd::initialize();  // initialize brain screen
    chassis.calibrate();      // calibrate sensors
    backGate.set_value(LOW);  // Set back gate to default position
    frontGate.set_value(LOW); // Set front gate to default position
    lcd::set_text(3, "Spin Wheel to select auton");

    // Start a background task to update the LCD with robot pose and selection
    pros::Task screen_task([&]()
                           {
        while (true) {
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x position
            pros::delay(100);
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y position
            pros::delay(100);
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            pros::delay(100);
            pros::lcd::print(5, "Rotation Sensor: %i", hTracker.get_position());
            pros::delay(100);
            displaySelection(); // Show auton selection
            pros::delay(100);
        } });
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled()
{
    // pros::lcd::register_btn1_cb(autonSelector); // Example for registering a callback
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
    int selection = getAutonSelection(); // Get selected auton routine

    // Run the selected autonomous routine
    switch (selection)
    {
    case 0:
        // Red - Left AWP Autonomous
        break;
    case 1:
        // Red - Right AWP Autonomous
        break;
    case 2:
        // Red - Skills Autonomous
        sensorTesting();
        break;
    case 3:
        // Add your custom routine here
        break;
    case 4:
        // Blue - Right AWP Autonomous
        skills();
        break;
    case 5:
        // Blue - Skills Autonomous
        odomTesting();
        break;
    case 6:
        // Neutral - Defensive Autonomous
        sensorTesting();
        break;
    default:
        sensorTesting();
        // Default routine or do nothing
        break;
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
    // Main driver control loop
    while (true)
    {
        intake.set_brake_mode(MOTOR_BRAKE_HOLD);        // Hold intake when not moving
        right_motors.set_brake_mode(MOTOR_BRAKE_COAST); // Coast for smoother drive
        left_motors.set_brake_mode(MOTOR_BRAKE_COAST);

        // Get joystick values for tank drive
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        pow(leftY, 2) / 100;
        pow(rightY, 2) / 100;

        // Move the robot using tank drive
        chassis.tank(leftY, rightY);

        // Intake control
        if (controller.get_digital(E_CONTROLLER_DIGITAL_R1))
        {
            intakeLoop.move(-127);
        }
        else if (controller.get_digital(E_CONTROLLER_DIGITAL_R2))
        {
            intakeLoop.move(127);
        }
        else
        {
            intake.brake();
        }

        // Front stage control
        if (controller.get_digital(E_CONTROLLER_DIGITAL_L1))
        {
            frontStage.move(-127);
        }
        else if (controller.get_digital(E_CONTROLLER_DIGITAL_L2))
        {
            frontStage.move(127);
        }
        else
        {
            frontStage.brake();
        }

        // Toggle back gate with A
        if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A))
        {
            backGate.set_value(backGate.get_value() == LOW ? HIGH : LOW);
        }

        // Toggle front gate with Left
        if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT))
        {
            frontGate.set_value(frontGate.get_value() == LOW ? HIGH : LOW);
        }

        // Toggle Match Load Mech gate with B
        if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_B))
        {
            matchloadMech.set_value(matchloadMech.get_value() == LOW ? HIGH : LOW);
        }

        // delay to save resources
        pros::delay(25);
    }
}