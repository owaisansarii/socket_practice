#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFLEN 1024
#define LISTENPORT 1234

int main(int argc, char *argv[])
{
    int sockfd, len, port;
    char buffer[BUFLEN];
    if (argc != 2)
    {
        printf("No ports specified...\n");
        printf("Exiting you won't be able to receive message...\n");
        exit(1);
    }
    port = atoi(argv[1]);
    struct sockaddr_in receiverAddr, senderAddr;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket system call failed");
        exit(EXIT_FAILURE);
    }

    memset(&receiverAddr, 0, sizeof(receiverAddr));
    memset(&senderAddr, 0, sizeof(senderAddr));

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    receiverAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&receiverAddr, sizeof(receiverAddr)) < 0)
    {
        perror("bind system call failed");
        exit(EXIT_FAILURE);
    }
    len = sizeof(receiverAddr);
    FILE *fp;
    char filename[20];
    sprintf(filename, "%d", port);
    strcat(filename, "Receive.txt");
    fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening file in us!\n");
        exit(1);
    }
    fclose(fp);
    int read_size; // number of bytes read
    while (read_size = recvfrom(sockfd, buffer, BUFLEN, 0, (struct sockaddr *)&senderAddr, &len))
    {
        buffer[read_size] = '\0';
        // printf("filename: %s\n", filename);
        fp = fopen(filename, "a");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        // printf("Message recieved: %s\n", buffer);
        // write to file
        fprintf(fp, "%s", buffer);
        fclose(fp);
    }
    close(sockfd);
    return 0;
}
