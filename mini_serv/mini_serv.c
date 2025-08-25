#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include <netinet/ip.h>

int count = 0;
int max_fd = 0;
int ids[65536];
char *msgs[65536];

fd_set a_fds, r_fds, w_fds;

char buf_write[1024];
char buf_read[1024];

// get extract_message and str_join from main.c

void error() 
{
    write(1, "Fatal error\n", 12);
    exit(1);
}

void notify(int aut, char *str) 
{
    for(int fd = 0; fd <= max_fd; fd++ )
        if(FD_ISSET(fd, &w_fds) && fd != aut)
            send(fd, str, strlen(str), 0);
}

void add_client(int fd) 
{
	if(max_fd < fd)
		max_fd = fd;

	FD_SET(fd, &a_fds);
	msgs[fd] = NULL;
	ids[fd] = count++;
	sprintf(buf_write, "server: client %d just arrived\n", ids[fd]);
	notify(fd, buf_write);
}

void remove_client(int fd) 
{
	sprintf(buf_write, "server: client %d just left\n", ids[fd]);
	notify(fd, buf_write);
	FD_CLR(fd, &a_fds);
	free(msgs[fd]);
	close(fd);
}

void send_str(int fd) 
{
	char *str;

	while(extract_message(&(msgs[fd]), &str)) 
    {
		sprintf(buf_write, "client %d: ", ids[fd]);
		notify(fd, buf_write);
		notify(fd, str);
		free(str);
	}
}

int init_socket() 
{
	max_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(max_fd < 0)
		error();
	FD_SET(max_fd, &a_fds);
	return(max_fd);
}

int main(int ac, char **av)
{
	if(ac != 2) 
    {
		write(1, "Wrong number of arguments\n", 26);
		return(1);
	}
	int sockfd = init_socket();
	struct sockaddr_in servaddr;

	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433);
	servaddr.sin_port = htons(atoi(av[1])); 
  
	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
		error();
    if(listen(sockfd, 10))
        error();

	while(1)
	{
		r_fds = w_fds = a_fds;

		if(select(max_fd + 1, &r_fds, &w_fds, NULL, NULL) < 0)
			error();

		for(int fd = 0; fd <= max_fd; fd++ )
		{
			if(!FD_ISSET(fd, &r_fds))
				continue;
                
			if(fd == sockfd)
			{
				socklen_t len = sizeof(servaddr);
				int client_id = accept(sockfd, (struct sockaddr *)&servaddr, &len);
				if(client_id >= 0)
				{
					add_client(client_id);
					break;
				}
			}
			else
			{
				int bytes = recv(fd, buf_read, 1000, 0);
				if(bytes <= 0)
				{
					remove_client(fd);
					break;
				}
				buf_read[bytes] = '\0';
				msgs[fd] = str_join(msgs[fd], buf_read);
				send_str(fd);
			}
		}
	}
	return(0);
}