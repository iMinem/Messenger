//
//  client.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "client.h"
#include "functions.h"

#define IPLENGTH 16

void startClient()
{
    int sockfd;
    int portno;
    int test;
    int input;
    char firstLetter;
    char buffer[BUFFERSIZE];
    char address[IPLENGTH];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    printf("Please enter a server's IP-address.\n");
    getchar();
    bzero(address, IPLENGTH);
    fgets(address, IPLENGTH - 1, stdin);
    printf("Please enter a valid port number (choose a number between 2000 and 65535.\n");
    enterPort(&portno);
    printf("Connecting...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR: failed to open socket");
    }
    server = gethostbyname(address);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: no such host found");
        exit(0);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR: connecting failed");
    }
    else printf("Connected to host!\n");
    test = 1;
    while (test == 1)
    {
        printf("Choose action:\n");
        printf("(1) Receive messages\n");
        printf("(2) Type new message\n");
        printf("(3) Quit\n");
        scanf("%d", &input);
        switch (input)
        {
            case 1:
                readMessage(sockfd);
                break;
            case 2:
                printf("Please enter message:\n");
                firstLetter = getchar();
                bzero(buffer, BUFFERSIZE);
                fgets(buffer, BUFFERSIZE - 1, stdin);
                for (int i = strlen(buffer); i >= 0; i--)
                {
                    buffer[i + 1] = buffer[i];
                }
                buffer[0] = firstLetter;
                writeMessage(sockfd, buffer);
                break;
            case 3:
                test = 0;
                break;
            default:
                printf("Invalid input.\n");
                break;
        }
    }
}
