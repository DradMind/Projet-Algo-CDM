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
void afficherplateau(Jeu* jeu) {
    // Le fond marron du plateau (un peu plus grand que les cases)
    int bordure = PE(1, GetScreenWidth()); // 1% d'épaisseur
    DrawRectangle(BOARD_X - bordure, BOARD_Y - bordure,
        (4 * CELL_PX) + (2 * bordure), (4 * CELL_PX) + (2 * bordure),
        (Color) {
        60, 50, 35, 255
    });

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int type = jeu->plateau[y][x].TypeCase;
            Color couleur = (type >= 0 && type <= 5) ? COULEUR_CASE[type] : BLACK;

            // Calcul propre de la position de chaque case
            Rectangle Case = {
                BOARD_X + (x * CELL_PX),
                BOARD_Y + (y * CELL_PX),
                CELL_PX,
                CELL_PX
            };

            DrawRectangleRec(Case, couleur);
            DrawRectangleLinesEx(Case, 2, (Color) { 0, 0, 0, 120 });
        }
    }
}

// -------------------------------------------------------------
//  Dés dans le panneau latéral
// -------------------------------------------------------------
void afficher_de(Jeu* jeu, int joueur) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = PANEL_X + 10;
        int y = PANEL_Y + 10 + i * (DIE_H + 6);
        Rectangle rect = { x, y, DIE_H, DIE_H };

        if (jeu->Listede[i].bloque) {
            DrawRectangleRec(rect, (Color) { 100, 100, 100, 255 }); // Gris pour les dés bloqués
            DrawRectangleLinesEx(rect, 2, BLACK);
            continue; // Ne pas afficher la valeur des dés bloqués
		}

        // Change la couleur si le dé est sélectionné
        Color dieColor = jeu->Listede[i].selectionne ? GREEN : LIGHTGRAY;
        DrawRectangleRec(rect, dieColor);
        DrawRectangleLinesEx(rect, 2, BLACK);

        // Affiche la valeur de l'action si le dé a été lancé
        DrawText(TextFormat("%d", jeu->Listede[i].action), x + 20, y + 15, 20, BLACK);
    }
}

bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = PANEL_X + 10;
        int y = PANEL_Y + 10 + i * (DIE_H + 6);
        Rectangle rect = { (float)x, (float)y, (float)DIE_H, (float)DIE_H };

        // Si la souris est sur le dé ET que l'utilisateur vient de faire un clic gauche
        if (CheckCollisionPointRec(pos_souris, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            jeu->Listede[i].selectionne = !jeu->Listede[i].selectionne; // Toggle sélection
            return true;
        }
    }
    return false; // TRÈS IMPORTANT : renvoyer false si rien n'est cliqué !
}

int PE(float pourcentage, float valeur) { //sa sert surtout pour des pourcentages de l'écran, pour des écrans de taille différents
    int x = valeur / 100 * pourcentage;
    return x;
}