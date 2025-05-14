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
