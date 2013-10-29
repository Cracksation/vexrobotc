#pragma config(Sensor, in1,    programSelect,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  backRight,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  backLeft,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armRight,       sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  armLeft,        sensorQuadEncoder)
#pragma config(Sensor, dgtl11, downLimit,      sensorTouch)
#pragma config(Sensor, dgtl12, upLimit,        sensorTouch)
#pragma config(Motor,  port1,           intake,        tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port2,           backRight,     tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl1, 0)
#pragma config(Motor,  port3,           backLeft,      tmotorServoContinuousRotation, openLoop, reversed, encoder, encoderPort, dgtl3, 1000)
#pragma config(Motor,  port4,           frontRight,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           frontLeft,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           armRight,      tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port7,           armLeft,       tmotorServoContinuousRotation, openLoop, reversed, encoder, encoderPort, dgtl7, 1000)
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
	// define easy to remember variables for the vexRT commands
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
	bool moveRobotForward = false;
	bool moveRobotBackward = false;
	bool spinRobotLeft = false;
	bool spinRobotRight = false;

	word armSpeedNormal = 50;
	word armSpeedFast = 60;
	word armSpeedSlow = 40;

	word intakeSpeed = 127;
	word conveyorSpeed = 50;

	// clear and stop all motors by resetting power to zero
	motor[ port1 ] = 0;
	motor[ port2 ] = 0;
	motor[ port3 ] = 0;
	motor[ port4 ] = 0;
	motor[ port5 ] = 0;
	motor[ port6 ] = 0;
	motor[ port7 ] = 0;
	motor[ port8 ] = 0;
	motor[ port9 ] = 0;
	motor[ port10 ] = 0;

	SensorValue[ backLeft ] = 0;
	SensorValue[ backRight ] = 0;
	SensorValue[ armLeft ] = 0;
	SensorValue[ armRight ] = 0;

	while(1 == 1)
	{
		// read all of the vex controller inputs to figure out what
		// the driver wants to do.
		speedCommandLeft 			= vexRT[Ch3];   //
		speedCommandRight 		= vexRT[Ch2];   //
		intakeInCommand 			= vexRT[Btn5U] == 1 ? true : false;
		intakeOutCommand 			= vexRT[Btn5D] == 1 ? true : false;
		conveyorUpCommand 		= vexRT[Btn6U] == 1 ? true : false;
		conveyorDownCommand 	= vexRT[Btn6D] == 1 ? true : false;
		armUpCommand 					= vexRT[Btn7U] == 1 ? true : false;
		armDownCommand 				= vexRT[Btn7D] == 1 ? true : false;
		moveRobotForward		  = vexRT[Btn8U] == 1 ? true : false;
		moveRobotBackward		  = vexRT[Btn8D] == 1 ? true : false;
		spinRobotLeft					= vexRT[Btn8L] == 1 ? true : false;
		spinRobotRight 				= vexRT[Btn8R] == 1 ? true : false;
		armUpLimitReached 		= SensorValue( upLimit ) == 1 ? true : false;
		armDownLimitReached 	= SensorValue( downLimit ) == 1 ? true : false;

		// check the limit switches and shut off the arm motors if the limit is reached
		if(armDownLimitReached && ((motor[armRight] > 0) || (motor[armLeft] > 0)))
		{
			motor[armRight] = 0;
			motor[armLeft] = 0;
		}
		if(armUpLimitReached && ((motor[armRight] > 0) || (motor[armLeft] > 0)))
		{
			motor[armRight] = 0;
			motor[armLeft] = 0;
		}

		// check the wheel speed commands from the controller and move the wheels if the
		// controller input value is greater than the joystick noise threshold
		if((abs(speedCommandLeft) > speedCommandThreshold) && (abs(speedCommandLeft) > 0))
		{
			motor[frontLeft]  = (speedCommandLeft);
			motor[backLeft] = (speedCommandLeft*0.8);
		}

		if((abs(speedCommandRight) > speedCommandThreshold) && (abs(speedCommandRight) > 0))
		{
			motor[frontRight] = (speedCommandRight);
			motor[backRight] = (speedCommandRight*0.8);
		}

		// if the wheel speed command from the joystick is less than the joystick
		// noise threshold then turn the motors off
		if ((abs(speedCommandRight) < speedCommandThreshold))
		{
			motor[frontRight] = 0;
			motor[backRight] = 0;
		}

		if ((abs(speedCommandLeft) < speedCommandThreshold))
		{
			motor[frontLeft] = 0;
			motor[backLeft] = 0;
		}

		// if no command buttons are being pushed then shut both motors off
		// if both command buttons are being pushed at the same time then shut both motors off
		// else move the motor in the direction of the button being pushed
		if ( armUpCommand == false && armDownCommand == false )
		{
			motor[ armRight ] = 0;
			motor[ armLeft ] = 0;
		}
		else if ( armUpCommand == true && armDownCommand == true )
		{
			motor[ armRight ] = 0;
			motor[ armLeft] = 0;
		}
		else if ( armUpCommand == true )
		{
			if(SensorValue[armRight] > SensorValue[armLeft])
			{
				motor[armLeft] = armSpeedFast;
				motor[armRight] = armSpeedSlow;
			}
			else if( SensorValue[armLeft] > SensorValue[armRight])
			{
				motor[armRight] = armSpeedFast;
				motor[armLeft] = armSpeedSlow;
			}
			else
			{
				motor[armRight] = armSpeedNormal;
				motor[armLeft] = armSpeedNormal;
			}
		}
		else if (armDownCommand == true )
		{
			if(SensorValue[armRight] > SensorValue[armLeft])
			{
				motor[armLeft] = -armSpeedSlow;
				motor[armRight] = -armSpeedFast;
			}
			else if( SensorValue[armLeft] > SensorValue[armRight])
			{
				motor[armRight] = -armSpeedSlow;
				motor[armLeft] = -armSpeedFast;
			}
			else
			{
				motor[armRight] = -armSpeedNormal;
				motor[armLeft] = -armSpeedNormal;
			}
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
			motor[ intake ] = intakeSpeed;
		}
		else if (intakeOutCommand == true )
		{
			motor[ intake ] = -intakeSpeed;
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
			motor[ conveyor ] = conveyorSpeed;
		}
		else if (conveyorDownCommand == true )
		{
			motor[ conveyor ] = -conveyorSpeed;
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
