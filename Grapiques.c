#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>

// ===================================================================================
// Fonctions internes
// ===================================================================================

static void appliquer_taille_police(HANDLE hOut) {
    // 1. Force the program to be "DPI Aware". 
    // This tells Windows 11 NOT to artificially zoom the app, giving us true 4K/1080p metrics.
    // We load it dynamically so it doesn't crash on older compilers (like CodeBlocks/MinGW).
    HMODULE hUser32 = GetModuleHandle(L"user32.dll");
    if (hUser32) {
        typedef BOOL(WINAPI* PFN_SETPROCESSDPIAWARE)(void);
        PFN_SETPROCESSDPIAWARE setDpiAware = (PFN_SETPROCESSDPIAWARE)GetProcAddress(hUser32, "SetProcessDPIAware");
        if (setDpiAware) setDpiAware();
    }

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);

    // Leave a 10% margin at the bottom for the Windows Taskbar
    int max_h = (int)(sh * 0.90);
    int max_w = sw;

    // Calculate maximum font height based on screen width
    // 1 pixel = 2 characters wide. Total chars width = CANVAS_LARGEUR * 2
    // Font width is usually half of font height.
    int font_h_from_w = max_w / CANVAS_LARGEUR;

    // Calculate maximum font height based on screen height
    int font_h_from_h = max_h / CANVAS_HAUTEUR;

    // Take the minimum of both to ensure the canvas fits entirely on the screen
    int font_h = (font_h_from_w < font_h_from_h) ? font_h_from_w : font_h_from_h;
    if (font_h < 4) font_h = 4; // Absolute minimum

    CONSOLE_FONT_INFOEX cfi;
    memset(&cfi, 0, sizeof(cfi));
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;            // 0 = Windows automatically deduces the width for Consolas
    cfi.dwFontSize.Y = (SHORT)font_h;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");

    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

static void configurer_buffer(HANDLE hOut) {
    SHORT buf_w = (SHORT)(CANVAS_LARGEUR * 2);
    SHORT buf_h = (SHORT)(CANVAS_HAUTEUR + 2); // Small margin at the bottom

    // Shrink window to 1x1 BEFORE changing buffer (Windows blocks making buffer smaller than window)
    SMALL_RECT minRect = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(hOut, TRUE, &minRect);

    COORD bufSize = { buf_w, buf_h };
    SetConsoleScreenBufferSize(hOut, bufSize);

    // Set exact window size
    SMALL_RECT winRect = { 0, 0, buf_w - 1, buf_h - 1 };
    SetConsoleWindowInfo(hOut, TRUE, &winRect);
}

// ===================================================================================
// Fonctions publiques
// ===================================================================================

void initialiser_console() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Enable ANSI Escape Codes for Colors
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // 2. Hide blinking cursor
    CONSOLE_CURSOR_INFO cci = { 1, FALSE };
    SetConsoleCursorInfo(hOut, &cci);

    // 3. Apply calculated Font Size (DPI Aware)
    appliquer_taille_police(hOut);

    // 4. Set exact Buffer Size
    configurer_buffer(hOut);

    // 5. Maximize Window reliably
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        ShowWindow(hwnd, SW_MAXIMIZE); // Standard Win32 Maximize function
    }
}

// ===================================================================================
// Dessin de base
// ===================================================================================

void bouger_curseur(int x, int y) {
    // ANSI sequences are base 1, and x is multiplied by 2 because 1 pixel = 2 spaces
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

    bouger_curseur(x, y);
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b); // Double space "  " makes a square pixel
}

// ===================================================================================
// Image PPM ASCII (P3)
// ===================================================================================

void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        bouger_curseur(0, CANVAS_HAUTEUR);
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
    bouger_curseur(0, CANVAS_HAUTEUR);
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
    bouger_curseur(0, CANVAS_HAUTEUR);
}

void dessiner_cercle(int centre_x, int centre_y, int rayon, int hex_couleur) {
    for (int cy = -rayon; cy <= rayon; cy++) {
        for (int cx = -rayon; cx <= rayon; cx++) {
            if (cx * cx + cy * cy <= rayon * rayon) {
                dessine_pixel_hex(centre_x + cx, centre_y + cy, hex_couleur);
            }
        }
    }
    bouger_curseur(0, CANVAS_HAUTEUR);
}