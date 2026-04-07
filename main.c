#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Graphiques.h"
#include "Logique.h"
#include <stdbool.h>
#include <time.h>

int main() {
    srand((unsigned int)time(NULL));

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
	int etapejeu = 0; // 0 = lancer de de, 1 = Bouger les figurines, 2 = Vérifier les séismes, 3 = déplacement du pion, Vérifier les éruptions (décompte des points)
    int curseur_x = 0;
    int curseur_y = 0;
	int lancerde = 0;
	int nblancer = 3; // nombre de lancer de dé que le joueur a pour son tour
    bool achoisides = true;
	int choixdes;
	//scanf("%d", &nbjoueur);
	initialiser_plateau(false, nbjoueur);// Initialise le plateau de base pour 4 joueurs

	//==================== Affichage =========================

	afficher_arrière_plan(); // Affiche le plateau de jeu et le plateau des oeufs et tt

	//======================= Code du jeu ====================
    
	while (partie && !fin_jeu) {
		
		Sleep(10); // Petite pause pour éviter de lire trop de touches à la fois)
        if (etapejeu == 0) {
            if(nblancer > 0) {
                afficher_texte_pixel("Voulez vous lancer les de ? ", 0, 0, 0xFF0040);
                afficher_de(joueur);// demande au joueur de choisir les des qu'il veut garder

				if (achoisides) { 
					lancerde = lire_touche(); // demande au joueur si il veut lancer le de ou non 
				}

				if ((lancerde == '1') && achoisides) {
					logiquede(joueur);
					achoisides = false;
					nblancer--;
					lancerde = 0;
				}

				if (!achoisides) {
					afficher_de(joueur); // Affiche les dés à l'écran pour le joueur actuel
					afficher_texte_pixel("Vous gardez quels des ? B pour bloquer/debloquer, Entrer pour finir", 0, 0, 0xFF0040);
					achoisides = selectiondes(); // demande au joueur quels dés il veut garder après le lancer de dés  
				}
				// printf("nombre de lancer : %d\n", nblancer);
            }
            else {
                etapejeu = 1; // Passe à l'étape suivante du tour de jeu
                lancerde = 0; // réinitialise le lancer de dé pour le prochain tour
            }
        }
        else if (etapejeu == 1) {
			printf("Déplacement des figurines en cours...\n");
			actions_des(); // Permet de faire les actions des dés gardés par le joueur
            etapejeu = 2;
        }
        else if (etapejeu == 2) {
			printf("Vérification des séismes en cours...\n");
            etapejeu = 3;
        }
        else if (etapejeu == 3) {
            printf("Déplacement du pion et vérification des éruptions en cours...\n");
            //etapejeu = 1;
		}

        
        
        
        
        
		printf("FIN BOUCLE DE JEU\n");
    }
    //============================================================================


    
    bouger_curseur(0, 200);// Déplace le curseur tout en bas pour ne pas casser l'affichage
    printf("\nRendu termine! Appuyez sur Entree pour quitter...");

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
* for (int i = 0; i < 7; i++) {
			   printf("De %d : bloque = %d, action = %d\n", i, Listede[i].bloque, Listede[i].action); // pour vérifier la valeur des dés après le lancer
		}
* 
* 
* 
* 
* 
* printf("touche appuyé : %d\n", lire_touche()); // pour vérifier la valeur de la touche appuyé dans la fonction lire_touche() et ainsi vérifier que les touches sont bien lues
* 
*/