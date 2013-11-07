#pragma config(Sensor, in1,    programSelect,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  backRightEnc,      sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  backLeftEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  armRightEnc,       sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  armLeftEnc,        sensorQuadEncoder)
#pragma config(Sensor, dgtl11, downLimitSw,      sensorTouch)
#pragma config(Sensor, dgtl12, upLimitSw,        sensorTouch)
#pragma config(Motor,  port1,           conveyorMot,      tmotorVex393HighSpeed, openLoop)
#pragma config(Motor,  port2,           backRightMot,     tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl1, 0)
#pragma config(Motor,  port3,           backLeftMot,      tmotorServoContinuousRotation, openLoop, reversed, encoder, encoderPort, dgtl3, 1000)
#pragma config(Motor,  port4,           frontRightMot,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           frontLeftMot,     tmotorServoContinuousRotation, openLoop, reversed)
#pragma config(Motor,  port6,           armRightMot1,     tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl5, 1000)
#pragma config(Motor,  port7,           armRightMot2,     tmotorServoContinuousRotation, openLoop, encoder, encoderPort, dgtl7, 1000)
#pragma config(Motor,  port8,           armLeftMot1,      tmotorServoStandard, openLoop)
#pragma config(Motor,  port9,           armLeftMot2,      tmotorServoStandard, openLoop)
#pragma config(Motor,  port10,          intakeMot,        tmotorVex393, openLoop)
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
	bool conveyorUpCommand = false;		// true if the conveyorMot is being commanded to pull balls away from intakeMot
	bool conveyorDownCommand = false;	// true if the conveyorMot is being commanded to push balls toward intakeMot
	bool armUpLimitReached = false;
	bool armDownLimitReached = false;
	bool moveRobotForward = false;
	//bool moveRobotBackward = false;
	bool spinRobotLeft = false;
	bool spinRobotRight = false;
	bool intakeOn = false;
	bool intakeCommand = false;
	bool intakeButtonRelease = true;

	word armUpSpeedNormal = 117;
	word armUpSpeedFast = 127;
	word armUpSpeedSlow = 107;

	word armDownSpeedNormal = -24;
	word armDownSpeedFast = -32;
	word armDownSpeedSlow = -16;

	word armSpeedRateDiv = 1;
	word armSpeedOffset = 0;

	word intakeSpeed = 127;
	word conveyorSpeed = 127;

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

	SensorValue[ backLeftEnc ] = 0;
	SensorValue[ backRightEnc ] = 0;
	SensorValue[ armLeftEnc ] = 0;
	SensorValue[ armRightEnc ] = 0;

	while(1 == 1)
	{
		// read all of the vex controller inputs to figure out what
		// the driver wants to do.
		speedCommandLeft 			= vexRT[Ch3];   //
		speedCommandRight 		= vexRT[Ch2];   //
		intakeCommand					= vexRT[Btn8D] == 1 ? true : false;
		conveyorUpCommand 		= vexRT[Btn6U] == 1 ? true : false;
		conveyorDownCommand 	= vexRT[Btn5U] == 1 ? true : false;
		armUpCommand 					= vexRT[Btn6D] == 1 ? true : false;
		armDownCommand 				= vexRT[Btn5D] == 1 ? true : false;
		moveRobotForward		  = vexRT[Btn8U] == 1 ? true : false;
		//moveRobotBackward		  = vexRT[Btn8D] == 1 ? true : false;
		spinRobotLeft					= vexRT[Btn8L] == 1 ? true : false;
		spinRobotRight 				= vexRT[Btn8R] == 1 ? true : false;
		armUpLimitReached 		= SensorValue( upLimitSw ) == 1 ? true : false;
		armDownLimitReached 	= SensorValue( downLimitSw ) == 1 ? true : false;


		// check the wheel speed commands from the controller and move the wheels if the
		// controller input value is greater than the joystick noise threshold
		if((abs(speedCommandLeft) > speedCommandThreshold) && (abs(speedCommandLeft) > 0))
		{
			motor[frontLeftMot]  = (speedCommandLeft);
			motor[backLeftMot] = (speedCommandLeft*0.8);
		}

		if((abs(speedCommandRight) > speedCommandThreshold) && (abs(speedCommandRight) > 0))
		{
			motor[frontRightMot] = (speedCommandRight);
			motor[backRightMot] = (speedCommandRight*0.8);
		}

		// if the wheel speed command from the joystick is less than the joystick
		// noise threshold then turn the motors off
		if ((abs(speedCommandRight) < speedCommandThreshold))
		{
			motor[frontRightMot] = 0;
			motor[backRightMot] = 0;
		}

		if ((abs(speedCommandLeft) < speedCommandThreshold))
		{
			motor[frontLeftMot] = 0;
			motor[backLeftMot] = 0;
		}

		// if no command buttons are being pushed then shut both motors off
		// if both command buttons are being pushed at the same time then shut both motors off
		// else move the motor in the direction of the button being pushed
		// check the limit switches and shut off the arm motors if the limit is reached
		//if(armDownLimitReached == true && ((motor[armRightMot1] < 0) || (motor[armLeftMot1] < 0) || (motor[armRightMot2] < 0) || (motor[armLeftMot2] < 0)))
		if(armDownLimitReached == true && armDownCommand == true)
		{
			motor[armRightMot1] = 0;
			motor[armRightMot2] = 0;
			motor[armLeftMot1] = 0;
			motor[armLeftMot2] = 0;
		}
		//else if(armUpLimitReached == true && ((motor[armRightMot1] > 0) || (motor[armRightMot2] > 0) || (motor[armLeftMot1] > 0) || (motor[armLeftMot2] > 0)))
		else if(armUpLimitReached == true && armUpCommand == true)
		{
			motor[armRightMot1] = 0;
			motor[armRightMot2] = 0;
			motor[armLeftMot1] = 0;
			motor[armLeftMot2] = 0;
		}
		else if ( armUpCommand == false && armDownCommand == false )
		{
			motor[ armRightMot1 ] = 0;
			motor[ armRightMot2 ] = 0;
			motor[ armLeftMot1 ] = 0;
			motor[ armLeftMot2] = 0;
		}
		else if ( armUpCommand == true && armDownCommand == true )
		{
			motor[ armRightMot1 ] = 0;
			motor[ armRightMot2 ] = 0;
			motor[ armLeftMot1 ] = 0;
			motor[ armLeftMot2] = 0;
		}
		else if ( armUpCommand == true )
		{
			if( SensorValue[armRightEnc] > 60 && SensorValue[armLeftEnc] > 60 )
			{
				armSpeedRateDiv = 2;
				armSpeedOffset = 0;
			}
			else if( SensorValue[armRightEnc] > 45 && SensorValue[armLeftEnc] > 45 )
			{
				armSpeedRateDiv = 2;
			armSpeedOffset = 0;
			}
			else
			{
				armSpeedRateDiv = 1;
				armSpeedOffset = 0;
			}

			if(SensorValue[armRightEnc] > SensorValue[armLeftEnc])
			{
				motor[armLeftMot1] = armUpSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armUpSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armUpSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armUpSpeedSlow/armSpeedRateDiv + armSpeedOffset;
			}
			else if( SensorValue[armLeftEnc] > SensorValue[armRightEnc])
			{
				motor[armLeftMot1] = armUpSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armUpSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armUpSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armUpSpeedFast/armSpeedRateDiv + armSpeedOffset;
			}
			else
			{
				motor[armLeftMot1] = armUpSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armUpSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armUpSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armUpSpeedNormal/armSpeedRateDiv + armSpeedOffset;
			}
		}
		else if (armDownCommand == true )
		{
			if( SensorValue[armRightEnc] < 10 && SensorValue[armLeftEnc] < 10 )
			{
				armSpeedRateDiv = 4;
				armSpeedOffset = 8;
			}
			else if( SensorValue[armRightEnc] < 30 && SensorValue[armLeftEnc] < 30 )
			{
				armSpeedRateDiv = 2;
				armSpeedOffset = 16;
			}
			else
			{
				armSpeedRateDiv = 1;
				armSpeedOffset = 0;
			}

			if(SensorValue[armRightEnc] > SensorValue[armLeftEnc])
			{
				motor[armLeftMot1] = armDownSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armDownSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armDownSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armDownSpeedFast/armSpeedRateDiv + armSpeedOffset;
			}
			else if( SensorValue[armLeftEnc] > SensorValue[armRightEnc])
			{
				motor[armLeftMot1] = armDownSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armDownSpeedFast/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armDownSpeedSlow/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armDownSpeedSlow/armSpeedRateDiv + armSpeedOffset;
			}
			else
			{
				motor[armLeftMot1] = armDownSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armLeftMot2] = armDownSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot1] = armDownSpeedNormal/armSpeedRateDiv + armSpeedOffset;
				motor[armRightMot2] = armDownSpeedNormal/armSpeedRateDiv + armSpeedOffset;
			}
		}

		if ( intakeCommand == true && intakeButtonRelease == true)
		{
			intakeButtonRelease = false;

			if( intakeOn == true )
			{
					intakeOn = false;
			}
			else if( intakeOn == false )
			{
					intakeOn = true;
			}
		}

		if ( intakeCommand == false )
		{
			intakeButtonRelease = true;
		}

		if ( intakeOn == true )
		{
			motor[ intakeMot ] = intakeSpeed;
		}

		if ( intakeOn == false )
		{
			motor [ intakeMot ] = 0;
		}


		if ( conveyorUpCommand == false && conveyorDownCommand == false )
		{
			motor[conveyorMot ] = 0;
		}
		else if ( conveyorUpCommand == true && conveyorDownCommand == true )
		{
			motor[ conveyorMot ] = 0;
		}
		else if ( conveyorUpCommand == true )
		{
			motor[ conveyorMot ] = conveyorSpeed;
		}
		else if (conveyorDownCommand == true )
		{
			motor[ conveyorMot ] = -conveyorSpeed;
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
