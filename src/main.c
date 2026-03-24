#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>

// ==========================================
// 1. INITIALIZATION & SCALING
// ==========================================
void initialiser_console(int dezoom_count) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // Enable 24-bit TrueColor ANSI Escape Sequences
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // Teacher's Trick: Alt + Enter (Toggle Fullscreen)
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1C, 0, 0);
    keybd_event(VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
    Sleep(500); // Wait for fullscreen animation

    // Teacher's Trick: Ctrl + Minus (Zoom Out)
    for (int i = 0; i < dezoom_count; i++) {
        keybd_event(VK_CONTROL, 0x1D, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, 0, 0);
        keybd_event(VK_OEM_MINUS, 0xBD, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

// ==========================================
// 2. DRAWING PRIMITIVES (HEX COLORS)
// ==========================================
void bouger_curseur(int x, int y) {
    // Console coordinates start at 1,1. 
    // We multiply x by 2 to make the characters "square" pixels.
    printf("\x1b[%d;%dH", y + 1, (x * 2) + 1);
}

void dessine_pixel_hex(int x, int y, int hex_couleur) {
    // Extract R, G, B from the Hex integer (e.g., 0xFF5733)
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;

    bouger_curseur(x, y);
    // Print two spaces with the extracted RGB background color
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}

void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < longueur; i++) {
            dessine_pixel_hex(x + i, y + j, hex_couleur);
        }
    }
    bouger_curseur(0, 42);
}

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

    // Move cursor out of the way at the bottom so text doesn't overlap graphics
    bouger_curseur(0, 42);
    printf("Rendu termine! Appuyez sur Entree pour quitter...");
    getchar();

    return 0;
}