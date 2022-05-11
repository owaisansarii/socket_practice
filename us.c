#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// #define REMOTEPORT 2002
#define BUFLEN 1024

int main(int argc, char *argv[])
{
    int sockfd;
    char portStr[13];
    int port;
    if (argc != 2)
    {
        printf("No ports specified...\n");
        printf("Exiting you won't be able to send message...\n");
        exit(1);
    }
    else
        port = atoi(argv[1]);

    sprintf(portStr, "%d", port);
    // char *messageStr = "very important Good Morning message with 100s of emojis!!!";

    FILE *fp;
    strcat(portStr, "Sent.txt");
    printf("%s\n", portStr);
    // char line[BUFLEN];
    while (1)
    {
        fp = fopen(portStr, "r");
        if (fp == NULL)
        {
            printf("Error in reading message to be sent!\n");
            exit(1);
        }
        char buffer[BUFLEN];
        fgets(buffer, BUFLEN, fp);
        int msgLen = strlen(buffer);
        char *firstWord, *context, *remainder;
        char *msgCopy = (char *)calloc(msgLen, sizeof(char));
        strncpy(msgCopy, buffer, msgLen);
        firstWord = strtok_r(msgCopy, " ", &context);
        remainder = context;
        if ((firstWord != NULL) && (remainder != NULL))
        {
            struct sockaddr_in receiverAddr;
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
            {
                perror("Socket failed");
                exit(EXIT_FAILURE);
            }

            memset(&receiverAddr, 0, sizeof(receiverAddr));
            receiverAddr.sin_family = AF_INET;
            receiverAddr.sin_port = htons(atoi(firstWord));
            receiverAddr.sin_addr.s_addr = INADDR_ANY;
            // int n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));
            int n = sendto(sockfd, remainder, strlen(remainder), 0, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr));
            if (n < 0)
            {
                perror("Send message failed: ");
            }
            // clear the buffer
            memset(buffer, 0, BUFLEN);
            // close the socket
            close(sockfd);
        }
        fclose(fp);
        fp = fopen(portStr, "w");
        if (fp == NULL)
        {
            printf("Error clearing sent message!\n");
            exit(1);
        }
        fclose(fp);
    }
    // for closing file and socket if some unexpected error occurs
    close(sockfd);
    fclose(fp);
    return 0;
}
