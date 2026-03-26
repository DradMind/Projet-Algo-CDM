#include "Logique.h"
#include <stdbool.h>

void initialiser_plateau(bool PlateauBase, int nbJoueurs) {
	srand((unsigned int)time(NULL));
	int temp[16]; // le tableau temporaire pour le mélange aléatoire des cases
	memcpy(InventaireCase, (int[]) { 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 5 }, sizeof(InventaireCase)); //permet d'initialiser l'inventaire des cases avec les bonnes quantités de chaque type de case
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