#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

void	die(char *str)
{
	printf("%s\n", str);
	exit(-1);
}

void	func(int connfd)
{
	char	buff[80];
	int		n;

	while (1)
	{
		bzero(buff, sizeof(buff));
		read(connfd, buff, sizeof(buff));
		printf("read: %s\n", buff);
		bzero(buff, sizeof(buff));
		n = 0;
		while (buff[n++] = getchar() != '\n')
			;
		write(connfd, buff, sizeof(buff));
	}
}

int	main()
{
	int					sockfd;
	int					connfd;
	int					len;
	struct sockaddr_in	servaddr;
	struct sockaddr_in	cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		die("Error creating socket");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9000);
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		die("socket bind failed");
	if (listen(sockfd, 5) != 0)
		die("Listen failed");
	len = sizeof(cli);
	connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
		die("Accepting error");
	func(connfd);
}
