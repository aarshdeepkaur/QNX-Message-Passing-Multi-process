#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <process.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <errno.h>
#include "../../des_controller/src/des.h"

int main(int argc, char* argv[])
  {

	int coid;
	pid_t controllerPid;
	char input[20];
	int reply;
	int message;

	if (argc < 2)
	{
		printf("Usage: ./des_inputs.c <des_controller PID>");
		exit(EXIT_FAILURE);
	}

	controllerPid = atoi(argv[1]);
	coid = ConnectAttach(ND_LOCAL_NODE, controllerPid, 1, _NTO_SIDE_CHANNEL,0);
	if (coid == -1)
	{
		printf(
				"error, as des_inputs can't connect attached to processID %d (slogger:0)\n",
				controllerPid);
		exit(EXIT_FAILURE);
	}

	while (TRUE)
	{
		printf(
				"Enter the event type (ls= left scan, rs= right scan, ws= weight scale, lo =left open, ro=right open, lc = left closed, rc = right closed, gru= guard rightunlock, grl= guard right lock, gll=guard left lock, glu= guardleft unlock)\n");
		scanf("%s", input);
		if (strcmp(input, inMessage[INPUT_LEFT_SCAN]) == 0)
		{
			message = INPUT_LEFT_SCAN;
			if (MsgSend(coid, &message, sizeof(message), &reply, sizeof(reply))
					== -1L) {
				fprintf(stderr, "MsgSend error.");
				exit(EXIT_FAILURE);
			}

		scanf(" %d", &message);
		}
		else if (strcmp(input, inMessage[INPUT_RIGHT_SCAN]) == 0)
		{
			message = INPUT_RIGHT_SCAN;
			if (MsgSend(coid, &message, sizeof(message), &reply, sizeof(reply))
					== -1L) {
				fprintf(stderr, "MsgSend error.");
				exit(EXIT_FAILURE);
			}

			scanf(" %d", &message);
		}
		else if (strcmp(input, inMessage[INPUT_GUARD_LEFT_UNLOCK]) == 0)
		{
			message = INPUT_GUARD_LEFT_UNLOCK;
		}
		else if (strcmp(input, inMessage[INPUT_GUARD_RIGHT_UNLOCK]) == 0)
		{
			message = INPUT_GUARD_RIGHT_UNLOCK;
		}
		else if (strcmp(input, inMessage[INPUT_LEFT_OPEN]) == 0)
		{
			message = INPUT_LEFT_OPEN;
		}
		else if (strcmp(input, inMessage[INPUT_RIGHT_OPEN]) == 0)
		{
			message = INPUT_RIGHT_OPEN;
		}
		else if (strcmp(input, inMessage[INPUT_WEIGHED]) == 0)
		{
			message = INPUT_WEIGHED;

			if (MsgSend(coid, &message, sizeof(message), &reply, sizeof(reply))
					== -1L)
			{
				fprintf(stderr, "MsgSend error.");
				exit(EXIT_FAILURE);
			}

			scanf(" %d", &message);
		}
		else if (strcmp(input, inMessage[INPUT_LEFT_CLOSE]) == 0)
		{
			message = INPUT_LEFT_CLOSE;
		}
		else if (strcmp(input, inMessage[INPUT_RIGHT_CLOSE]) == 0)
		{
			message = INPUT_RIGHT_CLOSE;
		}
		else if (strcmp(input, inMessage[INPUT_GUARD_LEFT_LOCK]) == 0) {
			message = INPUT_GUARD_LEFT_LOCK;
		}
		else if (strcmp(input, inMessage[INPUT_GUARD_RIGHT_LOCK]) == 0)
		{
			message = INPUT_GUARD_RIGHT_LOCK;
		}
		else if (strcmp(input, inMessage[INPUT_EXIT]) == 0)
		{
			message = INPUT_EXIT;

			if (MsgSend(coid, &message, sizeof(message), &reply, sizeof(reply))== -1L)
			{
				fprintf(stderr, "MsgSend error.");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}

		if (MsgSend(coid, &message, sizeof(message), &reply, sizeof(reply))
				== -1L) {
			fprintf(stderr, "MsgSend error.");
			exit(EXIT_FAILURE);
		}
	}

}
