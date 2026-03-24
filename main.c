#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Graphiques.h"

// ==========================================
//             Programme Principal
// ==========================================
int main() {
    initialiser_console();//calcule le zoom nécessaire pour que le rendu soit adapté à la taille de l'écran
    printf("\x1b[2J");// Efface l'ecran (Clear Screen)


	dessiner_rectangle(20, 20, 1, 1, 0xFFCC8A); //Rectangle de fond autorisé pour le jeu (au dessus le zoom bug)







    dessiner_image_ppm("Images/CMF.ppm", 0, 0);






	// Déplace le curseur en bas pour éviter les artefacts de rendu
    bouger_curseur(0, 42);
    printf("Rendu termine! Appuyez sur Entree pour quitter...");
	getchar(); //permet de garder la console ouverte jusqu'à ce que l'utilisateur appuie sur Entrée

    return 0;
}
