
// BY Rishabh Kalia

// This file  Defines the Pokémon data structure and declares shared functions.
// pokemon.h

// Header guard to prevent multiple inclusions
#ifndef POKEMON_H
#define POKEMON_H

// Define maximum number of Pokémons
#define MAX_POKEMONS 1000

// Define maximum length for Pokémon type
#define MAX_NAME_LEN 50
#define MAX_TYPE_LEN 30

// Structure to represent a Pokémon with its properties
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char type1[MAX_TYPE_LEN];
    char type2[MAX_TYPE_LEN];
    int total;
    int hp, attack, defense;
    int sp_atk, sp_def, speed;
    int generation;
    int legendary;
} Pokemon;

// Shared function declaration
int loadPokemonData(const char *filename, Pokemon pokemons[], int max);


#endif
