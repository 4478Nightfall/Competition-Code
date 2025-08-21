# 4478N Highlander - VEX Robotics Competition Robot

A VEX V5 robotics competition robot built by Masuk 4478N Nightfall for the 2024-2025 VEX Robotics Competition season.

## 🚀 Project Overview

This project contains the complete source code for a VEX V5 robot designed for competitive robotics. The robot features advanced autonomous capabilities, precise movement control, and a modular design for easy maintenance and upgrades.

## 🏗️ Robot Architecture

### Core Components
- **Chassis System**: Tank drive with left/right motor groups
- **Intake System**: Dual-stage intake with color sorting capabilities
- **Stage Mechanism**: Front stage for intaking
- **Gate System**: Configurable front and back gates for scoring
- **Match Load Mechanism**: Automated match loading system

### Sensors & Control
- **IMU**: Inertial Measurement Unit for orientation tracking
- **Optical Sensor**: Color detection and sorting
- **Rotation Sensors**: Position tracking and autonomous selection
- **V5 Controller**: Driver control interface

## 🎮 Control Scheme

### Drive Control
- **Left Stick**: Left side tank drive
- **Right Stick**: Right side tank drive
- **Exponential Drive Curves**: Smooth acceleration and deceleration

### Mechanism Control
- **R1/R2**: Intake forward/reverse
- **L1/L2**: Front stage up/down
- **A Button**: Toggle back gate
- **Left Arrow**: Toggle front gate
- **B Button**: Toggle match load mechanism

### Autonomous Selection
- Use the rotation sensor on the robot to select autonomous routines
- LCD screen displays current selection and robot pose information
- Real-time position tracking with X, Y, and theta coordinates

## 🛠️ Technical Features

### Motion Control
- **LemLib Integration**: Advanced motion profiling and path following
- **PID Controllers**: Precise position and heading control
- **Odometry**: Real-time position tracking with tracking wheels
- **Chassis Calibration**: Automatic sensor calibration on startup

### Software Architecture
- **Modular Design**: Separated device definitions, autonomous routines, and movement functions
- **Task Management**: Background tasks for LCD updates and sensor monitoring
- **Error Handling**: Robust error handling and recovery systems

## 📁 Project Structure

```
4478N_Highlander/
├── src/                    # Source code files
│   ├── main.cpp           # Main robot program
│   ├── auton.cpp          # Autonomous routines
│   ├── devices.cpp        # Device initialization
│   ├── moveFunctions.cpp  # Movement and control functions
│   └── sensor.cpp         # Sensor handling
├── include/               # Header files
│   ├── devices.h          # Device declarations
│   ├── auton.h            # Autonomous function declarations
│   ├── moveFunctions.h    # Movement function declarations
│   └── sensor.h           # Sensor function declarations
├── firmware/              # V5 firmware and libraries
├── bin/                   # Compiled binary output
└── Makefile               # Build configuration
```

## 🔧 Dependencies

### Core Libraries
- **PROS 4.1.1**: VEX V5 development framework
- **LemLib 0.5.4**: Advanced motion control library
- **LVGL 8.3.8**: Graphics library for advanced UI

### Hardware Requirements
- VEX V5 Brain
- VEX V5 Controller
- V5 Smart Motors
- V5 Optical Sensor
- V5 IMU Sensor
- V5 Rotation Sensors
- ADI Digital Outputs (for gates)

### Development Workflow
1. **Edit Source**: Modify files in `src/` directory
2. **Build**: Run `make` to compile
3. **Test**: Upload to robot and test functionality
4. **Iterate**: Make adjustments and repeat

## 🎯 Competition Strategy

### Autonomous Phase
- **AWP Focus**: Prioritize Autonomous Win Points for early game advantage
- **Positioning**: Use precise odometry for consistent autonomous performance
- **Adaptive Selection**: Multiple autonomous routines for different field positions

## 🔍 Troubleshooting

### Common Issues
- **Sensor Calibration**: Ensure IMU and tracking wheels are properly calibrated
- **Motor Configuration**: Verify motor ports and gear ratios in device setup
- **Autonomous Selection**: Check rotation sensor connection for autonomous routine selection

### Debug Features
- **LCD Display**: Real-time robot pose and sensor data
- **Console Output**: Detailed logging for troubleshooting
- **Sensor Testing**: Built-in sensor validation routines

## 📄 License

This project is developed for VEX Robotics Competition use. All code and designs are property of 4478N and Masuk Robotics.

## 📞 Support

For questions or support regarding this robot code:
- Check the [PROS Documentation](https://pros.cs.purdue.edu/)
- Review [LemLib Documentation](https://lemlib.com/)
- Contact Team 4478N for team-specific assistance

---

**Good luck in competition! 🏆**
