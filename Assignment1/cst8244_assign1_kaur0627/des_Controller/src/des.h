#ifndef DOOR_ENTRY_SYSTEM_H_
#define DOOR_ENTRY_SYSTEM_H_
#define TRUE 1
#define FALSE 0

#define NUM_STATES 9

typedef enum
{
	START_STATE = 0,
	SCAN_STATE = 1,
	ENTER_UNLOCK_STATE = 2,
	ENTER_OPEN_STATE = 3,
	ENTER_CLOSED_STATE = 4,
	ENTER_LOCKED_STATE = 5,
	LEAVE_UNLOCK_STATE = 6,
	LEAVE_CLOSED_STATE = 7,
	EXIT_STATE = 8
} State;

#define NUM_INPUTS 12

typedef enum
{
	INPUT_LEFT_SCAN = 0,
	INPUT_RIGHT_SCAN = 1,
	INPUT_GUARD_LEFT_UNLOCK = 2,
	INPUT_GUARD_RIGHT_UNLOCK = 3,
	INPUT_LEFT_OPEN = 4,
	INPUT_RIGHT_OPEN = 5,
	INPUT_WEIGHED = 6,
	INPUT_LEFT_CLOSE = 7,
	INPUT_RIGHT_CLOSE = 8,
	INPUT_GUARD_LEFT_LOCK = 9,
	INPUT_GUARD_RIGHT_LOCK = 10,
	INPUT_EXIT = 11
} Input;

const char *inMessage[NUM_INPUTS] =
{
		"ls",
		"rs",
		"ws",
		"lo",
		"ro",
		"lc",
		"rc",
		"gru",
		"grl",
		"gll",
		"glu",
		"exit"
};

#define NUM_OUTPUTS 14
typedef enum
{
	GET_ID = 0,
	DISPLAY_PERSON_ID = 1,
	LEFT_GUARD_UNLOCK = 2,
	LEFT_OPEN = 3,
	GET_WEIGHT = 4,
	DISPLAY_PERSON_WEIGHT = 5,
	LEFT_CLOSE = 6,
	LEFT_GUARD_LOCK = 7,
	RIGHT_GUARD_UNLOCK = 8,
	RIGHT_OPEN = 9,
	RIGHT_CLOSE = 10,
	RIGHT_GUARD_LOCK = 11,
	PERSON_WAITING = 12,
	EXIT = 13
} Output;

const char *outMessage[NUM_OUTPUTS] =
{
		"Enter the Person's ID:",
		"Person scanned ID, ID = ",
		"Left door unlocked by Guard",
		"Person opened left door",
		"Enter the Person's weight:",
		"Person weighed, Weight = ",
		"Left door closed (automatically)",
		"Left door locked by Guard",
		"Right door unlocked by Guard",
		"Person opened right door",
		"Right door closed (automatically)",
		"Right door locked by Guard",
		"Waiting for Person",
		"Exiting Display"
};

typedef struct
{
	int id;
	int weight;
	int direction;
} Person;

typedef struct
{
	int outputMessage;
	Person person;
} Display;

#endif

