#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <stdbool.h>
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));

    initialiser_fenetre();
    initialiser_plateau(false, 2);

    int  joueur = 0;
    bool partie = true;
    bool fin_jeu = false;
    int  etapejeu = 0;
    int  nblancer = 3;
    bool achoisides = true;

    while (!WindowShouldClose() && partie && !fin_jeu) {

        // ============ LOGIQUE ============

        if (etapejeu == 0) {
            if (nblancer > 0) {

                // Lancer les dés avec ESPACE
                if (achoisides && IsKeyPressed(KEY_SPACE)) {
                    logiquede(joueur);
                    achoisides = false;
                    nblancer--;
                }

                // Sélection des dés à garder
                if (!achoisides) {
                    if (IsKeyPressed(KEY_UP)) { /* selectiondes() gérera ça */ }
                    if (IsKeyPressed(KEY_DOWN)) {}
                    if (IsKeyPressed(KEY_B)) {}
                    if (IsKeyPressed(KEY_ENTER)) {
                        achoisides = true;          // prêt pour le prochain lancer
                        if (nblancer == 0) etapejeu = 1;
                    }
                }

            }
            else {
                etapejeu = 1;
                nblancer = 3;
                achoisides = true;
            }

        }
        else if (etapejeu == 1) {
            // action_des() est encore bloquante – à convertir en state machine
            // action_des(joueur);
            etapejeu = 2;

        }
        else if (etapejeu == 2) {
            // séismes – à implémenter
            etapejeu = 3;

        }
        else if (etapejeu == 3) {
            // fin de tour – passer au joueur suivant
            joueur = (joueur + 1) % 2;
            etapejeu = 0;
            nblancer = 3;
        }

        // ============ RENDU ============

        BeginDrawing();

        afficher_arriere_plan();
        afficher_de(joueur);

        // Messages HUD selon l'étape
        if (etapejeu == 0) {
            if (achoisides)
                afficher_message("ESPACE pour lancer les des", WINDOW_H - 50, YELLOW);
            else
                afficher_message("Fleches+B pour bloquer  ENTREE pour valider", WINDOW_H - 50, SKYBLUE);

            afficher_message(TextFormat("Lancers restants : %d", nblancer), WINDOW_H - 28, LIGHTGRAY);
        }

        // Joueur actuel
        DrawText(TextFormat("Joueur %d", joueur + 1), PANEL_X + 10, WINDOW_H - 55, 18,
            (Color) {
            255, 200, 60, 255
        });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}