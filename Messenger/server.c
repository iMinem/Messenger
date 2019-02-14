//
//  server.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h> // Einlesen/Auslesen
#include <string.h> // Stringhandling
#include <netinet/in.h> // Internet-Domains
#include "server.h"
#include "functions.h"

#define BUFFERSIZE 128

void startServer() // Hauptprogramm des Servers
{
    int sockfd;
    int newsockfd;
    int portno;
    socklen_t clilen; // benötigte Variablen für Socket-Verbindung
    int test; // zur Terminierung der while-Schleife
    int input; // Eingabevariable zur Optionenauswahl
    char firstLetter;
    char buffer[BUFFERSIZE]; // speichert die eingegebene Nachricht
    struct sockaddr_in serv_addr; // enthält Internetadresse des Servers
    struct sockaddr_in cli_addr; // enthält Internetadresse des Clients
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // erstellt einen neuen Socket
    if (sockfd < 0)
    {
        error("ERROR: failed to open socket");
    }
    bzero((char*)&serv_addr, sizeof(serv_addr)); // initialisiert serv_addr mit Nullen
    printf("Please enter a valid port number (choose a number between 2000 and 65535).\n");
    enterPort(&portno); // Eingabe der Portnummer
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // serv_addr die jeweiligen Werte zuweisen
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) // bindet den Socket an die Adresse
    {
        error("ERROR: binding failed");
    }
    printf("Waiting for client...\n");
    listen(sockfd, 5); // wartet auf eine Anfrage von einem Client
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen); // accept() blockt den Prozess, bis ein Client verbunden wurde
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
                for (int i = (int)strlen(buffer); i >= 0; i--)
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
