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

volatile bool datalogging_running = false;
pros::Task* datalogging_task = nullptr;

void log_data() {
    FILE* file = std::fopen("/usd/datalog.csv", "a");
    if (!file) {
        pros::lcd::set_text(0, "SD open fail");
        return;
    }
    // Write header if file is empty
    static bool header_written = false;
    if (!header_written) {
        std::fseek(file, 0, SEEK_END);
        if (std::ftell(file) == 0) {
            std::fprintf(file, "timestamp,left_temp,left_vel,left_deg,right_temp,right_vel,right_deg,intake_temp,intake_vel,intake_deg,overclock_temp,overclock_vel,overclock_deg,rotation_deg,autonSelector_deg,hTracker_deg,imu_heading\n");
        }
        header_written = true;
    }
    // Get time (ms since PROS start)
    uint32_t timestamp = pros::millis();
    // Motors (MotorGroup: average values)
    auto left_temps = left_motors.get_temperature_all();
    auto left_vels = left_motors.get_actual_velocity_all();
    auto left_degs = left_motors.get_position_all();
    double left_temp = 0, left_vel = 0, left_deg = 0;
    if (!left_temps.empty()) left_temp = std::accumulate(left_temps.begin(), left_temps.end(), 0.0) / left_temps.size();
    if (!left_vels.empty()) left_vel = std::accumulate(left_vels.begin(), left_vels.end(), 0.0) / left_vels.size();
    if (!left_degs.empty()) left_deg = std::accumulate(left_degs.begin(), left_degs.end(), 0.0) / left_degs.size();

    auto right_temps = right_motors.get_temperature_all();
    auto right_vels = right_motors.get_actual_velocity_all();
    auto right_degs = right_motors.get_position_all();
    double right_temp = 0, right_vel = 0, right_deg = 0;
    if (!right_temps.empty()) right_temp = std::accumulate(right_temps.begin(), right_temps.end(), 0.0) / right_temps.size();
    if (!right_vels.empty()) right_vel = std::accumulate(right_vels.begin(), right_vels.end(), 0.0) / right_vels.size();
    if (!right_degs.empty()) right_deg = std::accumulate(right_degs.begin(), right_degs.end(), 0.0) / right_degs.size();

    double intake_temp = intake.get_temperature();
    double intake_vel = intake.get_actual_velocity();
    double intake_deg = intake.get_position();

    double overclock_temp = Overclock.get_temperature();
    double overclock_vel = Overclock.get_actual_velocity();
    double overclock_deg = Overclock.get_position();

    // Rotation sensors
    double rotation_deg = rotation.get_position() / 100.0; // centidegrees to degrees
    double autonSelector_deg = autonSelector.get_position() / 100.0;
    double hTracker_deg = hTracker.get_position() / 100.0;

    // IMU
    double imu_heading = imu.get_heading();

    std::fprintf(file, "%u,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
        timestamp,
        left_temp, left_vel, left_deg,
        right_temp, right_vel, right_deg,
        intake_temp, intake_vel, intake_deg,
        overclock_temp, overclock_vel, overclock_deg,
        rotation_deg, autonSelector_deg, hTracker_deg,
        imu_heading
    );
    std::fclose(file);
}

void datalogging_loop_fn(void*) {
    while (datalogging_running) {
        log_data();
        pros::delay(100); // 100ms
    }
}

void start_datalogging() {
    if (datalogging_running) return;
    datalogging_running = true;
    datalogging_task = new pros::Task(datalogging_loop_fn, nullptr, "Datalogging Task");
}

void stop_datalogging() {
    datalogging_running = false;
    if (datalogging_task) {
        datalogging_task->remove();
        delete datalogging_task;
        datalogging_task = nullptr;
    }
}

