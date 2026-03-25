#define _CRT_SECURE_NO_WARNINGS
#include "Graphiques.h"
#include <stdio.h>
<<<<<<< HEAD
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
=======
#include <windows.h>

int scale_factor = 1;

// ===================================================================================
// Fonctions de gestion de la console et du rendu
void initialiser_console(int dezoom_count) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Activer le mode de traitement des séquences d'échappement ANSI pour les couleurs
>>>>>>> parent of 9316425 (fix de claude (ia))
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

<<<<<<< HEAD
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
=======
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


void set_echelle(int echelle) {
    if (echelle > 0) {
        scale_factor = echelle;
    }
}

int calculer_dezoom_auto() {
    int screen_height = GetSystemMetrics(SM_CYSCREEN);

    if (screen_height >= 2160) {
        set_echelle(3);
        return 10; // Dezoom count
    }
    else if (screen_height >= 1440) {
        // 1440p Monitor
        set_echelle(2);
        return 10;
    }
    else {
        // 1080p Monitor
        set_echelle(1); // Standard 1x scale
        return 4;
    }
}

void bouger_curseur(int x, int y) {
    // Faut multiplier x par 2 car chaque "pixel" est en réalité deux caractères (pour un rendu plus carré)
>>>>>>> parent of 9316425 (fix de claude (ia))
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

<<<<<<< HEAD
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
=======
    // Loop through the height of the mega-pixel
    for (int dy = 0; dy < scale_factor; dy++) {

        // 1. Move cursor ONCE per row (Much faster!)
        bouger_curseur(x * scale_factor, (y * scale_factor) + dy);

        // 2. Set the color ONCE per row
        printf("\x1b[48;2;%d;%d;%dm", r, g, b);

        // 3. Print the full width of the mega-pixel all at once
        for (int dx = 0; dx < scale_factor; dx++) {
            printf("  "); // 2 spaces per logical pixel
        }

        // 4. Reset the color so it doesn't bleed to the next line
        printf("\x1b[0m");
    }
}

// Charge et dessine une image PPM ASCII (P3) à partir d'un fichier
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y) {
    FILE* fichier = fopen(nom_fichier, "r");
    if (!fichier) {
>>>>>>> parent of 9316425 (fix de claude (ia))
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
<<<<<<< HEAD
    bouger_curseur(0, CANVAS_HAUTEUR);
=======
    bouger_curseur(0, 42);
>>>>>>> parent of 9316425 (fix de claude (ia))
}

//==================================================================================
//fonctions de dessin

void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < longueur; i++) {
            dessine_pixel_hex(x + i, y + j, hex_couleur);
        }
    }
<<<<<<< HEAD
    bouger_curseur(0, CANVAS_HAUTEUR);
=======
    bouger_curseur(0, 42);
>>>>>>> parent of 9316425 (fix de claude (ia))
}

void dessiner_cercle(int centre_x, int centre_y, int rayon, int hex_couleur) {
    for (int y = -rayon; y <= rayon; y++) {
        for (int x = -rayon; x <= rayon; x++) {
            if (x * x + y * y <= rayon * rayon) {
                dessine_pixel_hex(centre_x + x, centre_y + y, hex_couleur);
            }
        }
    }
<<<<<<< HEAD
    bouger_curseur(0, CANVAS_HAUTEUR);
=======
    bouger_curseur(0, 42);
>>>>>>> parent of 9316425 (fix de claude (ia))
}