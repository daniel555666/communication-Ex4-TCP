#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // I add
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#define PORT 8080
#define SERVER_IP "127.0.0.1"


int main()
{
    socklen_t len;
    struct sockaddr_in server_address;
    inet_pton(AF_INET, (const char *)SERVER_IP, &server_address.sin_addr);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    char buffer[1024];
        char buf[256] = {0};
    int p=0;


    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket == -1)
            {
                printf("socket error");
                exit(EXIT_FAILURE);
            }
            if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
            {
                printf("error connect");
                exit(EXIT_FAILURE);
            }
             if (i == 1)
        {
            strcpy(buf ,"cubic");
        }
        else
        {
            strcpy(buf ,"reno");
        }
            // this the code from the word Ex4 that we can use
            len = sizeof(buf);
            if (getsockopt(client_socket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
            {
                perror("getsockopt");
                return -1;
            }

            printf("Current: %s\n", buf);

            len = strlen(buf);
            if (setsockopt(client_socket, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0)
            {
                perror("setsockopt");
                return -1;
            }
            len = sizeof(buf);
            if (getsockopt(client_socket, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
            {
                perror("getsockopt");
                return -1;
            }
            printf("New: %s\n", buf);

            FILE *fp;
            fp = fopen("m.txt", "r");
            p++;
            int count;
            while ( count=fread(buffer,1,sizeof(buffer),fp)>0 )
            {  
            }
            printf("send the %d time", p );
            close(client_socket);
        }
    }

    return 0;
}

