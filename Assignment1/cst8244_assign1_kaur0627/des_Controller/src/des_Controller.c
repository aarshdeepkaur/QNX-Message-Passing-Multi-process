#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include "des.h"

void start();
void scan();
void enterOpen();
void enterClosed();
void enterLocked();
void enterUnlock();
void leaveClosed();
void leaveUnlock();
void exitState();

void (*stateArray[NUM_STATES])()=
{
	start, scan,
    enterOpen, enterClosed,
	enterLocked, enterUnlock,
	leaveClosed, leaveUnlock,
	exitState

	};

	int rc;
	int chid;
	pid_t displayPid;
	int coid;
	Person currentUser;
	Display displayMessage;
	int rmsg;
	int reply = 0;

	int main(int argc, char* argv[])
	{
		printf("The Controller is running as PID: %d\n", getpid());

		displayPid = atoi(argv[1]);

		chid = ChannelCreate(0);
		if (chid == -1)
		{
			perror("failed to create the channel.");
			exit(EXIT_FAILURE);
		}

		coid = ConnectAttach(ND_LOCAL_NODE, displayPid, 1, _NTO_SIDE_CHANNEL,
				0);
		if (coid == -1)
		{
			printf(
					"error)", displayPid);
			exit(EXIT_FAILURE);
		}
		currentUser.direction = -1;
		currentUser.id = -1;
		currentUser.weight = -1;
		while (TRUE) {
			(*stateArray[START_STATE])();
		}
		printf("Exiting Controller");
		return EXIT_SUCCESS;
	}


	void start()
	{
		displayMessage.person = currentUser;
		displayMessage.outputMessage = PERSON_WAITING;
		if (MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
				sizeof(rmsg)) == 1L) {
			fprintf(stderr, "MsgSend had an error.");
			exit(EXIT_FAILURE);
		}

		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));

		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (currentUser.id != -1)
		{
			printf("Already in use");
		}

		if (message == INPUT_EXIT)
		{
			(*stateArray[EXIT_STATE])();
		}

		switch (message)
		{

		case INPUT_LEFT_SCAN:
			displayMessage.outputMessage = GET_ID;

			if (MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg)) == 1L) {
				fprintf(stderr, "MsgSend had an error.");
				exit(EXIT_FAILURE);
			}

			rc = MsgReceive(chid, &message, sizeof(message), NULL);
			MsgReply(rc, EOK, &reply, sizeof(reply));
			currentUser.id = message;
			displayMessage.person = currentUser;
			displayMessage.outputMessage = DISPLAY_PERSON_ID;

			if (MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg)) == 1L) {
				fprintf(stderr, "MsgSend had an error.");
				exit(EXIT_FAILURE);
			}

			currentUser.direction = INPUT_LEFT_SCAN;
			(*stateArray[SCAN_STATE])(chid, rc);
			break;

		 case INPUT_RIGHT_SCAN:
			displayMessage.outputMessage = GET_ID;

			if (MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg)) == 1L)
			{
				fprintf(stderr, "MsgSend had an error.");
				exit(EXIT_FAILURE);
			}

			rc = MsgReceive(chid, &message, sizeof(message), NULL);
			MsgReply(rc, EOK, &reply, sizeof(reply));
			currentUser.id = message;
			displayMessage.person = currentUser;
			displayMessage.outputMessage = DISPLAY_PERSON_ID;

			if (MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg)) == 1L)
			{
				fprintf(stderr, "MsgSend had an error.");
				exit(EXIT_FAILURE);
			}

			currentUser.direction = INPUT_RIGHT_SCAN;
			(*stateArray[SCAN_STATE])(chid, rc);
			break;
		default:
			(*stateArray[SCAN_STATE])();
		}
	}

	void scan()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
					(*stateArray[EXIT_STATE])();
		switch (currentUser.direction)
		{
		case INPUT_LEFT_SCAN:
			switch (message)
			{
			case INPUT_GUARD_LEFT_UNLOCK:
				printf("i am in INPUT_GUARD_LEFT_UNLOCK\n");
				displayMessage.outputMessage = LEFT_GUARD_UNLOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_UNLOCK_STATE])();
				break;
			}

			break;
		case INPUT_RIGHT_SCAN:
			switch (message)
			{
			case INPUT_GUARD_RIGHT_UNLOCK:
				displayMessage.outputMessage = RIGHT_GUARD_UNLOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_UNLOCK_STATE])();
				break;
			}
			break;
		}
		(*stateArray[SCAN_STATE])();
	}

	void enterUnlock()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();
		switch (currentUser.direction)
		{

		case INPUT_LEFT_SCAN:
			if (message == INPUT_LEFT_OPEN)
			{
				displayMessage.outputMessage = LEFT_OPEN;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_OPEN_STATE])();
			}

			break;
		case INPUT_RIGHT_SCAN:
			if (message == INPUT_RIGHT_OPEN)
			{
				displayMessage.outputMessage = RIGHT_OPEN;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_OPEN_STATE])();
			}
			break;
		}
		(*stateArray[ENTER_UNLOCK_STATE])();
	}


	void enterOpen()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}
		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();
		if (message == INPUT_WEIGHED)
		{
			displayMessage.outputMessage = GET_WEIGHT;
			MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg));
			rc = MsgReceive(chid, &message, sizeof(message), NULL);
			MsgReply(rc, EOK, &reply, sizeof(reply));
			if (rc == -1)
			{
				perror("problem using MsgRecieve");
				exit(EXIT_FAILURE);
			}

			currentUser.weight = message;
			displayMessage.person = currentUser;
			displayMessage.outputMessage = DISPLAY_PERSON_WEIGHT;
			MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
					sizeof(rmsg));
			(*stateArray[ENTER_CLOSED_STATE])();
		}
		(*stateArray[ENTER_OPEN_STATE])();
	}


	void enterClosed()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();

		switch (currentUser.direction) {
		case INPUT_LEFT_SCAN:
			switch (message)
			{
		case INPUT_LEFT_CLOSE:
				displayMessage.outputMessage = LEFT_CLOSE;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_CLOSED_STATE])();
				break;

		case INPUT_GUARD_LEFT_LOCK:
				displayMessage.outputMessage = LEFT_GUARD_LOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_LOCKED_STATE])();
				break;
			}
			break;

		case INPUT_RIGHT_SCAN:
			switch (message)
			{
			case INPUT_RIGHT_CLOSE:
				displayMessage.outputMessage = RIGHT_CLOSE;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_CLOSED_STATE])();
				break;

		case INPUT_GUARD_RIGHT_LOCK:
				displayMessage.outputMessage = RIGHT_GUARD_LOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[ENTER_LOCKED_STATE])();
				break;
			}
		}
		(*stateArray[ENTER_CLOSED_STATE])();
	}


	void enterLocked()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));

		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();
		switch (currentUser.direction)
		{
		case INPUT_LEFT_SCAN:
			if (message == INPUT_GUARD_RIGHT_UNLOCK)
			{
				displayMessage.outputMessage = RIGHT_GUARD_UNLOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_UNLOCK_STATE])();
			}
			break;

		case INPUT_RIGHT_SCAN:
			if (message == INPUT_GUARD_LEFT_UNLOCK)
			{
				displayMessage.outputMessage = LEFT_GUARD_UNLOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_UNLOCK_STATE])();
			}

			break;
		}
		(*stateArray[ENTER_LOCKED_STATE])();
	}

	void leaveUnlock()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1)
		{
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();
		switch (currentUser.direction)
		{

		case INPUT_LEFT_SCAN:
			if (message == INPUT_RIGHT_OPEN)
			{
				displayMessage.outputMessage = RIGHT_OPEN;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_CLOSED_STATE])();
			}

			break;
		case INPUT_RIGHT_SCAN:
			if (message == INPUT_LEFT_OPEN)
			{
				displayMessage.outputMessage = LEFT_OPEN;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_CLOSED_STATE])();
			}

			break;
		}
		(*stateArray[LEAVE_UNLOCK_STATE])();
	}


	void leaveClosed()
	{
		int message;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		MsgReply(rc, EOK, &reply, sizeof(reply));
		if (rc == -1) {
			perror("problem using MsgRecieve");
			exit(EXIT_FAILURE);
		}

		if (message == INPUT_EXIT)
			(*stateArray[EXIT_STATE])();
		switch (currentUser.direction)
		{
		case INPUT_LEFT_SCAN:
			switch (message) {
			case INPUT_RIGHT_CLOSE:
				displayMessage.outputMessage = RIGHT_CLOSE;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_CLOSED_STATE])();
				break;
			case INPUT_GUARD_RIGHT_LOCK:
				currentUser.id = -1;
				displayMessage.outputMessage = RIGHT_GUARD_LOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[START_STATE])();
				break;
			}
			break;

		case INPUT_RIGHT_SCAN:
			switch (message)
			{
			case INPUT_LEFT_CLOSE:
				displayMessage.outputMessage = LEFT_CLOSE;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[LEAVE_CLOSED_STATE])();
				break;
			case INPUT_GUARD_LEFT_LOCK:
				currentUser.id = -1;
				displayMessage.outputMessage = LEFT_GUARD_LOCK;
				MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
						sizeof(rmsg));
				(*stateArray[START_STATE])();
				break;
			}
			break;
		}
		(*stateArray[LEAVE_CLOSED_STATE])();
	}


	void exitState()
	{
		displayMessage.outputMessage = EXIT;
		MsgSend(coid, &displayMessage, sizeof(displayMessage), &rmsg,
				sizeof(rmsg));
		exit(EXIT_SUCCESS);
	}
