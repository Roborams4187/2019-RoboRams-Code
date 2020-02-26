/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <string>

#include "Pixy2.h"

#include <frc/TimedRobot.h>
#include <frc/Joystick.h>
#include <frc/Spark.h>
#include <frc/Encoder.h>
#include <frc/Compressor.h>
#include <frc/DigitalInput.h>
#include <frc/DoubleSolenoid.h>
#include <frc/Relay.h>

#include <cameraserver/CameraServer.h>


class Robot : public frc::TimedRobot {
 public:
  void DriveCode();
  void RobotInit() override;

  void AutonomousInit() override;
  void AutonomousPeriodic() override;

  void TeleopInit() override;
  void TeleopPeriodic() override;

  void TestInit() override;
  void TestPeriodic() override;

  private:
  int counter = 0;
  //Camera
  cs::UsbCamera cargoCam;
  cs::UsbCamera hatchCam;
  //Pneumatics
  frc::Compressor compressor{0};
  //Drivetrain
  bool hatchIsFront = false;
  bool manualShift = true;

  double tempLeftAxis = 0;
  double dCLeftYAxis = 0;
  double dCRightYAxis = 0;

  frc::Joystick driverController{0}; // dC driverController
  frc::Joystick operatorController{1}; // oC operatorController

  frc::Spark  leftTopMotor{2};
  frc::Spark rightTopMotor{1};
  frc::Spark  leftMidMotor{8};
  frc::Spark rightMidMotor{3};
  frc::Spark  leftLowMotor{5};
  frc::Spark rightLowMotor{4};

  frc::Encoder  leftDriveEncoder{0,1};
  frc::Encoder rightDriveEncoder{2,3};

  //Elevator
  int encoderReading = 0;
  double oCLeftYAxis = 0;

  frc::Encoder elevatorEncoder{7,8};
  frc::Spark elevatorMotor9{9};
  frc::Spark elevatorMotor7{7};
  //Cargo Intake
  bool cargoIntakeActive = false;
  bool clearToShoot = false;
  bool cargoPresent = false;
  
  double cargoTrigger = 0;
  double rollerSpeed = 0.65;

  frc::Spark rollerMotor{6};
  frc::DigitalInput iRSensor{9};
  frc::DoubleSolenoid cargoIntakeValve{2,3};
  frc::DoubleSolenoid hatchIntakeValve{4,5};
  frc::Relay shooterWheel{0};
  
  //Pixy
  frc::I2C pixy2_0{frc::I2C::kOnboard,84};
  frc::I2C* PixyPointer0 = &pixy2_0;
  martin::PixyVector pixyLine0;
  bool pixyMode = false;
  int centerX = 38;
};
