#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // I add
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define PORT 8080

int main()
{

    char buf[256] = {0};
    socklen_t len;
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    char buffer[1024] = {0};
    // make socket server
    int socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server == -1)
    {
        printf("socket error");
        exit(EXIT_FAILURE);
    }
    int t = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int)) == -1)
    {
        printf("setsockpot error this");
        exit(1);
    } // bind the socket server
    if (bind(socket_server, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        printf("bind error\n");
        printf("%d\n", errno);
        exit(EXIT_FAILURE);
    }

    listen(socket_server, 10)==-1;
    printf("the server is runnig..\n");

    // change the CC
    for (int i = 0; i < 2; i++)
    {   memset(buf,0,sizeof(buf));
        // if (i == 1)
        // {
        //     strncpy(buf ,"cubic ",6);
        // }
        // else
        // {
        //     strncpy(buf ,"reno  ",6);
        // }
        // this the code from the word Ex4 that we can use
        len = sizeof(buf);
        if (getsockopt(socket_server, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
        {
            perror("getsockopt this?");
            return -1;
        }

        printf("Current: %s\n", buf);
        if(i==1){
        
        len = strlen(buf);
        if (setsockopt(socket_server, IPPROTO_TCP, TCP_CONGESTION, "reno", len) != 0)
        {
            perror("setsockopt");
            return -1;
        }
        len = sizeof(buf);
        if (getsockopt(socket_server, IPPROTO_TCP, TCP_CONGESTION, buf, &len) != 0)
        {
            perror("getsockopt");
            return -1;
        }
        }
        printf("New: %s\n", buf);
        double averge = 0;
        // get the file and get the averge time
        for (int j = 0; j < 5; j++)
        {

            int sender_socket = accept(socket_server, 0, 0);
            if (sender_socket == -1)
            {
                printf("connect error");
                exit(EXIT_FAILURE);
            }

            printf("read in the %d time\n", j);
            char buffer[1024] = {0};

            struct timespec start,end;
            clock_gettime(CLOCK_REALTIME, &start);
            int count=1;
            while (count!= 0){
                count=recv(sender_socket, buffer, 1024, 0) ;
            }
            clock_gettime(CLOCK_REALTIME, &end);
            double dif = (end.tv_sec -start.tv_sec)+(end.tv_nsec -start.tv_nsec)/1000000000.0;
            averge += dif;
        }
        if (i == 0)
        {
            printf("cubic ");
        }
        else
        {
            printf("reno ");
        }
        printf("took %f seconds \n", averge/5.0);
        
        sleep(1);
    }
    

    return 0;
}