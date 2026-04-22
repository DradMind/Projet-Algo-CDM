#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Déclarations de structures (Pion, Case, Des, Joueur) conservées à l'identique...
typedef struct {
	int TypePion;
	char CheminImage[100];
	int nboeuf;
	int couleur;
	int joueur;
	bool survolcan;
} Pion;

typedef struct {
	int TypeCase;
	char CheminImage[100];
	int nbPlacePion;
	int nbpion;
	int distance;
} Case;

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

typedef struct {
	Case plateau[4][4];
	Pion PlateauPion[4][4][10];
	Des Listede[7];
	Joueur Joueurs[4][6];
} Jeu;

// Fonctions
void initialiser_plateau(Jeu* jeu, bool PlateauBase, int nbJoueurs);
void logiquede(Jeu* jeu, int joueur);
int possedetitanosaure(Jeu* jeu, int joueur);
int seisme(Jeu* jeu, int joueur_actif); // Correction de variable
int comptage_piont_case(Jeu* jeu, int joueur, int ligne, int colonne);
void deplacement_pion(Jeu* jeu);
void deploiment_pion(Jeu* jeu, int joueur);
void oeuf(Jeu* jeu, int joueur);
int verification_case(Jeu* jeu, int joueur, int ligne, int colonne, int type_recherche, int cd_recherche);
void enlever_pion(Jeu* jeu, int ligne, int colonne, int joueur, int place);