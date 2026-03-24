#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "Graphiques.h"

// Charge et dessine une image PPM ASCII (P3) à partir d'un fichier
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur: Impossible d'ouvrir %s\n", nom_fichier);
        return;
    }

    char type[3];
    int width, height, max_color;

    if (fscanf(fichier, "%2s %d %d %d", type, &width, &height, &max_color) != 4) {
        fclose(fichier);
        return;
    }

    if (type[0] != 'P' || type[1] != '3') {
        printf("Erreur: Le fichier n'est pas un P3 ASCII.\n");
        fclose(fichier);
        return;
    }

    int r, g, b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (fscanf(fichier, "%d %d %d", &r, &g, &b) == 3) {
                bouger_curseur(start_x + x, start_y + y);
                printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
            }
        }
    }

    fclose(fichier);
    bouger_curseur(0, 42);
}




// ==========================================
//             Programme Principal
// ==========================================
int main() {
    initialiser_console(20);
// Efface l'ecran (Clear Screen)
    printf("\x1b[2J");

    dessiner_rectangle(0, 0, 80, 40, 0x2A2A2A);
    dessiner_rectangle(5, 5, 70, 30, 0x0055FF);
    dessiner_image_ppm("Images/estaca.ppm", 10, 10);

	// Déplace le curseur en bas pour éviter les artefacts de rendu
    bouger_curseur(0, 42);
    printf("Rendu termine! Appuyez sur Entree pour quitter...");
    getchar();

    return 0;
}
