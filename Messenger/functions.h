//
//  functions.h
//  Messenger
//

#define BUFFERSIZE 128

void error(char* msg);
void writeMessage(int sock, char buffer[BUFFERSIZE]);
void readMessage(int sock);
void enterPort(int* portno);
