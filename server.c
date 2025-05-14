
// MINI ASSIGNMENT POKEMON SEREVR PROJECT 
// BY Rishabh Kalia, Hikmatullah Hussain Zada, Casey Kiiza 0778570


// server.c
// The files manages Server program that loads data, listens for client requests, and sends search results.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "pokemon.h"

#define PORT 57461

// Array to store loaded Pokémon data
Pokemon pokemons[MAX_POKEMONS];

// Variable to track total Pokémon loaded
int totalPokemon = 0;

// Function to handle client requests
void handleClient(int clientSocket) {
    // Buffer to store search type received from client
    char searchType[MAX_TYPE_LEN];

    // Read search type from client
    int bytesRead = read(clientSocket, searchType, sizeof(searchType));
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }

    // Display received search request
    printf("Received search request for type: %s\n", searchType);

    // Search and send matching Pokémon records to client
    for (int i = 0; i < totalPokemon; i++) {
        if (strcmp(pokemons[i].type1, searchType) == 0) {
            write(clientSocket, &pokemons[i], sizeof(Pokemon));
        }
    }

    // Send end signal to indicate completion
    Pokemon endSignal;
    memset(&endSignal, 0, sizeof(Pokemon));
    write(clientSocket, &endSignal, sizeof(Pokemon));

    // Close client connection
    close(clientSocket);
}

// Main function to set up server and handle incoming connections
int main() {
    // Buffer to store Pokémon CSV file name
    char filename[100];
    printf("Enter the name of the Pokemon CSV file: ");
    scanf("%s", filename);

    // Load Pokémon data from CSV file
    totalPokemon = loadPokemonData(filename, pokemons, MAX_POKEMONS);
    if (totalPokemon == -1) {
        printf("Pokemon file is not found.\n");
        return 1;
    }

    // Create server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address structure
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to specified port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for incoming client connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(1);
    }

    // Display server status
    printf("Server is listening on port %d...\n", PORT);

    // Infinite loop to accept and handle client connections
    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1) {
            perror("Accept failed");
            continue;
        }

        handleClient(clientSocket);
    }

    // Close server socket
    close(serverSocket);
    return 0;
}
