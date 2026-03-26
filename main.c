#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Graphiques.h"
#include "Logique.h"
#include <stdbool.h>

int main() {
    // =========================================================================
    // PARAMÈTRE D'AFFICHAGE :
    // Si l'affichage déborde de l'écran ou semble mal cadré, modifie le zoom 
    int dezoom_manuel = 5;
    // =========================================================================
    initialiser_console(dezoom_manuel); // Initialise le plein écran et applique le dézoom
    printf("\x1b[2J");// Efface l'écran (Clear Screen)







	//======================= Code du jeu ========================================

	initialiser_plateau(true, 4); // Initialise le plateau de base pour 4 joueurs
	afficherplateau(10,10); // Affiche le plateau à l'écran
    


	//============================================================================
    





    
    bouger_curseur(0, 45);// Déplace le curseur tout en bas pour ne pas casser l'affichage
	
    printf("\nRendu termine! Appuyez sur Entree pour quitter...");
    getchar(); // jsp ( a voir ce que sa fait plus tard)

    return 0;
}













/* FONCTIONS DE DEBUG UTILES  :
* 
* ========== Affiche les valeurs du plateau dans la console ====================
* for (int i = 0; i < 4; i++) {
		
        for (int j = 0; j < 4; j++) {
            printf("[%d]", plateau[i][j].TypeCase);
            if (j == 3) {
                printf("\n");
            }
            
		}
    }
* =============================================================================
* 
* 
* 
* 
* 
* 
*/