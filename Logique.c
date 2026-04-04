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
	static int valeurtableau = 0;
	int touche = lire_touche();
	switch(touche) {
		case 72: // flèche du haut
			valeurtableau--;
			if (valeurtableau < 0) valeurtableau = 0;
			break;
		case 80: // flèche du bas
			valeurtableau++;
			if (valeurtableau > 6) valeurtableau = 6;
			break;
		case 13: // Touche entrer
			return true;
		case 'b':
			if (Listede[valeurtableau].bloque) { // inverse la valeur de bloque du dé sélectionné, si il est bloqué il devient débloqué et inversement
				Listede[valeurtableau].bloque = false;
			} else {
				Listede[valeurtableau].bloque = true;
			}
			break;
	}

	for (int i = 0; i < 7; i++) {
		Listede[i].selectionne = (i == valeurtableau);
	}

	return false;

}

void LogiqueCurseur(int* curseur_x, int* curseur_y, bool* partie) {
	int k = lire_touche();

	// On met a jour le curseur et le plateau seulement si une touche a été pressé 
	if (k != -1) {
		switch (k) {
		case 72: (*curseur_y)--; break; // Haut
		case 80: (*curseur_y)++; break; // Bas
		case 75: (*curseur_x)--; break; // Gauche
		case 77: (*curseur_x)++; break; // Droite
		case 27: *partie = false; return; // Echap pour quitter
		}
	}
}

int seisme(Joueur) {
	int ligne = 0;
	int colonne = 0;
	int séisme = 0;
	for (int i = 0;i < 4;i++) {						// cette boucle permet de trouver la position du volcan sur le plateau
		for (int j = 0;i < 4;j++) {
			if (plateau[i][j].TypeCase == 0) {
				ligne = i;
				colonne = j;
			}
		}
	}
	int pions_j = comptage_piont_case(Joueur, ligne, colonne); // cette fonction permet de compter le nombre de pion que le joueur a sur la case du volcan
	
	int moitie = (plateau[ligne][colonne].nbpion) / 2; 
	if ( moitie <= pions_j ){ séisme= 1; } 

	// rajouter une vérification pour le stégosaure qui peut activer le séisme même si il n'a pas la moitié des pions sur le volcan

	if (séisme=1) {
		int choix_direction;
		//selection de la direction
		printf("choisissez la ligne (1) ou la colonne (2)  ");
		scanf("%d", &choix_direction);
		// selection du sens
		int choix_sens;
		if (choix_direction == 2) {
			printf("choisissez le sens : haut (1) ou bas (2)  ");
			scanf("%d", &choix_sens);
		}
		else {
			printf("choisissez le sens : gauche (1) ou droite (2)  ");
			scanf("%d", &choix_sens);
		}
		

		//deplacement des cases et des pions
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (choix_direction == 1) { // ligne
					if (choix_sens == 1) { // haut
						// déplacer la ligne vers le haut
						// déplacer les pions en conséquence
					} else { // bas
						// déplacer la ligne vers le bas
						// déplacer les pions en conséquence
					}
				} else { // colonne
					if (choix_sens == 1) { // gauche
						// déplacer la colonne vers la gauche
						// déplacer les pions en conséquence
					} else { // droite
						// déplacer la colonne vers la droite
						// déplacer les pions en conséquence
					}
				}
			}
		
		

	}
	
}

int comptage_piont_case(Joueurs,ligne,colonne){
	int pions_j = 0;
	for (int i = 0; i < 10; i++) {
		if (PlateauPion[ligne][colonne][i].joueur == 0) { break; }
		if (PlateauPion[ligne][colonne][i].joueur == Joueur) {
			pions_j++;
		}
	}
	return pions_j;
}

void deplacement_pion() {
	//selection case d'origine avec le deplacement par les touches directionnelles
	//selection pion a deplacer (si il y a un dinosaure du joueur)
	//surbrillance des cases de destination possible
	//cheque si il y a de l'eau à proximité
		//cheque si le joueur a un autre déplacement possible
			//si oui, surbriance spéciale pour l'eau 
			// si selection du déplacement, rendre obligatoire le déplacement de ce même pion
	// selection de la case de destination
	// deplacement du pion
}

void deploiment_pion(type case) {
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

void oeuf(joueur) {
//verification de la présence d'un cromagnon dans la réserve du joueur
	// si oui, couveuse (nom que je donne à là où sont envoyé les cromagnons avant de choisir le dino)
	//si non, informer le joueur que sa réserve est vide et lui demander si il veux continuer
		// si oui, verifier si il a un cromagnon sur le plateau
			// si non, fin de l'action du joueur
			// si oui, selection case
			// selection pion
			//résolution
}

