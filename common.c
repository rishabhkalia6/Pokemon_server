
// MINI ASSIGNMENT POKEMON SEREVR PROJECT 
// BY Rishabh Kalia, Hikmatullah Hussain Zada, Casey Kiiza 0778570

// common.c
// This file Implements the function to load Pokémon data from the CSV file.

// standard input/output library

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
