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
    int dezoom_manuel = 8;

    // =========================================================================
    initialiser_console(dezoom_manuel); // Initialise le plein écran et applique le dézoom
    printf("\x1b[2J");// Efface l'écran (Clear Screen)



	//===================== Initialisation =========================
    int joueur = 0; // 0 = rouge, 1 = bleu, 2 = vert, 3 = violet (a faire plus tard)
	int nbjoueur = 2;
	bool partie = true;
	bool fin_jeu = false;
	int etapejeu = 0;
    initialiser_plateau(false, nbjoueur); // Initialise le plateau de base pour 4 joueurs
    int curseur_x = 0;
    int curseur_y = 0;

    
	
	//==================== Affichage =========================
    afficherplateau(1, 1); // Affiche le plateau à l'écran
    dessiner_rectangle(1, 45, 50, 10, 0xFFFFFF); // bare d'info en bas en bleu
    dessiner_rectangle(60, 10, 30, 50, 0xFFFFFF); // Zone de jeu en rouge
    afficher_texte("Zone de pions/items/etc", 1, 45, 0xFF0000, 0xFFFFFF);
    afficher_texte("Zone du plateau de jeu ", 60, 10, 0xFF0000, 0xFFFFFF);
	//dessiner_image_ppm("Images/img.ppm", 0, 0);

	//======================= Code du jeu ========================================
	while (partie && !fin_jeu) {
		//if (etapejeu == 0) {
		afficherclavier(&curseur_x, &curseur_y, &partie);
        afficher_texte("Bienvenue sur Colère de la Montagne de Feu !\n Veuillez choisir le nombre de joueur", 10, 10, 0xFF0040, 0x000000);
		

        
        
        
        
        
    }
    //============================================================================


    
    bouger_curseur(0, 200);// Déplace le curseur tout en bas pour ne pas casser l'affichage
	
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