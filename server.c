/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"			/* the port users will be connecting to*/
#define FILENAME "server.c" /* For error tracking */
#define BACKLOG 10			/* how many pending connections queue will hold */

void sigchld_handler(__attribute__((unused)) int s)
{

	/* waitpid() might overwrite errno, so we save and restore it:*/
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0)
		;

	errno = saved_errno;
}

/* get sockaddr, IPv4 or IPv6:*/
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
	int sockfd, new_fd; /* listen on sock_fd, new connection on new_fd*/
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; /* connector's address information*/
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	char *port; /* meant to hold PORT value if none is specified through argv */

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
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; /* use my IP*/

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and bind to the first we can*/
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
							 p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
					   sizeof(int)) == -1)
		{
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); /* all done with this structure*/

	if (p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; /* reap all dead processes*/
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while (1)
	{ /* main accept() loop*/
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
				  get_in_addr((struct sockaddr *)&their_addr),
				  s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork())
		/* ---- CHILD PROCESS ---- */
		{
			char buffer[255];
			int numbytes;
			int i;

			close(sockfd); /* child doesn't need the listener*/
			if (send(new_fd, "Hello, world!", 13, 0) == -1)
				perror("send");

			if ((numbytes = recv(new_fd, buffer, 254, 0)) == -1)
			{
				perror("recv");
				exit(1);
			}

			if (numbytes > 1)
			{
				char c;
				int stringend = numbytes - 2;

				buffer[numbytes - 1] = '\0'; /* smash \n */
				printf("Client sent : %s\n", buffer);

				for (i = 0; i < (stringend + 1) / 2; i++)
				{
					c = buffer[i];
					buffer[i] = buffer[stringend - i];
					buffer[stringend - i] = c;
				}

				printf("Reversed string is : %s\n", buffer);
			}
			else
				puts("Client disconnected without sending anything");

			close(new_fd);
			exit(0);
		}
		close(new_fd); /* parent doesn't need this*/
	}

	return 0;
}
