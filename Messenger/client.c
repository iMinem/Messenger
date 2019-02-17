//
//  client.c
//  Messenger
//

#include <stdio.h> // Einlesen/Ausgeben
#include <stdlib.h> // exit()
#include <string.h> // Stringhandling
#include <netdb.h> // Netzwerkoperationen
#include "client.h"
#include "functions.h"

#define IPLENGTH 16

void startClient() // Hauptprogramm des Clients
{
    int sockfd;
    int portno; // benoetigte Variablen für Socket-Verbindung
    int test; // zur Terminierung der while-Schleife
    int input; // Eingabevariable zur Optionenauswahl
    char firstLetter;
    char buffer[BUFFERSIZE]; // speichert die eingegebene Nachricht
    char address[IPLENGTH]; // zum Einlesen der IP-Adresse
    struct sockaddr_in serv_addr; // enthaelt Internetadresse des Servers
    struct hostent *server; // enthaelt Informationen ueber den Server
    printf("Please enter a server's IP-address.\n");
    getchar();
    bzero(address, IPLENGTH); // initialisiert address mit Nullen
    fgets(address, IPLENGTH - 1, stdin); // Eingabe der IP-Adresse des Servers
    printf("Please enter a valid port number (choose a number between 2000 and 65535.\n");
    enterPort(&portno); // Eingabe der Portnummer
    printf("Connecting...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // erstellt einen neuen Socket
    if (sockfd < 0)
    {
        error("ERROR: failed to open socket");
    }
    server = gethostbyname(address); // weist server die passenden Informationen zu
    if (server == NULL)
    {
        fprintf(stderr, "ERROR: no such host found");
        exit(0);
    }
    bzero((char*) &serv_addr, sizeof(serv_addr)); // initialisiert serv_addr mit Nullen
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno); // serv_addr die jeweiligen Werte zuweisen
    if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0) // mit Server verbinden
    {
        error("ERROR: connecting failed");
    }
    else printf("Connected to host!\n");
    test = 1;
    // In der Schleife wird der Nutzer immer wieder aufgefordert, auszuwählen, ob er neue Nachrichten empfangen, oder eine neue Nachricht verschicken will.
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
                fgets(buffer, BUFFERSIZE - 1, stdin); // Einlesen der eingegebenen Nachricht
                for (int i = (int)strlen(buffer); i >= 0; i--)
                {
                    buffer[i + 1] = buffer[i];
                }
                buffer[0] = firstLetter;
                writeMessage(sockfd, buffer);
                break;
            case 3:
                test = 0; // beendet das Programm, indem es die Schleifenbedingung nicht mehr erfüllt
                break;
            default:
                printf("Invalid input.\n");
                break;
        }
    }
}
