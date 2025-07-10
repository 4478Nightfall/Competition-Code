#ifndef __FUNCTIONS__
#define __FUNCTIONS__

// required files for devices
#include "main.h" 

// namespace for declarations
using namespace pros;


extern void spinIntake(double=1);//-1 for outake, 1 for intake
extern void stopIntake();
extern void clampOn();
extern void clampOff();
extern double slew(double, double);
void drivePID(double, double=100, double=2000);
extern void turnPID(double);

#endif