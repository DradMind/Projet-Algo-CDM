#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <windows.h>


// ===================================================================================
// Fonctions de gestion de la console et du rendu
void initialiser_console(int dezoom_count) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Activer le mode de traitement des séquences d'échappement ANSI pour les couleurs
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1C, 0, 0);
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    Sleep(500); // On attend que la fenêtre soit en plein écran

    for (int i = 0; i < dezoom_count; i++) {
        keybd_event(VK_CONTROL, 0x1D, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void bouger_curseur(int x, int y) {
    // Faut multiplier x par 2 car chaque "pixel" est en réalité deux caractères (pour un rendu plus carré)
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    // extrait les composantes R, G, B du code hexadécimal
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

    bouger_curseur(x, y);
    // Print deux espaces avec le fond coloré en RGB
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}

//==================================================================================
//fonctions de dessin

void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < longueur; i++) {
            dessine_pixel_hex(x + i, y + j, hex_couleur);
        }
    }
    bouger_curseur(0, 42);
}

void dessiner_cercle(int centre_x, int centre_y, int rayon, int hex_couleur) {
    for (int y = -rayon; y <= rayon; y++) {
        for (int x = -rayon; x <= rayon; x++) {
            if (x * x + y * y <= rayon * rayon) {
                dessine_pixel_hex(centre_x + x, centre_y + y, hex_couleur);
            }
        }
    }
    bouger_curseur(0, 42);
}