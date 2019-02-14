//
//  functions.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h> // Einlesen/Ausgeben
#include <stdlib.h> // exit()
#include <string.h> // Stringhandling
#include <unistd.h> // read()/write()
#include <sys/select.h> // select()
#include "functions.h"
#include "encryption.h"

#define BUFFERSIZE 128

 void error(char* msg)
{
    perror(msg);
    exit(1);
}

void writeMessage(int sock, char* buffer) // verschluesselt eine zuvor eingegebene Nachricht und sendet sie an den Chatpartner
{
    char key[4]; // Schluessel zur Verschluesselung
    char cryptedMsg[BUFFERSIZE]; // verschluesselte Nachricht
    strcpy(key, "abc"); // initialisiert den Schluessel
    int n;
    strcpy(cryptedMsg, crypter(buffer, key)); // verschluesselt die Nachricht
    n = (int)write(sock, cryptedMsg, strlen(cryptedMsg)); // verschickt die Nachricht
    if (n < 0)
    {
        error("ERROR: writing to socket");
    }
}

void readMessage(int sock) // checkt, ob es eine neue Nachricht gibt; falls ja wird diese entschluesselt und ausgegeben
{
    char key[4]; // Schluessel zur Verschluesselung
    char buffer[BUFFERSIZE]; // Buffer, in den die erhaltene Nachricht eingelesen wird
    char message[BUFFERSIZE]; // entschluesselte Nachricht
    fd_set readset; // File Descriptor fuer select()
    struct timeval tv; // Zeitintervall
    int retval; // Rueckgabewert von select()
    bzero(buffer, BUFFERSIZE);
    bzero(message, BUFFERSIZE); // initialisiert buffer und message mit Nullen
    FD_ZERO(&readset);
    FD_SET(sock, &readset); // initialisiert readset mit dem Wert von sock
    tv.tv_sec = 1;
    tv.tv_usec = 0; // setzt das Zeitintervall auf eine Sekunde
    strcpy(key, "abc"); // initialisiert den Schluessel
    retval = select(sock + 1, &readset, NULL, NULL, &tv); // checkt, ob es neue Nachrichten gibt; ist das nicht der Fall, wird die Funktion beendet
    if (retval == 1)
    {
        read(sock, buffer, BUFFERSIZE - 1); // liest die neue Nachricht in buffer ein
        strcpy(message, decrypter(buffer, key)); // entschluesselt die Nachricht
        printf("%s\n", message);
    }
    else
    {
        printf("No new messages.\n");
    }
}

void enterPort(int* portno) // Eingabe der Portnummer
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
