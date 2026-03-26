#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <windows.h>
#include "Logique.h"
#include <conio.h>

// Échelle verrouillée à 2 pour garantir des pixels carrés
const int SCALE_FACTOR = 2;

void initialiser_console(int dezoom_count) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Activer les couleurs TrueColor (ANSI)
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // Cacher le curseur clignotant
    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &cci);

    // 1. Alt + Entrée (Force le mode Plein Ecran)
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1C, 0, 0);
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    Sleep(500); // Laisse le temps à l'animation de se finir

    // 2. Ctrl + Moins (Dézoom Manuel)
    for (int i = 0; i < dezoom_count; i++) {
        keybd_event(VK_CONTROL, 0x1D, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void bouger_curseur(int x, int y) {
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

    // Rendu du pixel "Scale 2" (Carré parfait)
    for (int dx = 0; dx < SCALE_FACTOR; dx++) {
        bouger_curseur(x * SCALE_FACTOR, (y * SCALE_FACTOR) + dx);
        printf("\x1b[48;2;%d;%d;%dm", r, g, b);

        for (int dy = 0; dy < SCALE_FACTOR; dy++) {
            printf("  "); // 2 espaces = 1 sub-pixel carré
        }
        printf("\x1b[0m"); // Reset couleur
    }
}

void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur) {
    for (int i = 0; i < longueur; i++) {
        for (int j = 0; j < hauteur; j++) {
            dessine_pixel_hex(x + i, y + j, hex_couleur);
        }
    }
}

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

    int r, g, b;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (fscanf(fichier, "%d %d %d", &r, &g, &b) == 3) {
                int hex_couleur = (r << 16) | (g << 8) | b;
                dessine_pixel_hex(start_x + x, start_y + y, hex_couleur);
            }
        }
    }

    fclose(fichier);
}

void afficher_texte(const char* texte, int x, int y, int hex_texte, int hex_fond) {
    // Extraction RGB pour le texte 
    int r_t = (hex_texte >> 16) & 0xFF;
    int g_t = (hex_texte >> 8) & 0xFF;
    int b_t = hex_texte & 0xFF;

    // pour ele fond
    int r_f = (hex_fond >> 16) & 0xFF;
    int g_f = (hex_fond >> 8) & 0xFF;
    int b_f = hex_fond & 0xFF;

    bouger_curseur(x * SCALE_FACTOR, y * SCALE_FACTOR);
    printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm%s\x1b[0m",r_t, g_t, b_t, r_f, g_f, b_f, texte);
}

void afficherplateau(int posx, int posy) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int type_case = plateau[y][x].TypeCase;
            int couleur_case;
            switch (type_case) {
                case 0: couleur_case = 0xFF4500; break; // Volcan
                case 1: couleur_case = 0x1E90FF; break; // Eau
                case 2: couleur_case = 0x228B22; break; // Jungle
                case 3: couleur_case = 0x7CFC00; break; // Prairie
                case 4: couleur_case = 0x8B4513; break; // Hutte
                case 5: couleur_case = 0xA9A9A9; break; // Caverne
                default: couleur_case = 0x000000; break;
            }
            dessiner_rectangle(posx + x * 10, posy + y * 10, 10, 10, couleur_case);
        }
    }
}

int lire_touche() {
    if (_kbhit()) {
        int c = _getch();
        if (c == 0 || c == 224) { // Touche spéciale (Flèches)
            return _getch(); // Retourne le code spécifique (72=Haut, 80=Bas, 75=Gauche, 77=Droite)
        }
        return c; // Retourne le caractère (ex: 'z', 'q', etc.)
    }
    return -1; // Rien n'a été pressé
}