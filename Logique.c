#define _CRT_SECURE_NO_WARNINGS
#include "Logique.h"
#include <stdbool.h>
#include <string.h>
#include "Graphiques.h"

void initialiser_plateau(Jeu* jeu, bool PlateauBase, int nbJoueurs) {
	srand((unsigned int)time(NULL));

	// InventaireCase devient une variable locale !
	int InventaireCase[16] = { 0, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 5 };

	if (PlateauBase) {
		jeu->plateau[0][0].TypeCase = 1; jeu->plateau[0][1].TypeCase = 1; jeu->plateau[0][2].TypeCase = 3; jeu->plateau[0][3].TypeCase = 4;
		jeu->plateau[1][0].TypeCase = 3; jeu->plateau[1][1].TypeCase = 2; jeu->plateau[1][2].TypeCase = 2; jeu->plateau[1][3].TypeCase = 2;
		jeu->plateau[2][0].TypeCase = 1; jeu->plateau[2][1].TypeCase = 3; jeu->plateau[2][2].TypeCase = 0; jeu->plateau[2][3].TypeCase = 3;
		jeu->plateau[3][0].TypeCase = 3; jeu->plateau[3][1].TypeCase = 5; jeu->plateau[3][2].TypeCase = 2; jeu->plateau[3][3].TypeCase = 2;
	}
	else {
		for (int i = 15; i > 0; i--) {
			int r = rand() % (i + 1);
			int valeurtemp = InventaireCase[i];
			InventaireCase[i] = InventaireCase[r];
			InventaireCase[r] = valeurtemp;
		}
		int index_pool = 0;
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				jeu->plateau[x][y].TypeCase = InventaireCase[index_pool++];
			}
		}
	}

	// Initialisation propre
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			jeu->Joueurs[i][j].numerojoueur = i;
			jeu->Joueurs[i][j].dino = 0;
			jeu->Joueurs[i][j].reserve = 0;
			jeu->Joueurs[i][j].points = 0;
		}
	}

	// Initialiser les pions à 0 pour éviter des bugs
	for (int l = 0; l < 4; l++) {
		for (int c = 0; c < 4; c++) {
			for (int p = 0; p < 10; p++) {
				jeu->PlateauPion[l][c][p].TypePion = 0;
			}
		}
	}
}

void logiquede(Jeu* jeu, int joueur) {
	for (int j = 0; j < 5 + possedetitanosaure(jeu, joueur); j++) {
		if (!jeu->Listede[j].bloque) {
			jeu->Listede[j].action = rand() % 6;
		}
	}
}

int possedetitanosaure(Jeu* jeu, int joueur) {
	int nbtitanosaurus = 0;
	for (int i = 0; i < 6; i++) {
		if (jeu->Joueurs[joueur][i].dino == 1) {
			nbtitanosaurus++;
		}
	}
	return nbtitanosaurus;
}

int seisme(Jeu* jeu, int Joueurs) {
	int ligne = 0;
	int colonne = 0;
	int seisme = 0;
	for (int i = 0; i < 4; i++) {						// cette boucle permet de trouver la position du volcan sur le plateau
		for (int j = 0; j < 4; j++) {
			if (jeu->plateau[i][j].TypeCase == 0) {
				ligne = i;
				colonne = j;
			}
		}
	}
	int pions_j = comptage_piont_case(jeu, Joueurs, ligne, colonne); // cette fonction permet de compter le nombre de pion que le joueur a sur la case du volcan

	int moitie = (jeu->plateau[ligne][colonne].nbpion) / 2;
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


int comptage_piont_case(Jeu* jeu, int Joueurs, int ligne, int colonne) {
	int pions_j = 0;
	for (int i = 0; i < 10; i++) {
		if (jeu->PlateauPion[ligne][colonne][i].joueur == Joueurs) {
			pions_j++;
		}
	}
	return pions_j;
}

void deplacement_pion(Jeu* jeu) {
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

void deploiment_pion(Jeu* jeu, int joueur) {
	if (jeu->Joueurs[joueur][0].reserve > 0) { // vérification de la présence d'un pion dans la réserve du joueur
		jeu->Joueurs[joueur][0].reserve--; // si oui, on enlève un pion de la réserve
		for (int l = 0; l < 4; l++) {
			for (int c = 0; c < 4; c++) {
				if (jeu->plateau[l][c].TypeCase == 0) { // vérification de la position du déploiement (si c'est une hutte ou une caverne)
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

void oeuf(Jeu* jeu, int joueur) {
	if (jeu->Joueurs[joueur][0].reserve > 0) {
		jeu->Joueurs[joueur][0].reserve--; jeu->Joueurs[joueur][0].dino++;
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

int verification_case(Jeu* jeu, int joueur, int ligne, int colonne, int type_recherche, int cd_recherche) {
	int place = 0;
	int presence = 0;
	if (cd_recherche == 0) {
		while (jeu->PlateauPion[ligne][colonne][place].TypePion != 0) {  // Tant qu'il y a des pions sur la case
			if (jeu->PlateauPion[ligne][colonne][place].joueur == joueur && jeu->PlateauPion[ligne][colonne][place].TypePion == type_recherche) {// Vérifie si le pion appartient au joueur et correspond au type recherché (dino ou cromagnon)
				presence++;

			}
			place++;

		}
	}
	else if (cd_recherche >= 1) {
		if (jeu->plateau[ligne][colonne].distance >= cd_recherche) {
			presence++;
		}
		//vérification de la présence recherché (dino ou cromagnon) sur la case sélectionné
		//si type Cromagnon, selection uniquement de cromagnon
		//si type dino, laisser le choix
		return presence;// pour préparer la taille du tableau de sélection des pions
	}
}

void enlever_pion(Jeu* jeu, int ligne, int colonne, int joueur, int place) {
	jeu->PlateauPion[ligne][colonne][place].TypePion = 0;
}


