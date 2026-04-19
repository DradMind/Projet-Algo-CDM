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
int seisme(int Joueurs);
int comptage_piont_case(int Joueurs,int ligne,int colonne);
void deplacement_pion(void);
void deploiment_pion(void);
void action_des(int joueur);
void oeuf(int joueur);
int vérification_pion(int joueur, int ligne, int colonne, int type_recherche);
void selection_case(int *ligneR, int *colonneR);



typedef struct {
	int TypePion; // case vide : 0, cromagnon : 1, titanosaure : 2, brontosaure : 3, stégosaure : 4, ankylosaure : 5,	ptéranodon : 6, plésiosaure : 7, tricératops : 8, tyrannosaure : 9
	char CheminImage[100];
	int nboeuf;//position sur les dino : 0 rien, 1 couveuse, 2 dino
	int couleur;
	int joueur;
	bool survolcan;
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
	int action; // 0 : rien, 1 : déplacement, 2 : oeuf, 3 : volcan, 4 : hutte, 5 : cave, 6 : déplacement
	bool selectionne;
} Des;

typedef struct {
	int numerojoueur;
	int dino;
	int reserve;
	int points;
} Joueur;


extern Pion PlateauPion[4][4][10]; //position ligne collonne + nombre de pions
extern Pion stockpion[4][10];
extern Case plateau[4][4];
extern Joueur joueurs[4][6];
extern Des Listede[7];
extern int InventaireCase[16];
