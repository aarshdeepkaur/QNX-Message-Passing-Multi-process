#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <process.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include "../../des_controller/src/des.h"


int main()  {

	int displayPid = getpid();
	printf("The display is running as PID: %d\n", displayPid);
	int rc;
	int chid;
	int bL = FALSE;
	int reply = 0;
	Display message;


	chid = ChannelCreate(0);
	if (chid == -1)
	{
		perror("failed to create the channel.");
		exit(EXIT_FAILURE);
	}


	while (1)
	{
		if (bL == TRUE)
			break;
		rc = MsgReceive(chid, &message, sizeof(message), NULL);
		if (rc == -1) {
			perror("failed to receive message from controller");
			exit(EXIT_FAILURE);
		}

		MsgReply(rc, EOK, &reply, sizeof(reply));

		switch (message.outputMessage)
		{

		case EXIT:
			printf("%s\n", outMessage[EXIT]);
			bL = TRUE;
			break;

		case DISPLAY_PERSON_ID:
			printf("%s%d\n", outMessage[DISPLAY_PERSON_ID], message.person.id);
			break;

		case DISPLAY_PERSON_WEIGHT:
			printf("%s%d\n", outMessage[DISPLAY_PERSON_WEIGHT], message.person.weight);
			break;

		default:
			printf("%s\n", outMessage[message.outputMessage]);
			break;
		}
	}



	if (ChannelDestroy(chid) == -1)
	{
		printf("destroy Failed");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
