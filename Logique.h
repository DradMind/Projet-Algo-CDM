#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void initialiser_plateau(bool PlateauBase, int nbJoueurs); // PlateauBase pour configuration du plateau de base bool = true/false, avec false la config aléatoire
int lire_touche();
void logiquede(int joueur);

typedef struct {
	int TypePion; // cromagnon : 0, titanosaure : 1, brontosaure : 2, stégosaure : 3, ankylosaure : 4,	ptéranodon : 5, plésiosaure : 6, tricératops : 7, tyrannosaure 8
	char CheminImage[100];
	int nboeuf;
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

typedef struct {
	int numerojoueur;
	int dino;
} Joueur;


extern Pion pions[4][4][10];
extern Pion plateaupion[4][4]; 
extern Case plateau[4][4];
extern Joueur joueurs[4][6];
extern De Listede[7];
extern int InventaireCase[16];