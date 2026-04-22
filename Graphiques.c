#include "Graphiques.h"
#include "Logique.h"

// =============================================================
//  Graphiques.c  –  Rendu raylib natif
//  Pas de conversion hex, pas de pixels ASCII, pas de Windows.h
// =============================================================

void initialiser_fenetre(void) {
    InitWindow(WINDOW_W, WINDOW_H, "Colere de la Montagne de Feu");
    SetTargetFPS(TARGET_FPS);
}

// -------------------------------------------------------------
//  Plateau 4×4
// -------------------------------------------------------------
void afficherplateau(void) {
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            int type = plateau[row][col].TypeCase;
            Color coul = (type >= 0 && type <= 5) ? COULEUR_CASE[type] : BLACK;

            Rectangle rect = {
                BOARD_X + col * CELL_PX,
                BOARD_Y + row * CELL_PX,
                CELL_PX - 2,
                CELL_PX - 2
            };

            DrawRectangleRec(rect, coul);
            DrawRectangleLinesEx(rect, 2, (Color) { 0, 0, 0, 120 });
        }
    }
}

// -------------------------------------------------------------
//  Dés dans le panneau latéral
// -------------------------------------------------------------
void afficher_de(int joueur) {
    int nb_des = 5 + possedetitanosaure(joueur);

    for (int i = 0; i < nb_des; i++) {
        int action = Listede[i].action;
        Color coul = (action >= 0 && action <= 5) ? COULEUR_DE[action] : GRAY;

        int x = PANEL_X + 10;
        int y = PANEL_Y + 10 + i * (DIE_H + 6);

        // Surbrillance si sélectionné
        if (Listede[i].selectionne) {
            DrawRectangle(x - 4, y - 4, PANEL_W - 12, DIE_H + 8, (Color) { 0, 220, 200, 50 });
            DrawRectangleLinesEx((Rectangle) { x - 4, y - 4, PANEL_W - 12, DIE_H + 8 }, 1, SKYBLUE);
        }

        // Carré coloré du dé
        DrawRectangle(x, y, DIE_H, DIE_H, coul);
        DrawRectangleLines(x, y, DIE_H, DIE_H, BLACK);

        // Nom de la face
        DrawText(NOM_FACE[action], x + DIE_H + 8, y + DIE_H / 2 - 10, 16, WHITE);

        // Indicateur "bloqué"
        if (Listede[i].bloque) {
            DrawText("BLOQUE", x + DIE_H + 8, y + DIE_H / 2 + 4, 12, RED);
        }
    }
}

// -------------------------------------------------------------
//  Fond général
// -------------------------------------------------------------
void afficher_arriere_plan(void) {
    ClearBackground((Color) { 35, 28, 20, 255 });

    // Zone plateau
    DrawRectangle(BOARD_X - 4, BOARD_Y - 4, 4 * CELL_PX + 8, 4 * CELL_PX + 8,
        (Color) {
        60, 50, 35, 255
    });

    afficherplateau();

    // Panneau latéral
    DrawRectangle(PANEL_X, PANEL_Y, PANEL_W, WINDOW_H, (Color) { 22, 18, 14, 255 });
    DrawLine(PANEL_X, 0, PANEL_X, WINDOW_H, (Color) { 80, 60, 40, 255 });
    DrawText("DES", PANEL_X + 10, WINDOW_H - 30, 18, (Color) { 200, 160, 60, 255 });
}

// -------------------------------------------------------------
//  Ligne de texte HUD (bas d'écran ou position libre)
// -------------------------------------------------------------
void afficher_message(const char* texte, int y, Color couleur) {
    DrawText(texte, BOARD_X, y, 16, couleur);
}