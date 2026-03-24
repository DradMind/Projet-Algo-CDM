#pragma once
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int TypePion;
	int couleur;


} Pion;


typedef struct {
	int TypeCase;
	
} Case;

extern Case plateau[4][4];
extern Pion pions[4][4][10];