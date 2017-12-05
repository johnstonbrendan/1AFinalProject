#include "EV3_FileIO.c"
#include "lineStruct.c"
#include "strummer.c"
#include "fretter.c"

// Written by Brendan Johnston and Niel Mistry
task main()
{
	const int LONG_TAB = 100; // Length of longest file possible
	TFileHandle fin_A;
	TFileHandle fin_B;
	bool fileOkayA = openReadPC(fin_A, "Play_me_A.txt");
	bool fileOkayB = openReadPC(fin_B, "Play_me_B.txt");
	if(!fileOkayA || !fileOkayB)
	{
		displayString(0, "Failed to Open File");

		//open file and read  (file is in a string)
		displayString(1, "I'm not going to let you go on");
		displayString(2, "Force terminate me plz");
		while(true)
		{}
	}

	string song_name = "Name";

	Line A;
	Line B;
	char NoteSeqA[LONG_TAB];
	char NoteSeqB[LONG_TAB];

	for(int i = 0; i < LONG_TAB; i++) // initialize arrays
	{
		NoteSeqA[i] = 0;
		NoteSeqB[i] = 0;
	}

	for (int seqIndex = 0; seqIndex < LONG_TAB; seqIndex++) // read in values
	{
		readCharPC(fin_A, NoteSeqA[seqIndex]);
		readCharPC(fin_B, NoteSeqB[seqIndex]);
	}

	initializeLine(A, motorC, S1, motorA);
	initializeLine(B, motorD, S2, motorB);

	zero(A,B);

	while(!getButtonPress(buttonAny))
	{}

	while(getButtonPress(buttonAny))
	{}

	eraseDisplay();
	displayString(0, "Now Playing:");
	displayString(1,"%s",song_name);

	int current = 0;

	while (current < LONG_TAB && NoteSeqA[current] != '|'  
		&& NoteSeqB[current] != '|')
		// keeps going through array until its hit max size or | character
	{
		updateCurrentNote(A, B, NoteSeqA[current], NoteSeqB[current]);
		moveFrets(A,B);
		strum (A,B);

		updateCurrentPosition(A,B);

		current++;
	}


	displayString(0, "That is the end of the song:");
	displayString(2, "Feel free to terminate me now", song_name);

	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}

	muted_reset (A,B);

	motor[A.strummingMotor] = motor[B.strummingMotor] = 0;

	displayString(0,"Thanks :)");

	wait1Msec(5000);
}
