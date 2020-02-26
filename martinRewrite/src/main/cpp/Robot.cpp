/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

void Robot::DriveCode() {
    //Elevator
    encoderReading = elevatorEncoder.Get();
    oCLeftYAxis = operatorController.GetRawAxis(1);
    if (oCLeftYAxis < -0.2) { //Stick is up
        elevatorMotor9.Set(-oCLeftYAxis*0.9);
        elevatorMotor7.Set(-oCLeftYAxis*0.9);
    }
    else if (oCLeftYAxis > 0.2) { //Stick is down
        elevatorMotor9.Set(-oCLeftYAxis*0.9);
        elevatorMotor7.Set(-oCLeftYAxis*0.9);
    }
    else {
        elevatorMotor9.Set(0.25); //Stick is idle
        elevatorMotor7.Set(0.25);
    }
    //Cargo Intake
    if ((encoderReading < 100) and cargoIntakeActive) {
        rollerMotor.Set(rollerSpeed);
    }
    else if (driverController.GetRawButton(3) and not cargoIntakeActive) {
        rollerMotor.Set(-rollerSpeed);
    }
    else if (driverController.GetRawButton(4) and not cargoIntakeActive) {
        rollerMotor.Set(rollerSpeed);
    }
    else {
        rollerMotor.Set(0);
    }

    if (driverController.GetRawButton(6) and iRSensor.Get()) { //Driver right bumper, no ball present
        cargoIntakeValve.Set(frc::DoubleSolenoid::kReverse); //Intake Position
        cargoIntakeActive = true;
    }
    else if (driverController.GetRawAxis(3) > 0.8) { //Driver right (?) trigger
        cargoIntakeValve.Set(frc::DoubleSolenoid::kReverse); //Intake position, to shoot or cancel
        rollerMotor.Set(0.0); //Roller off
        shooterWheel.Set(frc::Relay::Value::kOff); //Shooter wheel off
        cargoIntakeActive = false; //Shoot or cancel, either one ends intake
    }
    else if (cargoIntakeActive and not iRSensor.Get()) { //In intake and ball present
        cargoIntakeValve.Set(frc::DoubleSolenoid::kForward); //GRAB THE BALL
        rollerMotor.Set(0.0); //Roller off
        shooterWheel.Set(frc::Relay::Value::kReverse); //Shooter wheel on
        counter = 0;
        cargoIntakeActive = false; //Ball caught/tried to catch. Not in intake.
    }

    counter = counter+1;
    if (counter >= 40) { //50 counter values is 1 second
        shooterWheel.Set(frc::Relay::Value::kOff);
        counter = 0;
    }

    //Hatch Claw
    if (driverController.GetRawButton(5)) {
        hatchIntakeValve.Set(frc::DoubleSolenoid::kForward);
    }
    else if (driverController.GetRawAxis(2) > 0.8) {
        hatchIntakeValve.Set(frc::DoubleSolenoid::kReverse);
    }

    //DriveTrain
    dCLeftYAxis = driverController.GetRawAxis(1); //Read driver sticks
    dCRightYAxis = -driverController.GetRawAxis(5);

    if (driverController.GetRawButton(7)) {
        hatchIsFront = true;
    }
    else if (driverController.GetRawButton(8)) {
        hatchIsFront = false;
    }

    if (hatchIsFront) {
        tempLeftAxis = dCLeftYAxis;
        dCLeftYAxis = dCRightYAxis;
        dCRightYAxis = tempLeftAxis;
    }

    if ((dCLeftYAxis > 0) and (dCRightYAxis < 0)) {
        dCLeftYAxis = dCLeftYAxis * 0.75;
        dCRightYAxis = dCRightYAxis * 0.75;
    }

    if ((dCLeftYAxis > 0.2) or (dCLeftYAxis < -0.2)) {  //             Run LEFT Motors.
        leftTopMotor.Set(dCLeftYAxis);
        leftMidMotor.Set(dCLeftYAxis);
        leftLowMotor.Set(dCLeftYAxis);
    } else {
        leftTopMotor.Set(0);
        leftMidMotor.Set(0);
        leftLowMotor.Set(0);
    } 
  
    if ((dCRightYAxis > 0.2) or (dCRightYAxis < -0.2)) {  //          Run RIGHT Motors.
        rightTopMotor.Set(dCRightYAxis);
        rightMidMotor.Set(dCRightYAxis);
        rightLowMotor.Set(dCRightYAxis);
    } else {
        rightTopMotor.Set(0);
        rightMidMotor.Set(0);
        rightLowMotor.Set(0);
    }

    //Pixy stuff :D
    if (driverController.GetPOV() == 0) {
      pixyMode = true;
    } else if (driverController.GetPOV() == 180) {
      pixyMode = false;
    }

    if ((dCRightYAxis > 0.2) and pixyMode and not hatchIsFront) {
        dCRightYAxis = dCRightYAxis * 0.5;
        std::cout << pixyLine0.m_x0 << std::endl;
        if (pixyLine0.m_x0 == 128) {
            std::cout << "Pixy has lost the line" << std::endl;
            dCRightYAxis = dCRightYAxis * 2;
        } else if (pixyLine0.m_x0 < centerX) {
            dCLeftYAxis = -0.4;
            dCRightYAxis = 0.21;
        } else if (pixyLine0.m_x0 > centerX) {
            dCLeftYAxis = -0.21;
            dCRightYAxis = 0.4;
        } else {
            dCLeftYAxis = -dCRightYAxis;
        }
    }

} //End of drive code

void Robot::RobotInit() {
    compressor.SetClosedLoopControl(true);  //                                    Start Compressor.
    cargoCam = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);  //    Start USB Camera.
    hatchCam = frc::CameraServer::GetInstance()->StartAutomaticCapture(1);  //    Start USB Camera.
    cargoIntakeValve.Set(frc::DoubleSolenoid::kForward);  // Pre-Set Cargo Intake for LOADED Cargo.
}

void Robot::AutonomousInit() {}
void Robot::AutonomousPeriodic() {
    DriveCode();
}

void Robot::TeleopInit() {}
void Robot::TeleopPeriodic() {
    DriveCode();
}

void Robot::TestInit() {}
void Robot::TestPeriodic() {
    
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
