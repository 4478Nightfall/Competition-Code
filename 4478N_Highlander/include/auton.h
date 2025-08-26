#ifndef __AUTONS__
#define __AUTONS__

// required files for devices
#include "main.h" 

// namespace for declarations
using namespace pros;


extern void skills();
extern void elimBallRush(int side, int color);// 1 for left and red, -1 for right and blue
extern void soloAWP();
extern void odomTesting();
extern void sensorTesting();


#endif