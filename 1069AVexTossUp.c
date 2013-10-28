#pragma config(Sensor, in1,    programSelect,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  rightBack,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftBack,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  rightFront,     sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  leftFront,      sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  arm,            sensorQuadEncoder)
#pragma config(Sensor, dgtl11, downLimit,      sensorTouch)
#pragma config(Sensor, dgtl12, upLimit,        sensorTouch)
#pragma config(Motor,  port1,            ,             tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port2,           rightBack,     tmotorServoContinuousRotation, openLoop, reversed, encoder, encoderPort, dgtl1, 0)
#pragma config(Motor,  port3,           leftBack,      tmotorServoContinuousRotation, openLoop, reversed, encoder, encoderPort, dgtl3, 1000)
#pragma config(Motor,  port4,           rightFront,    tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port5,           leftFront,     tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl7, 1000)
#pragma config(Motor,  port6,           leftArm,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port7,           rightArm,      tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port8,            ,             tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port9,           intake,        tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          conveyor,      tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*----------------------------------------------------------------------------------------------------*\
|*                                - Berthoud Robotics Team 1069A -                                    *|
|*                                        VEX Toss UP 2013                                            *|
|*                                                                                                    *|
|*                                                                                                    *|
|*                                        ROBOT CONFIGURATION                                         *|
|*    NOTES:                                                                                          *|
|*    1)                                                                                              *|
|*    2)                                                                                              *|
|*    3)                                                                                              *|
|*                                                                                                    *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]              [Name]              [Type]              [Description]                   *|
|*    see pragma statements above for motor and sensor configurations                                 *|
|*                                                                                                    *|
\*----------------------------------------------------------------------------------------------------*/

//+++++++++++++++++++++++++++++++++++++++++++++| MAIN |+++++++++++++++++++++++++++++++++++++++++++++++
task main ()
{
  word speedCommandLeft;            // desired speed command for left wheels, from left joystick
  word speedCommandRight;           // desired speed command for right wheels, from right joystick
  word speedCommandThreshold = 10;  // helps to eliminate 'noise' from a joystick that isn't perfectly at (0,0)
  bool armUpCommand = false;				// true if the arm is being commanded to go up
  bool armDownCommand = false;			// true if the arm is being commanded to go down
  bool intakeInCommand = false;			// true if intake is being commanded to intake balls
  bool intakeOutCommand = false;		// true if intake is being commanded to output balls
  bool conveyorUpCommand = false;		// true if the conveyor is being commanded to pull balls away from intake
  bool conveyorDownCommand = false;	// true if the conveyor is being commanded to push balls toward intake
  bool armUpLimitReached = false;
  bool armDownLimitReached = false;


  while(1 == 1)
  {
    speedCommandLeft 			= vexRT[Ch3];   //
    speedCommandRight 		= vexRT[Ch2];   //
    armUpCommand 					= vexRT[Btn5U] == 1 ? true : false;
    armDownCommand 				= vexRT[Btn5D] == 1 ? true : false;
    intakeInCommand 			= vexRT[Btn6U] == 1 ? true : false;
    intakeOutCommand 			= vexRT[Btn6D] == 1 ? true : false;
    conveyorUpCommand 		= vexRT[Btn7U] == 1 ? true : false;
    conveyorDownCommand 	= vexRT[Btn7D] == 1 ? true : false;
    armUpLimitReached 		= SensorValue( upLimit ) == 1 ? true : false;
    armDownLimitReached 	= SensorValue( downLimit ) == 1 ? true : false;


    if(armDownLimitReached && (motor[rightArm] > 0))
    {
    	motor[rightArm] = 0;
    	motor[leftArm] = 0;
 		}
 		if(armUpLimitReached && (motor[rightArm] > 0))
    {
    	motor[rightArm] = 0;
    	motor[leftArm] = 0;
 		}

    if((abs(speedCommandLeft) > speedCommandThreshold) && (abs(speedCommandLeft) > 0))
    {
      motor[leftFront]  = (speedCommandLeft);
      motor[leftBack] = (speedCommandLeft*0.8);
    }

    if((abs(speedCommandRight) > speedCommandThreshold) && (abs(speedCommandRight) > 0))
    {
     	motor[rightFront] = (speedCommandRight);
     	motor[rightBack] = (speedCommandRight*0.8);
    }

 		if ((abs(speedCommandRight) < speedCommandThreshold))
 		{
 			motor[rightFront] = 0;
 			motor[rightBack] = 0;
 		}

 		if ((abs(speedCommandLeft) < speedCommandThreshold))
 		{
 			motor[leftFront] = 0;
 			motor[leftBack] = 0;
 		}

 		if ( armUpCommand == false && armDownCommand == false )
 		{
 			motor[ rightArm ] = 0;
 			motor[ leftArm ] =0;
 		}
 		else if ( armUpCommand == true && armDownCommand == true )
 		{
 			motor[ rightArm ] = 0;
 			motor[ leftArm] = 0;
 		}
 		else if ( armUpCommand == true )
 		{
 			motor[rightArm] = 50;
 			motor[leftArm] = 50;
		}
		else if (armDownCommand == true )
		{
			motor[ rightArm ] = -50;
			motor[ leftArm ] = -50;
		}

		if ( intakeInCommand == false && intakeOutCommand == false )
		{
			motor[ intake ] = 0;
		}
		else if ( intakeInCommand == true && intakeOutCommand == true )
 		{
 			motor[ intake ] = 0;
 		}
 		else if ( intakeInCommand == true )
 		{
 			motor[ intake ] = 127;
		}
		else if (intakeOutCommand == true )
		{
			motor[ intake ] = -127;
		}

		if ( conveyorUpCommand == false && conveyorDownCommand == false )
		{
			motor[conveyor ] = 0;
		}
		else if ( conveyorUpCommand == true && conveyorDownCommand == true )
 		{
 			motor[ conveyor ] = 0;
 		}
 		else if ( conveyorUpCommand == true )
 		{
 			motor[ conveyor ] = 50;
		}
		else if (conveyorDownCommand == true )
		{
			motor[ conveyor ] = -50;
		}
  }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
