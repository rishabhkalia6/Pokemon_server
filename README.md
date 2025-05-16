# 🕹️ Pokémon Server (C, Sockets, Linux)

A client-server application written in C that uses TCP sockets to let clients search for Pokémon data by type. The server processes CSV queries and sends back results in real time.

---

## 📦 Features

- Built with **modular design** and shared header files  
- **Real-time communication** between client and server  
- **CSV parsing** and result filtering by Pokémon type  
- **Makefile** for easy building and running  

---

## 🗄️ Project file structure

The File structure for this project consists of the followig files that make this program work I will be explaing each file and you can find these files in the main repository.

- Client.c
- common.c
- Pokemon.h
- server.c
- Makefile
- Pokemon.csv

---

## 💻 Client.c

This file manages **Client** program that sends search **queries** to the **server** and handles results. I wont get into the logic side of it such as searching the csv file as that is very basic what I want to showcase however is the functionality of how the the **client side code** interacts with the **server side code**.

``` C
#define SERVER_IP "127.0.0.1"
#define PORT 57461

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
```

This snippet configures the client’s connection to the server using the **`sockaddr_in`** structure. It sets the IP address **`(127.0.0.1)`** and port **`(57461)`**, then attempts to connect using the **`connect()`** system call. If the connection fails, it safely exits with an error message.

---

## 🔎Common.c

This file Implements the function to load Pokémon data from the **CSV file.** It is has standard **input/output** library.

``` C
#include <stdio.h>
#include "pokemon.h"

// Function to load Pokémon data from a CSV file
int loadPokemonData(const char *filename, Pokemon pokemons[], int max) {
    // Open the CSV file in read mode
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    // Buffer to store each line from the file
    char line[256];
    int count = 0;
    fgets(line, sizeof(line), fp); 

    while (fgets(line, sizeof(line), fp) && count < max) {
        sscanf(line, "%d,%49[^,],%29[^,],%29[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d",
               &pokemons[count].id,
               pokemons[count].name,
               pokemons[count].type1,
               pokemons[count].type2,
               &pokemons[count].total,
               &pokemons[count].hp,
               &pokemons[count].attack,
               &pokemons[count].defense,
               &pokemons[count].sp_atk,
               &pokemons[count].sp_def,
               &pokemons[count].speed,
               &pokemons[count].generation,
               &pokemons[count].legendary);
        count++;
    }

    fclose(fp);
    return count;
}
```
This function reads **Pokémon** data from a **CSV file** and stores it into an array of **``Pokemon``** structs.
It skips the header line, then uses **``sscanf()``** to safely parse each line into fields like ID, name, types, and stats. It prevents buffer overflows by limiting string input sizes and stops reading when the maximum limit is reached or the file ends.

---

## 📡 Server.c

This file manages **Server program** that loads data, listens for client requests, and sends search results.

``` C
// Set up the TCP server
int serverFD = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in serverAddr = {0};
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(PORT);
serverAddr.sin_addr.s_addr = INADDR_ANY;

bind(serverFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
listen(serverFD, 5);

// Accept and handle client connections
while (1) {
    int clientFD = accept(serverFD, NULL, NULL);
    if (clientFD == -1) continue;

    // Read type from client and respond with matching Pokémon
    read(clientFD, buffer, sizeof(buffer));
    searchByType(buffer, results);  // Your CSV search function
    write(clientFD, results, strlen(results));
    close(clientFD);
}
```

This snippet sets up a TCP socket, binds it to a local port, and listens for incoming client connections. Once a client connects, the server reads a Pokémon type query, searches the CSV file for matches, sends the results back, and closes the connection. The loop runs indefinitely to handle multiple client requests.

## 🛠️ Finally the Makefile

```bash
# Default target to build both server and client
all : server client

# Compile server source file to object file
server.o: server.c pokemon.h
	gcc -c server.c

# Compile client source file to object file
client.o: client.c pokemon.h
	gcc -c client.c

# Compile shared common functions to object file
common.o common.c pokemon.h
	gcc -c common.c

# Link server object files into server executable
server: server.o common.o
	gcc -o server server.o common.o

# Link client object files into client executable
client: client.o common.o
	gcc -o client client.o common.o

# Clean up all compiled object files and executables
clean:
	rm -f * .o server client
```
This **``Makefile``** builds the project using separate object files for modularity. It compiles each source file into **``.o``** files, then links them into the **``server``** and **``client``** executables. The **``clean``** target removes all compiled files for a fresh rebuild.

---

## 💻 Output

