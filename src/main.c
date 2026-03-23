#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

// 1. Enables colors in the Windows Console
void initialiser_console() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// 2. Moves the cursor. Multiply X by 2 for "square" pixels
void bouger_curseur(int x, int y) {
    // Console coordinates start at 1,1 (not 0,0)
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

// 3. The Function to read and draw the PPM P3 image
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur: Impossible d'ouvrir %s\n", nom_fichier);
        return;
    }

    char type[3];
    int width, height, max_color;

    // Read the header (Expects "P3", then width, height, and max color)
    if (fscanf(fichier, "%2s %d %d %d", type, &width, &height, &max_color) != 4) {
        printf("Erreur de lecture de l'en-tete.\n");
        fclose(fichier);
        return;
    }

    // Verify it is actually an ASCII PPM file
    if (type[0] != 'P' || type[1] != '3') {
        printf("Erreur: Le fichier n'est pas au format P3 ASCII.\n");
        fclose(fichier);
        return;
    }

    // Read every pixel and draw it
    int r, g, b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (fscanf(fichier, "%d %d %d", &r, &g, &b) == 3) {
                bouger_curseur(start_x + x, start_y + y);
                // Print two spaces with the RGB background color
                printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
            }
        }
    }

    fclose(fichier);

    // Move cursor out of the way when finished
    bouger_curseur(0, start_y + height + 2);
}

// 4. The Main loop
int main() {
    initialiser_console();

    // Clear screen
    printf("\x1b[2J");

    // Make sure your file is in the same folder as the executable!
    dessiner_image_ppm("Images/VERT_test2.ppm", 5, 5);

    printf("\nImage terminee! Appuyez sur Entree...");
    getchar();
    return 0;
}