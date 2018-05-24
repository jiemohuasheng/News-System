#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        printf(" (Use loop back address: 127.0.0.1)\n");
        return 1;
    }

   /*initialize all bits of "recvBuff" to 0*/
    memset(recvBuff, '0',sizeof(recvBuff));

    //TODO: create a socket here.

	/*initialize all bits of "serv_addr" to 0*/
    memset(&serv_addr, '0', sizeof(serv_addr));

    //TODO: assign the values for the members "sin_family"
    //TODO: and "sin_port" of the "serv_addr" structure here.

    //TODO: call connect() here, to establish the connection.

    //TODO: read characters from the "sockfd" to "recvBuff"

    //TODO: print the string to "stdout".

    //TODO: try to print error messages for possible errors

    return 0;
}