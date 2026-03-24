#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

// ===================================================================================
// Modifiez CANVAS_LARGEUR et CANVAS_HAUTEUR pour changer la résolution virtuelle
// du jeu. Tous les écrans afficheront exactement ce nombre de "pixels".
// Ces valeurs DOIVENT correspondre à celles définies dans Graphiques.h
// ===================================================================================

// ===================================================================================
// Fonctions internes
// ===================================================================================

static void appliquer_taille_police(HANDLE hOut) {
    int sw = GetSystemMetrics(SM_CXSCREEN);
    // (les cellules de caractères console ont un ratio ~2:1 hauteur/largeur)
    //
    // Pour que CANVAS_LARGEUR pixels remplissent exactement l'écran en largeur :
    //   CANVAS_LARGEUR × 2 chars × font_w px/char = sw px
    //   → font_w = sw / (CANVAS_LARGEUR × 2)
    int font_w = sw / (CANVAS_LARGEUR * 2);
    int font_h = font_w * 2;

    // Valeurs minimales pour rester lisible
    if (font_w < 2) font_w = 2;
    if (font_h < 4) font_h = 4;

    CONSOLE_FONT_INFOEX cfi;
    memset(&cfi, 0, sizeof(cfi));
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = (SHORT)font_w;
    cfi.dwFontSize.Y = (SHORT)font_h;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

static void configurer_buffer(HANDLE hOut) {
    // Largeur en chars = CANVAS_LARGEUR × 2  (2 chars par pixel)
    // Hauteur en lignes = CANVAS_HAUTEUR + marge pour les messages texte
    SHORT buf_w = (SHORT)(CANVAS_LARGEUR * 2);
    SHORT buf_h = (SHORT)(CANVAS_HAUTEUR + 10);

    // Il faut réduire la fenêtre AVANT de réduire le buffer,
    // et agrandir le buffer AVANT d'agrandir la fenêtre.
    // On commence donc par mettre la fenêtre au minimum.
    SMALL_RECT minRect = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOut, TRUE, &minRect);

    COORD bufSize = { buf_w, buf_h };
    SetConsoleScreenBufferSize(hOut, bufSize);

    SMALL_RECT winRect = { 0, 0, buf_w - 1, buf_h - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &winRect);
}







// ===================================================================================
// Fonctions de gestion de la console et du rendu
// ===================================================================================

void initialiser_console() {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Activer le mode de traitement des séquences ANSI pour les couleurs
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // Mettre en plein écran (Alt+Entrée) — fonctionne dans cmd.exe / conhost
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1C, 0, 0);
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    Sleep(500); // Attendre que la fenêtre soit en plein écran

    // Appliquer la taille de police calculée pour cet écran
    appliquer_taille_police(hOut);

    // Ajuster la taille du buffer et de la fenêtre console
    configurer_buffer(hOut);

    // Cacher le curseur clignotant
    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &cci);
}










// ===================================================================================
// Fonctions de dessin de base
// ===================================================================================

void bouger_curseur(int x, int y) {
    // +1 car les séquences ANSI sont en base 1
    // x * 2 car chaque pixel fait 2 chars de large
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

    bouger_curseur(x, y);
    // 2 espaces = 1 pixel (2 chars larges × 1 ligne = carré grâce au ratio police)
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}

// ===================================================================================
// Chargement et affichage d'image PPM ASCII (P3)
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