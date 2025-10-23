/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Program:      241E Codebase (Vexcode)                                   */
/*    Module:       main.cpp                                                  */
/*    Author:       Andrew Bobay                                              */
/*    Team:         BBR1                                                      */
/*    Created:      Sep. 30th 2025, 2:30 PM                                   */
/*    Modified:     Oct. 23rd 2025, 07:00 PM                                  */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;
brain Brain;
controller Controller = controller(primary);

// A global instance of competition
competition Competition;
motor drive_fr = motor(PORT1, ratio6_1, false);
motor drive_br = motor(PORT2, ratio6_1, false);
motor drive_fl = motor(PORT4, ratio6_1, true);
motor drive_bl = motor(PORT5, ratio6_1, true);

motor_group drive_right = motor_group(drive_fr, drive_br);
motor_group drive_left = motor_group(drive_fl, drive_bl);

inertial IMU = inertial(PORT7);
// Included below are example values, CHANGE THEM FOR YOUR ROBOT
double wheel_travel = 260; // Given by vex
double track_width = 285.75; // Distance between the 2 drive sides
double wheel_base = 177.8; // Distance between the front and back axels
double wheel_c = 10.21; // Circumfrence of wheels
double gear_ratio = (36/48); // (Motorin/MotorOut)
smartdrive Drivetrain = smartdrive(drive_left, drive_right, IMU, wheel_travel, track_width, wheel_base, mm, gear_ratio);


motor smartmtr = motor(PORT11, ratio6_1, false);
pneumatics piston = pneumatics(Brain.ThreeWirePort.A);

// define control booleans for driver control here
bool drive_right_bool = true;
bool drive_left_bool = true;
bool piston_toggle = false;
bool smartmtr_bool = true;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

// Basic Intertial Calibration 
Brain.Screen.print("Device Init...");
Brain.Screen.setCursor(2,1);
wait(200, msec);
IMU.calibrate(0);
Brain.Screen.print("Calibrating inertial");
while (IMU.isCalibrating()){
  wait(25, msec);
}
Brain.Screen.setCursor(3,1);
Brain.Screen.print("Inertial Calibration Complete");

// Set Velocity and clear encoders 
Drivetrain.setDriveVelocity(100, percent);
}

/*---------------------------------------------------------------------------*/
/*                          Autonomous Functions                             */
/*                                                                           */
/*  Use This space to define functions to be used in autonomous,             */
/*  This includes control loops like PID, or Odomtry                         */                                                                    
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    
    // ........................................................................

    // Example Arcade stick control, I reccomend tuning some of these values to fit your driving style
    int drivetrain_right_speed = (Controller.Axis3.position()) - Controller.Axis1.position();
    int drivetrain_left_speed = (Controller.Axis3.position()) + Controller.Axis1.position();

    if (fabs(drivetrain_left_speed) < 5){
      if (drive_left_bool){
        drive_left.stop();
        drive_left_bool = false;
      } else {
        drive_left_bool = true;
      }
    }
    if (fabs(drivetrain_right_speed) < 5){
      if (drive_right_bool){
        drive_right.stop();
        drive_right_bool = false;
      } else {
        drive_right_bool = true;
      }
    }
      if (drive_left_bool) {
        drive_left.setVelocity(drivetrain_left_speed, percent);
        drive_left.spin(forward);
      }

      if (drive_right_bool) {
        drive_right.setVelocity(drivetrain_right_speed, percent);
        drive_right.spin(forward);
      }

      // Motor Contoller
      if (Controller.ButtonR1.pressing()) {
        smartmtr.spin(reverse);
        smartmtr_bool = false;
      } else if (Controller.ButtonR2.pressing()) {
        smartmtr.spin(fwd);
        smartmtr_bool = false;
      } else if (!smartmtr_bool) {
        smartmtr.stop();
        smartmtr_bool = true;
      }
      // Basic Toggle Controller
      if (Controller.ButtonL2.pressing()) {
        piston_toggle = !piston_toggle;
        while (Controller.ButtonL2.pressing()){}
          if (piston_toggle) {
            piston.open();
          } else {
            piston.close();
          }
        }

    



    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }

}
//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
