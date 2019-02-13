//
//  server.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "server.h"
#include "functions.h"

#define BUFFERSIZE 128

void startServer()
{
    int sockfd;
    int newsockfd;
    int portno;
    int test;
    int input;
    char firstLetter;
    char buffer[BUFFERSIZE];
    socklen_t clilen;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR: failed to open socket");
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    printf("Please enter a valid port number (choose a number between 2000 and 65535).\n");
    enterPort(&portno);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR: binding failed");
    }
    printf("Waiting for client...\n");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("ERROR: accept");
    }
    else printf("Client connected!\n");
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
                readMessage(newsockfd);
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
                writeMessage(newsockfd, buffer);
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
