#include "Logique.h"
#include <stdbool.h>
#include <string.h>

Pion PlateauPion[4][4][10];
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
		plateau[0][0].TypeCase = 1; // Volcan
		plateau[0][1].TypeCase = 1;
		plateau[0][2].TypeCase = 3;
		plateau[0][3].TypeCase = 4;
		plateau[1][0].TypeCase = 3;
		plateau[1][1].TypeCase = 2;
		plateau[1][2].TypeCase = 2;
		plateau[1][3].TypeCase = 2;
		plateau[2][0].TypeCase = 1;
		plateau[2][1].TypeCase = 3;
		plateau[2][2].TypeCase = 0;
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
}


int lire_touche() { // cette fonction permet de lire la valeur code d'une touche de clavier genre 'z', 'q', 's', 'd' pour les déplacements et les flèches pour la sélection des dés
	if (_kbhit()) {
		int c = _getch();
		if (c == 0 || c == 224) { // Touche spéciale (Flèches)
			return _getch(); // Retourne le code spécifique (72=Haut, 80=Bas, 75=Gauche, 77=Droite)
		}
		return c; // Retourne le caractère (ex: 'z', 'q', etc.)
	}
	return -1; //pas d'action
}

void logiquede(int joueur) { // cette fonction permet de lancer les dés en fonction du nombre de titanosaures que le joueur possède
	for (int j = 0; j < 5 + possedetitanosaure(joueur); j++) {
		if (!Listede[j].bloque) {
			Listede[j].action = rand() % 6 ; // Génère une action aléatoire entre 0 et 5
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

bool selectiondes() { // cette fonction permet de demander au joueur quels dés il veut garder après le lancer de dés
	int valeurtableau = 0;
	switch(lire_touche()) {
		case '38 ': // flèche du haut
			valeurtableau--;
			break;
		case '40':// flèche du bas
			valeurtableau++;
			break;
		case '13':// Touche entrer
			return true;
			break;
		case '66':// Touche B pour bloquer un dé
			if (Listede[valeurtableau].bloque) { // inverse la valeur de bloque du dé sélectionné, si il est bloqué il devient débloqué et inversement
				Listede[valeurtableau].bloque = false;
			} else {
				Listede[valeurtableau].bloque = true;
			}
			break;
	}
	return false;

}