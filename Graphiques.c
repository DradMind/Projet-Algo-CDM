#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <windows.h>
#include "Logique.h"
#include <conio.h>
#include "Police.h"

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
    for (int dx = 0; dx < 1; dx++) {
        bouger_curseur(x, y + dx);
        printf("\x1b[48;2;%d;%d;%dm", r, g, b);

        for (int dy = 0; dy < 1; dy++) {
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
            dessiner_rectangle(posx + x * 25, posy + y * 25, 25, 25, couleur_case);
        }
    }
}

// Graphiques.c



void afficher_texte_pixel(const char* texte, int x, int y, int hex_couleur) {
    int cur_x = x;

    for (int i = 0; texte[i] != '\0'; i++) {
        char c = texte[i];
        int index = -1;

        if (c >= 'A' && c <= 'Z') index = c - 'A';
        else if (c >= 'a' && c <= 'z') index = c - 'a';
        else if (c == '.') index = 26;
        else if (c == ',') index = 27;
        else if (c == ':') index = 28;
        else if (c == ';') index = 29;
        else if (c == '!') index = 30;
        else if (c == '?') index = 31;
        else if (c == '(') index = 32;
        else if (c == ')') index = 33;
        else if (c == '-') index = 34;
        else if (c == '+') index = 35;
        else if (c == '=') index = 36;
        else if (c == ' ') {
            cur_x += 4; // Espace entre les mots
            continue;
        }

        if (index != -1) {
            for (int row = 0; row < 5; row++) {
                unsigned char row_data = font5x5_extended[index][row];
                for (int col = 0; col < 5; col++) {
                    if ((row_data >> (4 - col)) & 1) {
                        // Utilise votre fonction de dessin existante
                        dessine_pixel_hex(cur_x + col, y + row, hex_couleur);
                    }
                }
            }
            cur_x += 6; // Espacement standard entre caractères
        }
    }
}

void afficher_arrière_plan() {
	printf("\x1b[2J"); //permet d'effacer l'écran avant de tout afficher
    dessiner_rectangle(9, 9, 102, 102, 0xFFFFFF); // Fond du plateau 
    afficherplateau(10, 10); // Affiche le plateau à l'écran
    dessiner_rectangle(120, 0, 67, 116, 0xFFFFFF); // Plateau des oeufs et tt 
}

void afficher_de(int joueur) {
    for (int i = 0; i < 5 + possedetitanosaure(joueur); i++) {
        int type_de = Listede[i].action;
        int couleur_de;
        switch (type_de) {
            case 0: couleur_de = 0xFF0000; break; // Rouge
            case 1: couleur_de = 0x0000FF; break; // Bleu
            case 2: couleur_de = 0x00FF00; break; // Vert
            case 3: couleur_de = 0x800080; break; // Violet
            case 4: couleur_de = 0xFFFF00; break; // Jaune
            case 5: couleur_de = 0xFFA500; break; // Orange
            case 6: couleur_de = 0xA52A2A; break; // Marron
            default: couleur_de = 0x000000; break;
        }
		if (Listede[i].selectionne) {
			dessiner_rectangle(139, 9 + i * 12, 12, 12, 0x00FFF7); // Fond cyan pour les dés sélectionnés
		}
        else {
            dessiner_rectangle(139, 9 + i * 12, 12, 12, 0xFFFFFF);
        }
		if (Listede[i].bloque) {
			dessiner_rectangle(152, 10 + i * 12, 4, 10, 0x000000); // Petite bare noir à coté pour bloquer
		} else {
			dessiner_rectangle(152, 10 + i * 12, 4, 10, 0xFFFFFF); // Efface le marqueur si pas bloqué
		}
		dessiner_rectangle(140, 10 + i * 12, 10, 10, couleur_de);
	}
}
        
