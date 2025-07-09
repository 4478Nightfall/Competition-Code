#ifndef __FUNCTIONS__
#define __FUNCTIONS__

// required files for devices
#include "main.h" 

// namespace for declarations
using namespace pros;


extern void spinIntake(double);
extern void clampOn();
extern void clampOff();
extern double slew(double, double);
extern void drivePID(double, double, bool, double);
extern void turnPID(double);

#endif