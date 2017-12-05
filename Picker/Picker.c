typedef struct
{
	// Fretting Stuff
	tMotor pulleyMotor;
	tSensors touchPort;
	char currentNote;

	// Strumming Stuff
	tMotor strummingMotor;
	int parity;    // Left relative to motor while looking at it from behind
}Line;

void switchParity(Line & object)
{
	object.parity *= -1;
}
void updateNote(Line & object, char note)
{
	object.currentNote = note;
}

void initializeLine(Line & object, tMotor pulleyMotor, tSensors touchPort, tMotor strummingMotor)
{
	object.pulleyMotor = pulleyMotor;
	object.touchPort = touchPort;
	object.currentNote = '-';
	object.strummingMotor = strummingMotor;
	object.parity = 1;
}

void strum(Line a, Line g)
{
	bool aIsRunning = false;
	bool bIsRunning = false;
	if(a.currentNote != '-')
	{
		motor[a.strummingMotor] = a.parity * 75;
		switchParity(a);
		aIsRunning = true;
	}
	if(g.currentNote != '-')
	{
		motor[g.strummingMotor] = g.parity * 75;
		switchParity(g);
		bIsRunning = true;
	}
	while((aIsRunning && abs(nMotorEncoder[a.strummingMotor]) < 45) || bIsRunning && abs(nMotorEncoder[g.strummingMotor]) < 45)
	{
		if(abs(nMotorEncoder[a.strummingMotor]) >= 45)
		{
			motor[a.strummingMotor] = 0;
		}
		if(abs(nMotorEncoder[g.strummingMotor]) >= 45)
		{
			motor[g.strummingMotor] = 0;
		}
	}

	motor[a.strummingMotor] = motor[g.strummingMotor] = 0;

	resetMotorEncoder(a.strummingMotor);
	resetMotorEncoder(g.strummingMotor);
}


task main()
{
	Line a;
	Line g;

	initializeLine(a, 1, 1, motorA);
	initializeLine(g, 2, 2, motorB);

	while(true)
	{
		while(!getButtonPress(buttonAny))
		{}
		while(getButtonPress(buttonAny))
		{
			if(getButtonPress(buttonEnter))
			{
				updateNote(a, 'G');
			}
			if(getButtonPress(buttonLeft))
			{
				updateNote(g, 'C');
			}
			if(getButtonPress(buttonRight))
			{
				updateNote(a, '-');
			}
			if(getButtonPress(buttonDown))
			{
				updateNote(g, '-');
			}
		}

		strum(a, g);

	}

}
