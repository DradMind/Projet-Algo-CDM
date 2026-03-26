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
    int dezoom_manuel = 6;
    // =========================================================================
    initialiser_console(dezoom_manuel); // Initialise le plein écran et applique le dézoom
    printf("\x1b[2J");// Efface l'écran (Clear Screen)



	//===================== Initialisation =========================
    int joueur = 0; // 0 = rouge, 1 = bleu, 2 = vert, 3 = violet (a faire plus tard)
	int nbjoueur = 2;
	bool partie = true;
	bool fin_jeu = false;
    //scanf("%d", &nbjoueur);
    initialiser_plateau(false
        , nbjoueur); // Initialise le plateau de base pour 4 joueurs
	printf("%d", nbjoueur);
    int curseur_x = 0, curseur_y = 0;
	
	//==================== Affichage =========================
    afficherplateau(10, 10); // Affiche le plateau à l'écran
    dessiner_rectangle(1, 55, 50, 10, 0xFFFFFF); // bare d'info en bas en bleu
    dessiner_rectangle(70, 10, 30, 50, 0xFFFFFF); // Zone de jeu en rouge
    afficher_texte("Zone de pions/items/etc", 1, 55, 0xFF0000, 0xFFFFFF);
    afficher_texte("Zone du plateau de jeu ", 70, 10, 0xFF0000, 0xFFFFFF);


	//======================= Code du jeu ========================================

    while (partie && !fin_jeu) {
        afficherplateau(10, 10);

        // On dessine un petit curseur visuel sur la case actuelle
        dessiner_rectangle(12 + (curseur_x), 12 + (curseur_y), 1, 1, 0xFFFFFF);

        // 2. Gestion des entrées
        int k = lire_touche();
        if (k != -1) {
            switch (k) {
            case 72: if (curseur_y > 0) curseur_y--; break; // Haut
            case 80: if (curseur_y < 3) curseur_y++; break; // Bas
            case 75: if (curseur_x > 0) curseur_x--; break; // Gauche
            case 77: if (curseur_x < 3) curseur_x++; break; // Droite
            case 'q': case 27: partie = false; break;     // Quitter
            }
        }

        Sleep(100); // Petit délai pour ne pas saturer le processeur
        //============================================================================
    }



    
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