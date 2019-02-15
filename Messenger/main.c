//
//  main.c
//  Messenger
//
//  Created by Max Marhofen on 13.02.19.
//  Copyright © 2019 EL2M. All rights reserved.
//

#include <stdio.h> // Einlesen/Auslesen
#include "server.h"
#include "client.h"

int main()
{
    int input; // Variable zur Auswahl, was der Benutzer tun möchte
    printf("Please choose whether you want to connect to a host or create a host yourself.\n");
    printf("(1) Connect to host\n");
    printf("(2) Create host\n");
    if (scanf("%d", &input) == 0) // Auswahl, Host zu erstellen oder Client zu verbinden
    {
        getchar();
        printf("Invalid input.\n");
        main();
    }
    switch (input)
    {
        case 1:
            startClient(); // startet Hauptmethode in client.c
            break;
        case 2:
            startServer(); // startet Hauptmethode in server.c
            break;
        default:
            printf("Invalid input.\n");
            main();
            break;
    }
    return 0;
}
