//
//  functions.h
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//
#define BUFFERSIZE 128

void error(char* msg);
void writeMessage(int sock, char buffer[BUFFERSIZE]);
void readMessage(int sock);
void enterPort(int* portno);
