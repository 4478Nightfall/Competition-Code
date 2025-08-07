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

void spinIntake(double direction){
    intake.set_brake_mode(MOTOR_BRAKE_HOLD);
    intake.move(direction*127);
}

void stopIntake(){
    intake.set_brake_mode(MOTOR_BRAKE_HOLD);
    intake.brake();
}
void handleBackGate(){
backGate.set_value(backGate.get_value() == LOW ? HIGH : LOW);
}

void handleFrontGate(){
frontGate.set_value(frontGate.get_value() == LOW ? HIGH : LOW);
}

void handleMLMech(){
matchloadMech.set_value(matchloadMech.get_value() == LOW ? HIGH : LOW);
}

double slewStep = 2;

double slew(double val, double fwdVal){
  static double prevVal = 0;
  if (fwdVal >= 0){  
    if (prevVal + slewStep < val){
    prevVal = prevVal + slewStep;
    return prevVal;
  }
  prevVal = val;
  return val;
  }else
  {
     if (prevVal - slewStep > val){
    prevVal = prevVal - slewStep;
    return prevVal;
  }
  prevVal = val;
  return val;
  }
}

void drivePID(double fwdVal, double limit, double timeout) {
    // PID constants
    double kP = 0.2; // Proportional constant
    double kD = 0.015; // Derivative constant
    // No integral component as disturbances were not observed in this game mode

    // Robot specifications
    const double diameter = 2.75; // Wheel diameter in inches
    const double pi = 3.14; // Value of pi
    const double outputGear = 48; // Output gear ratio
    const double inputGear = 36; // Input gear ratio

    // Convert forward value to target degrees
        double num = fwdVal; 
    double denom = (diameter * pi) * (inputGear / outputGear); // Calculate denominator
    double target = ((num / denom) * 360); // Convert forward value to degrees

    // Reset motor positions to zero
 left_motors.tare_position_all();

    bool isComplete = true; // Flag to determine if the target is reached
    double startTime = pros::millis(), prevTime = startTime, deltaTime, currentTime;
    double error = 0, derivative = 0, prevError = 0; // Initialize error variables
    double processVariable = left_motors.get_position();
    double output = 0; // Output value for motor speed
    // Use left_motors as the main base leader (mbl)
    auto& mbl = left_motors;

    // PID control loop
    while (isComplete) {
        currentTime = pros::millis(); // Get current time
        deltaTime = currentTime - prevTime; // Calculate time since last iteration
        prevTime = currentTime; // Update previous time

        processVariable = mbl.get_position(); // Update process variable (current position)

        // Proportional control
        error = target - processVariable; // Calculate error from target

        // Derivative control
        derivative = (error - prevError) / deltaTime; // Calculate derivative of error

        prevError = error; // Update previous error for next iteration

        // Calculate output based on PID formula
        output = (error * kP) + (derivative * kD); // PID output

        output = slew(output, fwdVal); // slew rate is added to the output of motors for PID

        // Limit output to the specified limit (saturation)
        if (output > limit) output = limit;
        if (output < -limit) output = -limit;

        // Spin motors with the calculated output
        left_motors.move(output);
        right_motors.move(output);

        // Timeout check
        if ((currentTime - startTime) >= timeout) {
            isComplete = false; // Exit the loop if timeout is reached
            left_motors.brake();
            right_motors.brake();
        }

        // Check if the error is within tolerance to stop the motors
        if (fabs(error) <= 5) {
            isComplete = false; // Exit the loop if target is reached
            left_motors.brake();
            right_motors.brake();
        }

        pros::delay(20); // Wait for a short duration before next iteration
    }
}

void turnPID(double turnVal) {
    // PID constants
    double kP = 0.3; // Proportional constant
    double kD = 0.017; // Derivative constant

    // Initialization
    bool isComplete = true; // Flag to determine if the target is reached
    double startTime = pros::millis(), prevTime = startTime, deltaTime, currentTime;
    double error = 0, derivative = 0, prevError = 0; // Initialize error variables
    double currentDeg = imu.get_heading(); // Current heading of the robot
    double output; // Output value for motor speed
    double target = turnVal; // Target heading

    // PID control loop
    while (isComplete) {
        currentTime = pros::millis(); // Get current time
        deltaTime = currentTime - prevTime; // Calculate time since last iteration
        prevTime = currentTime; // Update previous time
        currentDeg = imu.get_heading(); // Update current heading

        // Calculate error
        error = target - currentDeg;

        // Normalize error to range [-180, 180]
        if (error > 180) {
            error -= 360; // Wrap around for positive error
        } else if (error < -180) {
            error += 360; // Wrap around for negative error
        }

        // Derivative calculation
        derivative = (error - prevError) / deltaTime; // Calculate derivative of error
        prevError = error; // Update previous error for next iteration

        // Calculate output using PID formula
        output = error * kP + derivative * kD;
        // output = turnSlew(output);//adds the slew to the code to make it accelerate gradualy

        // Spin motors based on output
left_motors.move(output); // Left motors move in forward direction
        right_motors.move(-output); // Right motors move in reverse direction

        // Check if the error is within tolerance to stop the motors
        if (fabs(error) < 5) {
            isComplete = false; // Exit the loop if target is reached

            // Stop motors with hold mode (consider changing to coast if smoother stop needed)
            left_motors.brake();
            right_motors.brake();
            
    // Print the final heading on the brain screen
    lcd::set_text(6, "Final Heading: " + std::to_string(imu.get_heading()));            
    break; // Exit the loop
        }

        pros::delay(20); // Wait for a short duration before next iteration
    }

}