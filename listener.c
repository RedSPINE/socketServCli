/*
** listener.c -- a datagram sockets "server" demo
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

#define PORT "4950" /* the port users will be connecting to */
#define FILENAME "listener.c"

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

int main(int argc, char **argv)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int i, numbytes;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	char buf[255];
	char *port;

	/* Default init */
	port = PORT;

	/* Handle entries */
	if (argc > 1)
	{
		int i;
		char *error;
		error = "unexpected syntax.\n"
				"EXPECTED: ./a.out [-p <PORT>]";

		for (i = 1; i < argc + 1; ++i)
		{
			if (argv[i][0] != '-')
				break;

			if (strlen(argv[i]) > 2)
				error_exit(error);

			switch (argv[i][1])
			{
			case 'p':
				if (argc < i + 2)
					error_exit(error);
				port = argv[i + 1];
				break;

			default:
				break;
			}
		}
	}

	printf("Used port : %s\n", port);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; /* set to AF_INET to force IPv4 */
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; /* use my IP */

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and bind to the first we can */
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL)
	{
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, 254, 0,
							 (struct sockaddr *)&their_addr, &addr_len)) == -1)
	{
		perror("recvfrom");
		exit(1);
	}

	if (numbytes > 1)
	{
		char c;
		int stringend = numbytes - 2;

		buf[numbytes - 1] = '\0'; /* smash \n */
		printf("Client sent : %s\n", buf);

		for (i = 0; i < (stringend + 1) / 2; i++)
		{
			c = buf[i];
			buf[i] = buf[stringend - i];
			buf[stringend - i] = c;
		}

		printf("Reversed string is : %s\n", buf);
	}
	else
		puts("Client disconnected without sending anything");

	close(sockfd);

	return 0;
}
