#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "Graphiques.h"
#include <stdio.h>


int main()
{
    initialiser_rendu();
    plein_ecran();

    // Draw a modern, square-celled board
    dessiner_echiquier_moderne(5, 10);

    positionner_curseur(0, 0);
    printf(FG_RGB(255, 255, 255) "Appuyez sur une touche pour quitter..." RESET);

    getchar();
	return 0;
}


void dessiner_rectangle_rgb(int ligne, int colonne, int h, int w, int r, int g, int b) {
    for (int i = 0; i < h; i++) {
        positionner_curseur(ligne + i, colonne);
        // Set background color
        printf("\x1b[48;2;%d;%d;%dm", r, g, b);
        for (int j = 0; j < w; j++) {
            printf("  "); // Double space fix for square aspect ratio
        }
        printf(RESET);
    }
}