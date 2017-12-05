const int STRUM_POWER = 75;
const int STRUM_WAIT_TIME = 100;
const int DEGREE_OF_ROTATION = 45;

int setAngleToMuteA(Line const & A)
{
	if(A.parity == -1)
	{
		return 36;
	}

	return 40;
}

int setAngleToMuteB(Line const & B)
{
	if(B.parity == -1)
	{
		return 38;
	}

	return 42;
}

// Written by Niel Mistry
void strum(Line & A, Line & B)
{
	const int STRUM_TIME = 100;
	bool aIsRunning = false;
	bool bIsRunning = false;

	if(A.currentNote != '-')
	{
		motor[A.strummingMotor] = A.parity * STRUM_POWER;
		switchParity(A);
		aIsRunning = true;
	}
	if(B.currentNote != '-')
	{
		motor[B.strummingMotor] = B.parity * STRUM_POWER;
		switchParity(B);
		bIsRunning = true;
	}
	time1[T1] = 0;
	while((aIsRunning || bIsRunning) && time1[T1] < STRUM_TIME)
	{}
	motor[A.strummingMotor] = motor[B.strummingMotor] = 0;

	resetMotorEncoder(A.strummingMotor);
	resetMotorEncoder(B.strummingMotor);

	wait1Msec(STRUM_WAIT_TIME);
}

// From here below - Written by Brendan Johnston
void toggleMute(Line&A, Line&B)
{
	int angleToMoveA = 0;
	int angleToMoveB = 0;
	bool aIsRunning = false , bIsRunning = false;
	resetMotorEncoder(A.strummingMotor);
	resetMotorEncoder(B.strummingMotor);
	if (A.currentNote != '-' && A.currentPosition != A.currentNote)
	{
		angleToMoveA = setAngleToMuteA(A);
		motor[A.strummingMotor] = A.parity*STRUM_POWER;
		switchParity(A);
		aIsRunning = true;
	}
	if (B.currentNote != '-' && B.currentPosition != B.currentNote)
	{
		angleToMoveB = setAngleToMuteB(B);
		motor[B.strummingMotor] = B.parity*STRUM_POWER;
		switchParity(B);
		bIsRunning = true;
	}


	while ((aIsRunning && abs(nMotorEncoder[A.strummingMotor]) < angleToMoveA)
	 || (bIsRunning && abs(nMotorEncoder[B.strummingMotor]) < angleToMoveB))
	{
		if (abs(nMotorEncoder[A.strummingMotor]) >= angleToMoveA)
		{
			motor[A.strummingMotor] = 0;
		}
		if (abs(nMotorEncoder[B.strummingMotor]) >= angleToMoveB)
		{
			motor[B.strummingMotor] = 0;
		}
	}
	motor[A.strummingMotor] = 0;
	motor[B.strummingMotor] = 0;
}

void muted_reset(Line&A, Line&B)
{
	resetMotorEncoder(A.strummingMotor);
	resetMotorEncoder(B.strummingMotor);
	updateCurrentNote(A,B,'|','|');

	bool aIsRight = false;
	bool bIsRight = false;

	if (A.parity < 0)
	{
		motor[A.strummingMotor] = A.parity*STRUM_POWER;
		switchParity(A);
		aIsRight = true;
	}
	if (B.parity < 0)
	{
		motor[B.strummingMotor] = B.parity*STRUM_POWER;
		switchParity(B);
		bIsRight = true;
	}
	while(aIsRight&&(abs(nMotorEncoder[A.strummingMotor])<DEGREE_OF_ROTATION) 
		||bIsRight&&(abs(nMotorEncoder[B.strummingMotor])<DEGREE_OF_ROTATION))
		//spaces removed to fit onto paper without awkward cutting
	{
		if(abs(nMotorEncoder[A.strummingMotor]) >= DEGREE_OF_ROTATION)
		{
			motor[A.strummingMotor] = 0;
		}
		if(abs(nMotorEncoder[B.strummingMotor]) >= DEGREE_OF_ROTATION)
		{
			motor[B.strummingMotor] = 0;
		}
	}
	motor[A.strummingMotor] = 0;
	motor[B.strummingMotor] = 0;
	toggleMute(A,B);
}
