/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define FILENAME "server.c" /* For error tracking */
#define MAXDATASIZE 100		/* max number of bytes we can get at once */
#include <stdio.h>
/* get sockaddr, IPv4 or IPv6: */
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in *)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void error_exit(const char *string)
{
	printf("/%s:ERROR: %s\n", FILENAME, string);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN],
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
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(addr, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and connect to the first we can */
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			  s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); /* all done with this structure */

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1)
	{
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';

	printf("client: received '%s'\n", buf);

	/* TODO */
	printf("> ");
	fgets(buf, MAXDATASIZE, stdin);

	strcpy(error, "Tried to send message but failed.");
	if (send(sockfd, buf, strlen(buf), 0) == -1)
		error_exit(error);		

	close(sockfd);

	return 0;
}
