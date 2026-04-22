#define _CRT_SECURE_NO_WARNINGS
#include "Logique.h"
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include "Graphiques.h"

Pion PlateauPion[4][4][10];
Pion stockpion[4][10];
Case plateau[4][4];
Des Listede[7];
Pion plateaupion[4][4];
Joueur Joueurs[4][6];
int InventaireCase[16];

void initialiser_plateau(bool PlateauBase, int nbJoueurs) { // cette fonction permet d'initialiser le plateau de jeu, soit avec une configuration de base, soit de manière aléatoire
	srand((unsigned int)time(NULL));
	memcpy(InventaireCase, (int[]) { 0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 5 }, sizeof(InventaireCase)); //permet d'initialiser l'inventaire des cases avec les bonnes quantités de chaque type de case
	int temp[16]; // le tableau temporaire pour le mélange aléatoire des cases
	for (int i = 0; i < 16; i++) {
		temp[i] = InventaireCase[i]; //initialisation du tableau TEMP
	}
	switch (nbJoueurs) { //pour le volcan avec 2,3,4 joueurs (a faire plus tard)
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}
	if (PlateauBase) {
		plateau[0][0].TypeCase = 1;
		plateau[0][1].TypeCase = 1;
		plateau[0][2].TypeCase = 3;
		plateau[0][3].TypeCase = 4;
		plateau[1][0].TypeCase = 3;
		plateau[1][1].TypeCase = 2;
		plateau[1][2].TypeCase = 2;
		plateau[1][3].TypeCase = 2;
		plateau[2][0].TypeCase = 1;
		plateau[2][1].TypeCase = 3;
		plateau[2][2].TypeCase = 0;// Volcan
		plateau[2][3].TypeCase = 3;
		plateau[3][0].TypeCase = 3;
		plateau[3][1].TypeCase = 5;
		plateau[3][2].TypeCase = 2;
		plateau[3][3].TypeCase = 2;

	}
	else {
		for (int i = 15; i > 0; i--) { //mélange aléatoirement le tableeau invetnaire case et apres le fou dans le tableau temp
			int r = rand() % (i + 1);
			int valeurtemp = temp[i];
			temp[i] = temp[r];
			temp[r] = valeurtemp;
		}
		int index_pool = 0; // prend le teableau temp et le met dans le tableau 2D plateau
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				plateau[x][y].TypeCase = temp[index_pool++];
			}
		}
	}
	for (int i = 0; i < 4; i++) { // cette boucle permet d'initialiser les pions de chaque joueur dans leur réserve et aussi le plateau de jeu de pions
		for (int j = 0; j < 4; j++) {
			plateaupion[i][j].TypePion = 0;
		}
		for (int j = 0; j < 6; j++) {
			Joueurs[i][j].numerojoueur = i;
			Joueurs[i][j].dino = 0;
			Joueurs[i][j].reserve = 0;
			Joueurs[i][j].points = 0;
		}
	}
}


void logiquede(int joueur) { // cette fonction permet de lancer les dés en fonction du nombre de titanosaures que le joueur possède
	for (int j = 0; j < 5 + possedetitanosaure(joueur); j++) {
		if (!Listede[j].bloque) {
			Listede[j].action = rand() % 6; // Génère une action aléatoire entre 0 et 5
		}
	}
}

int possedetitanosaure(int joueur) { //Cette fonction permet de vérifier le nombre de titanosaure que possède le joueur
	int nbtitanosaurus = 0;
	for (int i = 0; i < 6; i++) {
		if (Joueurs[joueur][i].dino == 1) {
			nbtitanosaurus++;
		}
	}
	return nbtitanosaurus;
}

int seisme(int Joueurs) {
	int ligne = 0;
	int colonne = 0;
	int seisme = 0;
	for (int i = 0; i < 4; i++) {						// cette boucle permet de trouver la position du volcan sur le plateau
		for (int j = 0; i < 4; j++) {
			if (plateau[i][j].TypeCase == 0) {
				ligne = i;
				colonne = j;
			}
		}
	}
	int pions_j = comptage_piont_case(Joueurs, ligne, colonne); // cette fonction permet de compter le nombre de pion que le joueur a sur la case du volcan

	int moitie = (plateau[ligne][colonne].nbpion) / 2;
	if (moitie <= pions_j) {
		seisme = 1;
	}

	// rajouter une vérification pour le stégosaure qui peut activer le séisme même si il n'a pas la moitié des pions sur le volcan

	if (seisme = 1) {
		int choix_direction = 0;
		//selection de la direction
		printf("choisissez la ligne (1) ou la colonne (2)  ");
		// selection du sens
		int choix_sens = 0;
		if (choix_direction == 2) {
			printf("choisissez le sens : haut (1) ou bas (2)  ");
		}
		else {
			printf("choisissez le sens : gauche (1) ou droite (2)  ");
		}


		//deplacement des cases et des pions
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (choix_direction == 1) { // ligne
					if (choix_sens == 1) { // haut
						// déplacer la ligne vers le haut
						// déplacer les pions en conséquence
					}
					else { // bas
						// déplacer la ligne vers le bas
						// déplacer les pions en conséquence
					}
				}
				else { // colonne
					if (choix_sens == 1) { // gauche
						// déplacer la colonne vers la gauche
						// déplacer les pions en conséquence
					}
					else { // droite
						// déplacer la colonne vers la droite
						// déplacer les pions en conséquence
					}
				}
			}
		}
	}
}


int comptage_piont_case(int Joueurs, int ligne, int colonne) {
	int pions_j = 0;
	for (int i = 0; i < 10; i++) {
		if (PlateauPion[ligne][colonne][i].joueur == Joueurs) {
			pions_j++;
		}
	}
	return pions_j;
}

void deplacement_pion() {
	int ligne, colonne;	//selection case d'origine avec le deplacement par les touches directionnelles


	//selection pion a deplacer (si il y a un dinosaure du joueur)
	//surbrillance des cases de destination possible
	//cheque si il y a de l'eau à proximité
		//cheque si le joueur a un autre déplacement possible
			//si oui, surbriance spéciale pour l'eau
			// si selection du déplacement, rendre obligatoire le déplacement de ce même pion
	// selection de la case de destination
	// deplacement du pion
}

void deploiment_pion(int joueur) {
	if (Joueurs[joueur][0].reserve > 0) { // vérification de la présence d'un pion dans la réserve du joueur
		Joueurs[joueur][0].reserve--; // si oui, on enlève un pion de la réserve
		for (int l = 0; l < 4; l++) {
			for (int c = 0; c < 4; c++) {
				if (plateau[l][c].TypeCase == 0) { // vérification de la position du déploiement (si c'est une hutte ou une caverne)
				}
			}
		}

	}
	else {
		int choix;
		printf("votre réserve est vide vous allez pouvoir choisir si vous re-déployé un cromagnon du plateau (1) ou de ne pas faire l'action (0)");
		if (1 == 1) {
			int ligne, colonne;
			do {
				//selection_case(&ligne, &colonne);
			} while (0 == 0); // Vérifie que le joueur a bien un cromagnon sur la case sélectionnée
		}
	}
	//vérification de la position du déploiement (si c'est une hutte ou une caverne)
	//vérification de la présence d'un pions dans la réserve du joueur
		// si oui, verification de la présence d'un dino du joueur dans la réserve du joueur
			// si oui, selection du pion à déployer (hutte ou dino)
			// si non, résolution automatique
		// si non, informer le joueur que sa réserve est vide et lui demander si il veux continuer
			// si oui, verifier si il a un pion sur le plateau
				// si non, fin de l'action du joueur
				// si oui, selection case
				// selection pion
				//résolution
}

void oeuf(int joueur) {
	if (Joueurs[joueur][0].reserve > 0) {
		Joueurs[joueur][0].reserve--; Joueurs[joueur][0].dino++;
	}
	else {
		int choix;
		printf("votre réserve est vide vous allez pouvoir choisir si vous prenez un cromagnon du plateau (1) ou de ne pas faire l'action (0)");
		if (1 == 1) {
			int ligne, colonne;
			do {
				//selection_case(&ligne, &colonne);
			} while (0 == 0); // Vérifie que le joueur a bien un cromagnon sur la case sélectionnée

		}
	}
}

int verification_case(int	joueur, int ligne, int colonne, int type_recherche, int cd_recherche) {
	int place = 0;
	int presence = 0;
	if (cd_recherche == 0) {
		while (PlateauPion[ligne][colonne][place].TypePion != 0) {  // Tant qu'il y a des pions sur la case
			if (PlateauPion[ligne][colonne][place].joueur == joueur && PlateauPion[ligne][colonne][place].TypePion == type_recherche) {// Vérifie si le pion appartient au joueur et correspond au type recherché (dino ou cromagnon)
				presence++;

			}
			place++;

		}
	}
	else if (cd_recherche >= 1) {
		if (plateau[ligne][colonne].distance >= cd_recherche) {
			presence++;
		}
		//vérification de la présence recherché (dino ou cromagnon) sur la case sélectionné
		//si type Cromagnon, selection uniquement de cromagnon
		//si type dino, laisser le choix
		return presence;// pour préparer la taille du tableau de sélection des pions
	}
}

void enlever_pion(int ligne, int colonne, int joueur, int place) {
	PlateauPion[ligne][colonne][place].TypePion = 0;
}


