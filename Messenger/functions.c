//
//  functions.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include "functions.h"
#include "encryption.h"

#define BUFFERSIZE 128

 void error(char* msg)
{
    perror(msg);
    exit(1);
}

void writeMessage(int sock, char* buffer)
{
    char key[4];
    char cryptedMsg[BUFFERSIZE];
    strcpy(key, "abc");
    int n;
    strcpy(cryptedMsg, crypter(buffer, key));
    n = (int)write(sock, cryptedMsg, strlen(cryptedMsg));
    if (n < 0)
    {
        error("ERROR: writing to socket");
    }
}

void readMessage(int sock)
{
    //int n;
    char buffer[BUFFERSIZE];
    char message[BUFFERSIZE];
    char key[4];
    bzero(buffer, BUFFERSIZE);
    bzero(message, BUFFERSIZE);
    
    fd_set readset;
    struct timeval tv;
    int retval;
    
    FD_ZERO(&readset);
    FD_SET(sock, &readset);
    
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    
    strcpy(key, "abc");
    retval = select(sock + 1, &readset, NULL, NULL, &tv);
    if (retval == 1)
    {
        read(sock, buffer, BUFFERSIZE - 1);
        strcpy(message, decrypter(buffer, key));
        printf("%s\n", message);
    }
    else
    {
        printf("No new messages.\n");
    }
    /*n = (int)read(sock, buffer, BUFFERSIZE - 1);
    if (n < 0)
    {
        error("ERROR: reading from socket");
    }
    if (strcmp(buffer, "reading mode\n") == 0)
    {
        printf("No new messages.\n");
    }
    else
    {
        strcpy(message, decrypter(buffer, key));
        printf("%s\n", message);
    }*/
}

void enterPort(int* portno)
{
    if (scanf("%d", portno) == 0 || *portno < 2000 || *portno > 65535)
    {
        getchar();
        printf("Invalid input. Please try again.\n");
        enterPort(portno);
    }
    else
    {
        printf("Port %d was chosen.\n", *portno);
    }
}
