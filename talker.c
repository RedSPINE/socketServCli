/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define FILENAME "talker.c"
#define SERVERPORT "4950" /* the port users will be connecting to */
#define MAXDATASIZE 255   /* max number of bytes we can get at once */

void error_exit(const char *string)
{
	printf("/%s:ERROR: %s\n", FILENAME, string);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char buf[MAXDATASIZE],
		error[255], /* Error to output */
		*port,		/* meant to hold PORT value if none is specified through argv */
		*addr;		/* address you want to connect to */

	/* Default init */
	port = NULL; /* NOT OPTIONAL */
	addr = NULL; /* NOT OPTIONAL */

	/* Handle entries */

	strcpy(error, "unexpected syntax.\n"
				  "Specifying an address and a port is mandatory.\n"
				  "EXPECTED: ./a.out -h <ADDR>:<PORT>");

	if (argc > 1)
	{
		int i;
		char buf[255];
		const char s[2] = ":";

		for (i = 1; i < argc + 1; ++i)
		{
			if (argv[i][0] != '-')
				break;

			if (strlen(argv[i]) > 2)
				error_exit(error);

			switch (argv[i][1])
			{
			case 'h':
				if (argc < i + 2)
					error_exit(error);
				strcpy(buf, argv[i + 1]);
				addr = strtok(buf, s);
				port = strtok(NULL, s);
				break;
			default:
				break;
			}
		}
	}

	/* Be sure we have both a port and an address to connect to */
	if (!addr || !port)
		error_exit(error);

	printf("Used address : %s\n", addr);
	printf("Used port : %s\n", port);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(addr, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and make a socket */
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "talker: failed to create socket\n");
		return 2;
	}

	printf("> ");
	fgets(buf, MAXDATASIZE, stdin);

	strcpy(error, "Tried to send message but failed.");
	if (sendto(sockfd, buf, strlen(buf), 0, p->ai_addr, p->ai_addrlen) == -1)
		error_exit(error);

	freeaddrinfo(servinfo);

	printf("talker: sent %ld bytes to %s\n", strlen(buf), argv[1]);
	close(sockfd);

	return 0;
}
