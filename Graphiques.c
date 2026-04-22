#include "Graphiques.h"
#include "Logique.h"

void initialiser_fenetre(void) {
    InitWindow(1, 1, "");

    int ecran = GetCurrentMonitor();
    int Largeur_ecran = GetMonitorWidth(ecran);
    int Hauteur_ecran = GetMonitorHeight(ecran);

    int Largeur_voulu = 1920;
    int Hauteur_voulu = 1080;

    if (Largeur_ecran > 1920 || Hauteur_ecran > 1080) { // pour calibrer la bonne résolution entre moi et toi (et le prof)
        Largeur_voulu = 2560;
        Hauteur_voulu = 1440;
    }

    // 4. On redimensionne et on affiche la fenêtre
    SetWindowSize(Largeur_voulu, Hauteur_voulu);
    SetWindowTitle("Colere de la Montagne de Feu");
    SetTargetFPS(60);
}

// -------------------------------------------------------------
//  Plateau 4×4
// -------------------------------------------------------------
void afficherplateau(void) {
	DrawRectangle(PE(5, GetScreenWidth()), PE(5, GetScreenHeight()), PE(40, GetScreenWidth()), PE(40, GetScreenHeight()), (Color) { 60, 50, 35, 255 });
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int type = plateau[y][x].TypeCase;
			Color couleur = (type >= 0 && type <= 5) ? COULEUR_CASE[type] : BLACK; // Si le type de case est valide, on prend la couleur correspondante, sinon on utilise le noir par défaut
            Rectangle Case = { PE(5, GetScreenWidth()) * x,PE(5, GetScreenWidth()) * y,PE(5, GetScreenWidth()),PE(5, GetScreenWidth()) }; // c'est le modèle de la case
            DrawRectangleRec(Case, couleur);
            DrawRectangleLinesEx(Case, 2, (Color) { 0, 0, 0, 120 });
        }
    }
}

// -------------------------------------------------------------
//  Dés dans le panneau latéral
// -------------------------------------------------------------
void afficher_de(int joueur) {
    
}

// -------------------------------------------------------------
//  Fond général
// -------------------------------------------------------------


void afficher_arriere_plan(void) {
    ClearBackground((Color) { 35, 28, 20, 255 });

    // Zone plateau
    DrawRectangle(BOARD_X - 4, BOARD_Y - 4, 4 * CELL_PX + 8, 4 * CELL_PX + 8,(Color) {60, 50, 35, 255});

    afficherplateau();

    // Panneau latéral
    DrawRectangle(PANEL_X, PANEL_Y, PANEL_W, GetScreenHeight(), (Color) { 22, 18, 14, 255 });
    DrawLine(PANEL_X, 0, PANEL_X, GetScreenHeight(), (Color) { 80, 60, 40, 255 });
    DrawText("DES", PANEL_X + 10, GetScreenHeight() - 30, 18, (Color) { 200, 160, 60, 255 });
}

bool selectiondes(int joueur, Vector2 pos_souris) { //on fait passer en pointeur pour pouvoir modifier la valeur directe (pas une copie) de choixdes 
    for (int i = 0; i < 5 + possedetitanosaure(joueur); i++) {
        int x = PANEL_X + 10;
        int y = PANEL_Y + 10 + i * (DIE_H + 6);
        Rectangle rect = { x, y, DIE_H, DIE_H };
        if (CheckCollisionPointRec(pos_souris, rect)) {
            Listede[i].selectionne = !Listede[i].selectionne; // Toggle sélection
            return true;
        }
	}
}

int PE(float pourcentage, float valeur) { //sa sert surtout pour des pourcentages de l'écran, pour des écrans de taille différents
    int x = valeur / 100 * pourcentage;
    return x;
}