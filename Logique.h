#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void initialiser_plateau(bool PlateauBase, int nbJoueurs); // PlateauBase pour configuration du plateau de base bool = true/false, avec false la config aléatoire


typedef struct {
	int TypePion;
	char CheminImage[100];
	int couleur;

} Pion;

typedef struct {
	int TypeCase;
	char CheminImage[100];
	int nbPlacePion;
} Case;
// Chaque type de case avec le volcan : 0, l'eau : 1, la jungle : 2, la prairie : 3, la hutte : 4, la caverne : 5.	

typedef struct {
	bool bloque;
	int action;
} De;

extern Pion pions[4][4][10];
extern Case plateau[4][4];
extern int InventaireCase[16];