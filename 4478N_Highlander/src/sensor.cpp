#include "main.h"
#include "api.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "devices.h"
#include "auton.h"
#include "moveFunctions.h"
#include "autonSelector.h"
#include "sensor.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <numeric>

using namespace pros;
using namespace lemlib;



/**
 * Log current sensor and motor data to the SD card (datalog.csv).
 * Appends a row with timestamp, motor temps/velocities/positions, sensor values, etc.
 * Writes a header if the file is empty.
 */
 // Flag to indicate if datalogging is currently running
volatile bool dataLoggingRunning = false;
// Pointer to the datalogging background task
pros::Task *dataLoggingTask = nullptr;

void logData()
{
    FILE *file = std::fopen("/usd/datalog.csv", "a"); // Open file for appending
    if (!file)
    {
        pros::lcd::set_text(0, "SD open fail"); // Show error on LCD if SD card not found
        return;
    }
    // Write header if file is empty (first run)
    static bool headerWritten = false;
    if (!headerWritten)
    {
        std::fseek(file, 0, SEEK_END);
        if (std::ftell(file) == 0)
        {
            std::fprintf(file, "timestamp,leftTemp,leftVel,leftDeg,rightTemp,rightVel,rightDeg,intakeTemp,intakeVel,intakeDeg,autonSelectorDeg,hTrackerDeg,imuHeading\n");
        }
        headerWritten = true;
    }
    // Get time (ms since PROS start)
    uint32_t timestamp = pros::millis();

    // Get average values for left drive motors
    auto leftTemps = left_motors.get_temperature_all();
    auto leftVels = left_motors.get_actual_velocity_all();
    auto leftDegs = left_motors.get_position_all();
    double leftTemp = 0, leftVel = 0, leftDeg = 0;
    if (!leftTemps.empty())
        leftTemp = std::accumulate(leftTemps.begin(), leftTemps.end(), 0.0) / leftTemps.size();
    if (!leftVels.empty())
        leftVel = std::accumulate(leftVels.begin(), leftVels.end(), 0.0) / leftVels.size();
    if (!leftDegs.empty())
        leftDeg = std::accumulate(leftDegs.begin(), leftDegs.end(), 0.0) / leftDegs.size();

    // Get average values for right drive motors
    auto rightTemps = right_motors.get_temperature_all();
    auto rightVels = right_motors.get_actual_velocity_all();
    auto rightDegs = right_motors.get_position_all();
    double rightTemp = 0, rightVel = 0, rightDeg = 0;
        if (!rightTemps.empty())
        rightTemp = std::accumulate(rightTemps.begin(), rightTemps.end(), 0.0) / rightTemps.size();
    if (!rightVels.empty())
        rightVel = std::accumulate(rightVels.begin(), rightVels.end(), 0.0) / rightVels.size();
    if (!rightDegs.empty())
        rightDeg = std::accumulate(rightDegs.begin(), rightDegs.end(), 0.0) / rightDegs.size();

    // Intake motor values
    double intakeTemp = intake.get_temperature();
    double intakeVel = intake.get_actual_velocity();
    double intakeDeg = intake.get_position();

    // Rotation sensors (auton selector and horizontal tracker)

    double autonSelectorDeg = autonSelector.get_position() / 100.0; // Degrees
    double hTrackerDeg = hTracker.get_position() / 100.0;           // Degrees

    // IMU heading (degrees)
    double imuHeading = imu.get_heading();

    std::fprintf(file, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", 
        timestamp,      // %d (integer)
        leftTemp,      // %f (double)
        leftVel,       // %f (double)
        leftDeg,       // %f (double)
        rightTemp,     // %f (double)
        rightVel,      // %f (double)
        rightDeg,      // %f (double)
        intakeTemp,    // %f (double)
        intakeVel,     // %f (double)
        intakeDeg,     // %f (double)
        autonSelectorDeg, // %f (double)
        hTrackerDeg,   // %f (double)
        imuHeading);   // %f (double)
}

/**
 * Background task function for datalogging.
 * Continuously logs data every 100ms while datalogging_running is true.
 */
void dataloggingLoopFn(void *)
{
    while (dataLoggingRunning)
    {
        logData();
        pros::delay(100); // Log every 100ms
    }
}

/**
 * Start the datalogging background task if not already running.
 */
void startDatalogging()
{
    if (dataLoggingRunning)
        return;
    dataLoggingRunning = true;
    dataLoggingTask = new pros::Task(dataloggingLoopFn, nullptr, "Datalogging Task");
}

/**
 * Stop the datalogging background task and clean up resources.
 */
void stopDatalogging()
{
    dataLoggingRunning = false;
    if (dataLoggingTask)
    {
        dataLoggingTask->remove();
        delete dataLoggingTask;
        dataLoggingTask = nullptr;
    }
}

