#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define BUFLEN 1024

int main(int argc, char *argv[])
{
    // run us.c and ur.c
    int port, i;
    if (argc != 2)
    {
        printf("No ports specified...\n");
        printf("Usage: %s <port>\n", argv[0]);
        printf("Enter the port number to listen to: ");
        scanf("%d", &port);
    }
    else
        port = atoi(argv[1]);

    char portStr[13];
    sprintf(portStr, "%d", port);
    char sendFilename[20];
    char receiveFilename[20];
    strcpy(sendFilename, portStr);
    strcpy(receiveFilename, portStr);
    strcat(sendFilename, "Sent.txt");
    strcat(receiveFilename, "Receive.txt");
    char *prgrms[] = {"./us", "./ur"};
    char *arg[13];

    FILE *fps = fopen(sendFilename, "w");
    if (fps == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fclose(fps);
    int pid[2];
    for (i = 0; i < 2; i++)
    {
        char portStr[10];
        sprintf(portStr, "%d", port);
        arg[0] = prgrms[i];
        arg[1] = portStr;
        arg[2] = NULL;
        pid[i] = fork();
        if (pid[i] >= 0)
        {
            if (pid[i] == 0)
            {
                printf("");
                execvp(prgrms[i], (char **)arg);
                return 0;
            }
        }
        else
        {
            printf("Fork failed\n");
            exit(1);
        }
    }
    // wait for ur.c to finish
    // pid = fork();
    // if (pid >= 0)
    // {
    //     if (pid == 0)
    //     {
    //         execvp("./ur", arg);
    //         return 0;
    //     }
    // }
    // else
    // {
    //     printf("fork failed\n");
    //     exit(1);
    // }

    int choice = 0;
    do
    {
        printf("Press 1 to send message \n");
        printf("Press 2 to get received \n");
        printf("Press 3 to exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        // save the message to the file
        case 1:
            int port1;
            char message[BUFLEN], temp[BUFLEN];
            printf("Enter the port to send the message to: ");
            scanf("%d", &port1);
            getchar();
            sprintf(message, "%d", port1);
            printf("Enter the message to send: ");
            fgets(temp, BUFLEN, stdin);
            strcat(message, " ");
            strcat(message, temp);
            FILE *fps = fopen(sendFilename, "w");
            if (fps == NULL)
            {
                printf("Error opening file!\n");
                break;
            }
            fprintf(fps, "%s", message);
            fclose(fps);
            // clear the buffer
            // memset(message, 0, BUFLEN);
            // memset(temp, 0, BUFLEN);
            break;
        case 2:
            // read from file where PORT number is port[0]
            printf("The file name is %s\n", receiveFilename);
            FILE *fpr = fopen(receiveFilename, "r");
            if (fpr == NULL)
            {
                printf("Error opening file!\n");
                break;
            }
            char rmessage[BUFLEN];
            while (fgets(rmessage, BUFLEN, fpr) != NULL)
            {
                printf("%s", rmessage);
            }
            fclose(fpr);
            fpr = fopen(receiveFilename, "w");
            fclose(fpr);
            break;
        case 3:
            // kill all the child processes
            int ret;
            for (i = 0; i < 2; i++)
            {
                ret = kill(pid[i], SIGKILL);
                if (ret == -1)
                {
                    perror("kill failed");
                    exit(EXIT_FAILURE);
                }
            }
            system("rm -rf *.txt");
            exit(0);
            break;
        default:
            printf("Invalid choice\n");
            break;
        }
    } while (choice != 3);

    return 0;
}