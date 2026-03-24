#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

// ===================================================================================
// Fonctions internes
// ===================================================================================

static void appliquer_taille_police(HANDLE hOut) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    // On pilote uniquement dwFontSize.Y (hauteur) car Windows ignore souvent X.
    // Avec Consolas, le ratio naturel est ~2:1 (hauteur:largeur), donc :
    //   largeur d'un caractère ≈ font_h / 2
    //   largeur d'un pixel     = 2 chars × (font_h/2) = font_h
    //   hauteur d'un pixel     = font_h
    //   → pixel carré automatiquement, sans toucher à X.
    //
    // Pour que CANVAS_LARGEUR pixels tiennent en largeur : font_h = sw / CANVAS_LARGEUR
    // Pour que CANVAS_HAUTEUR pixels tiennent en hauteur : font_h = sh / CANVAS_HAUTEUR
    // On prend le minimum pour que tout rentre dans les deux dimensions.
    int font_h = sw / CANVAS_LARGEUR;
    int from_h = sh / CANVAS_HAUTEUR;
    if (from_h < font_h) font_h = from_h;
    if (font_h < 4) font_h = 4;

    CONSOLE_FONT_INFOEX cfi;
    memset(&cfi, 0, sizeof(cfi));
    cfi.cbSize       = sizeof(cfi);
    cfi.nFont        = 0;
    cfi.dwFontSize.X = 0;            // 0 = Windows choisit la largeur proportionnelle
    cfi.dwFontSize.Y = (SHORT)font_h;
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

static void configurer_buffer(HANDLE hOut) {
    SHORT buf_w = (SHORT)(CANVAS_LARGEUR * 2);
    SHORT buf_h = (SHORT)(CANVAS_HAUTEUR + 10);

    // Réduire la fenêtre au minimum AVANT de modifier le buffer
    // (Windows refuse sinon si le buffer devient plus petit que la fenêtre)
    SMALL_RECT minRect = {0, 0, 1, 1};
    SetConsoleWindowInfo(hOut, TRUE, &minRect);

    COORD bufSize = {buf_w, buf_h};
    SetConsoleScreenBufferSize(hOut, bufSize);

    SMALL_RECT winRect = {0, 0, buf_w - 1, buf_h - 1};
    SetConsoleWindowInfo(hOut, TRUE, &winRect);
}

// ===================================================================================
// Fonctions publiques
// ===================================================================================



void initialiser_console() {


    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Activer les séquences de couleur ANSI
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // 2. Cacher le curseur clignotant
    CONSOLE_CURSOR_INFO cci = {1, FALSE};
    SetConsoleCursorInfo(hOut, &cci);

    // 3. Appliquer la police AVANT de maximiser.
    //    SetCurrentConsoleFontEx fonctionne mieux sur une fenêtre non-maximisée.
    appliquer_taille_police(hOut);

    // 4. Configurer le buffer à la taille exacte de la toile
    configurer_buffer(hOut);

    // 5. Maximiser la fenêtre.
    //    SetWindowPlacement est plus fiable que ShowWindow quand le programme
    //    est lancé depuis Visual Studio (dont le débogueur possède partiellement
    //    la console, ce qui fait que ShowWindow est parfois ignoré).
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd, &wp);
        wp.showCmd = SW_SHOWMAXIMIZED;
        SetWindowPlacement(hwnd, &wp);
    }
}

// ===================================================================================
// Dessin de base
// ===================================================================================

void bouger_curseur(int x, int y) {
    // Les séquences ANSI sont en base 1, et x*2 car 2 chars par pixel
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >>  8) & 0xFF;
    int b =  hex_couleur        & 0xFF;

    bouger_curseur(x, y);
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}

// ===================================================================================
// Image PPM ASCII (P3)
// ===================================================================================

void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        bouger_curseur(0, CANVAS_HAUTEUR + 2);
        printf("Erreur: Impossible d'ouvrir %s\n", nom_fichier);
        return;
    }

    char type[3];
    int width, height, max_color;
    if (fscanf(fichier, "%2s %d %d %d", type, &width, &height, &max_color) != 4
        || type[0] != 'P' || type[1] != '3') {
        printf("Erreur: format PPM invalide.\n");
        fclose(fichier);
        return;
    }

    int r, g, b;
    for (int py = 0; py < height; py++) {
        for (int px = 0; px < width; px++) {
            if (fscanf(fichier, "%d %d %d", &r, &g, &b) == 3) {
                bouger_curseur(start_x + px, start_y + py);
                printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
            }
        }
    }

    fclose(fichier);
    bouger_curseur(0, CANVAS_HAUTEUR + 2);
}

// ===================================================================================
// Formes géométriques
// ===================================================================================

void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < longueur; i++) {
            dessine_pixel_hex(x + i, y + j, hex_couleur);
        }
    }
    bouger_curseur(0, CANVAS_HAUTEUR + 2);
}

void dessiner_cercle(int centre_x, int centre_y, int rayon, int hex_couleur) {
    for (int cy = -rayon; cy <= rayon; cy++) {
        for (int cx = -rayon; cx <= rayon; cx++) {
            if (cx * cx + cy * cy <= rayon * rayon) {
                dessine_pixel_hex(centre_x + cx, centre_y + cy, hex_couleur);
            }
        }
    }
    bouger_curseur(0, CANVAS_HAUTEUR + 2);
}