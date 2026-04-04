#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void initialiser_plateau(bool PlateauBase, int nbJoueurs); // PlateauBase pour configuration du plateau de base bool = true/false, avec false la config aléatoire
int lire_touche();
void logiquede(int joueur);
int possedetitanosaure(int joueur);
bool selectiondes();
void LogiqueCurseur(int* curseur_x, int* curseur_y, bool* partie);
int seisme(Joueurs);
int comptage_piont_case(Joueurs, ligne, colonne);
void deplacement_pion();
void deploiment_pion();
void oeuf(int joueur);


typedef struct {
	int TypePion; // cromagnon : 1, titanosaure : 2, brontosaure : 3, stégosaure : 4, ankylosaure : 5,	ptéranodon : 6, plésiosaure : 7, tricératops : 8, tyrannosaure : 9
	char CheminImage[100];
	bool nboeuf;//position sur les dino
	int couleur;
	int joueur;

} Pion;

typedef struct {
	int TypeCase;
	char CheminImage[100];
	int nbPlacePion;
	int nbpion;
} Case;
// Chaque type de case avec le volcan : 0, l'eau : 1, la jungle : 2, la prairie : 3, la hutte : 4, la caverne : 5.	

typedef struct {
	bool bloque;
	int action;
	bool selectionne;
} Des;

typedef struct {
	int numerojoueur;
	int dino;
	int reserve;
	int points;
} Joueur;


extern Pion PlateauPion[4][4][10]; //position ligne collonne + nombre de pions
extern Case plateau[4][4];
extern Joueur joueurs[4][6];
extern Des Listede[7];
extern int InventaireCase[16];