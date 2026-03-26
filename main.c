#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Graphiques.h"
#include "Logique.h"
#include <stdbool.h>

int main() {
    // =========================================================================
    // PARAMÈTRE D'AFFICHAGE (POUR L'ÉVALUATEUR) :
    // Si l'affichage déborde de l'écran ou semble mal cadré, modifiez le zoom :
    // - Pour un écran standard 1080p : utilisez 4 ou 5
    // - Pour un écran Haute Résolution (2K/4K) : utilisez 10 ou 12
    // =========================================================================
    int dezoom_manuel = 5;

    // Initialise le plein écran et applique le dézoom
    initialiser_console(dezoom_manuel);
    // Efface l'écran (Clear Screen)
    printf("\x1b[2J");

	//============================================================================

	initialiser_plateau(true, 4); // Initialise le plateau de base pour 4 joueurs
	afficherplateau(); // Affiche le plateau à l'écran
    


	//============================================================================
    
    // Déplace le curseur tout en bas pour ne pas casser l'affichage
    bouger_curseur(0, 45);
    printf("\nRendu termine! Appuyez sur Entree pour quitter...");
    getchar();

    return 0;
}