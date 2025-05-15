
// BY Rishabh Kalia

// client.c
// This file manages Client program that sends search queries to the server and handles results.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "pokemon.h"

#define SERVER_IP "127.0.0.1"
#define PORT 57461

// Array to store search results
Pokemon results[MAX_POKEMONS];

// Variable to track total search results
int totalResults = 0;

// Function to perform Pokémon type search
void searchPokemon() {
    // Create a socket for communication
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address structure
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Connect to the server
    if (connect(socketFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        printf("Unable to establish connection to the PPS!\n");
        exit(1);
    }

    // Get search type input from user
    char searchType[MAX_TYPE_LEN];
    printf("Enter Type 1 to search: ");
    scanf("%s", searchType);

    // Send search type to server
    write(socketFD, searchType, sizeof(searchType));

    // Receive Pokémon data from server
    Pokemon p;
    totalResults = 0;
    while (read(socketFD, &p, sizeof(Pokemon)) > 0) {
        if (strlen(p.name) == 0) break;
        results[totalResults++] = p;
    }

    // Display search completion message
    printf("Search completed. Found %d Pokemon(s).\n", totalResults);

    // Close the socket
    close(socketFD);
}

// Function to save search results to a file
void saveResults() {
    // Get output file name from user
    char saveFile[100];
    printf("Enter name of file to save results: ");
    scanf("%s", saveFile);

    // Open file for writing
    FILE *out = fopen(saveFile, "w");
    if (!out) {
        printf("Unable to create the new file. Please try again.\n");
        return;
    }

    // Write results to file
    for (int i = 0; i < totalResults; i++) {
        fprintf(out, "%s,%s,%s\n", results[i].name, results[i].type1, results[i].type2);
    }

    // Close the file
    fclose(out);

    // Display success message
    printf("Results saved successfully.\n");
}

// Main function to display menu and handle user choices
int main() {
    int choice;

    while (1) {
        printf("\nMenu:\n1. Type Search\n2. Save Results\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            searchPokemon();
        } else if (choice == 2) {
            saveResults();
        } else if (choice == 3) {
            printf("Exiting program...\n");
            break;
        } else {
            printf("Invalid option. Please choose 1, 2 or 3.\n");
        }
    }

    return 0;
}
