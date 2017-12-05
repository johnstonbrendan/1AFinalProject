// All functions in file except zero written by Niel Mistry
typedef struct
{
	// Fretting Variables
	tMotor pulleyMotor;
	tSensors touchPort;
	char currentNote;
	char currentPosition;

	// Strumming Variables
	tMotor strummingMotor;
	int parity;			// Sets direction of strumming motor
} Line;

void updateCurrentNote(Line & object_A, Line & object_B, char note_A, 
	char note_B)
{
	object_A.currentNote = note_A;
	object_B.currentNote = note_B;
}

void updateCurrentPosition(Line & object_A, Line & object_B)
{
	if(object_A.currentNote != '-')
		object_A.currentPosition = object_A.currentNote;

	if(object_B.currentNote != '-')
		object_B.currentPosition = object_B.currentNote;
}

void initializeLine (Line & object, tMotor pulleyMotor, tSensors touchPort, 
	tMotor strummingMotor)
{
	object.pulleyMotor = pulleyMotor;
	object.strummingMotor = strummingMotor;
	object.touchPort = touchPort;
	object.currentNote = '-';
	object.currentPosition = 'A';
	object.parity = -1;

	SensorType[object.touchPort] = sensorEV3_Touch;
}

void switchParity (Line & object)
{
	object.parity *= -1;
}

// Written by Ben Duo
void zero(Line & A, Line & B)
{
	const int MOVE_POWER = 20;
	const int TIME_DELAY = 1000; 
	motor[A.pulleyMotor]=motor[B.pulleyMotor] = MOVE_POWER;
	motor[A.strummingMotor] = A.parity * MOVE_POWER;
	motor[B.strummingMotor] = B.parity * MOVE_POWER;
	time1[T2] = 0;

	while(SensorValue[A.touchPort] == 0 || SensorValue[B.touchPort] == 0 ||
	 time1[T2] < TIME_DELAY)
	{
		if(SensorValue[A.touchPort] == 1)
			motor[A.pulleyMotor] = 0;

		if(SensorValue[B.touchPort] == 1)
			motor[B.pulleyMotor] = 0;

		if(time1[T2] >= TIME_DELAY)
			motor[A.strummingMotor] = motor[B.strummingMotor]  = 0;

	}
	motor[A.pulleyMotor] = motor[B.pulleyMotor] = motor[A.strummingMotor] 
		= motor[B.strummingMotor] = 0;

	switchParity(A);
	switchParity(B);
}
