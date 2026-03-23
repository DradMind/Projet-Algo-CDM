#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>


void dessiner_rectangle(int x, int y, int longueur, int hauteur, int couleur);
void draw_image_from_file(const char* filename, int start_x, int start_y);
void setup_fullscreen_pixel_console(int game_w, int game_h);

void setup_fullscreen_pixel_console(int game_w, int game_h) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Activer ANSI
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	// 2. Récupérer la taille de l'écran pour calculer la taille de la police
    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

	// 3. Calcul de la taille de la police pour que les "pixels" soient carrés et que tout rentre à l'écran
    // We want 'game_h' rows to fit in 'screen_h' pixels.
    int font_height = screen_h / game_h;
    if (font_height < 2) font_height = 4; // Safety minimum

	// 4. Force un font carré et mono pour que les "pixels" soient vraiment carrés
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = font_height;
    cfi.dwFontSize.Y = font_height;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Lucida Console"); // Font standard pour les consoles Windows
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

	// 5. Mettre la taille du buffer et de la fenêtre pour correspondre à notre résolution de jeu
	// On multiplie la largeur par 2 pour compenser les caractères plus larges
    COORD bufferSize = { (short)(game_w * 2), (short)game_h };
    SMALL_RECT windowSize = { 0, 0, (short)(game_w * 2 - 1), (short)(game_h - 1) };

	// D'abord on ajuste la taille de la fenêtre pour éviter les erreurs lors du changement de taille du buffer
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);

	// 6. On maximise la fenêtre pour qu'elle prenne tout l'écran
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}




void bouger_curseur(int x, int y) {
	// On multiplie x par 2 pour compenser les caractères plus larges 
    printf("\x1b[%d;%dH", y, x * 2);
}




void dessine_pixel_hex(int x, int y, int hex_couleur){
    int r = (hex_couleur >> 16) & 0xFF;
    int g = (hex_couleur >> 8) & 0xFF;
    int b = hex_couleur & 0xFF;
    bouger_curseur(x, y);
	//Gère la couleur de fond et affiche deux espaces pour faire un "pixel" carré
    printf("\x1b[48;2;%d;%d;%dm  \x1b[0m", r, g, b);
}



void dessiner_rectangle(int x, int y, int longueur, int hauteur, int couleur) {
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < longueur; i++) {
            dessine_pixel_hex(x + i, y + j, couleur);
        }
    }
}



void draw_image_from_file(const char* filename, int start_x, int start_y) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreurr: Fichier introuvable %s\n", filename);
        return;
    }

    int img_w, img_h;
	// lit les dimensions de l'image à partir du fichier
    if (fscanf(file, "%d %d", &img_w, &img_h) != 2) return;

    unsigned int hex;
    for (int y = 0; y < img_h; y++) {
        for (int x = 0; x < img_w; x++) {
            if (fscanf(file, "%i", &hex) == 1) {
                dessine_pixel_hex(start_x + x, start_y + y, hex);
            }
        }
    }

    fclose(file);
    bouger_curseur(0, start_y + img_h + 1);
}




int main() {
    setup_fullscreen_pixel_console(1920, 1080);
	dessiner_rectangle(5, 5, 10, 10, 0xFF0000); // Rouge
    draw_image_from_file("Images/level1.txt", 10, 10);
    bouger_curseur(0, 20);
    printf("Fin du programme.");
    getchar();
    return 0;
}

