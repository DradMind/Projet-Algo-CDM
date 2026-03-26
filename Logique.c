#include "Logique.h"
#include <stdbool.h>





void initialiser_plateau(bool PlateauBase, int nbJoueurs) {
	srand((unsigned int)time(NULL));
	memcpy(InventaireCase, (int[]) { 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 5 }, sizeof(InventaireCase)); //permet d'initialiser l'inventaire des cases avec les bonnes quantités de chaque type de case
	switch (nbJoueurs) { //pour le volcan avec 2,3,4 joueurs (a faire plus tard)
		case 2:
			break;
		case 3:
			break;
		case 4: 
			break;
	if (PlateauBase) {
		plateau[0][0].TypeCase = 0; // Volcan
		plateau[0][1].TypeCase = 1;
		plateau[0][2].TypeCase = 1;
		plateau[0][3].TypeCase = 1;
		plateau[1][0].TypeCase = 1;
		plateau[1][2].TypeCase = 1;
		plateau[1][3].TypeCase = 2;
		plateau[1][4].TypeCase = 2;
		plateau[2][0].TypeCase = 2;
		plateau[2][1].TypeCase = 2;
		plateau[2][2].TypeCase = 2;
		plateau[2][3].TypeCase = 3;
		plateau[3][0].TypeCase = 3;
		plateau[3][1].TypeCase = 3;
		plateau[3][2].TypeCase = 4;
		plateau[3][3].TypeCase = 5;
		
	}
	else {
		int i, j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				plateau[i][j].TypeCase = 0; // Initialiser toutes les cases à 0
			}
		}
	}
}