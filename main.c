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
    Vector2 pos_souris = { 0 };





    while (!WindowShouldClose() && partie && !fin_jeu) {
		pos_souris = GetMousePosition();


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
                    if (selectiondes(joueur, pos_souris)) {
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
                DrawText("Appuyez sur ESPACE pour lancer les dés", PANEL_X + 10, GetScreenHeight() - 50, 16, YELLOW);
            else
                DrawText("Fleches+B pour bloquer  ENTREE pour valider", PANEL_X + 10, GetScreenHeight() - 50, 16, SKYBLUE);
			DrawText(TextFormat("Lancers restants : %d", nblancer), PANEL_X + 10, GetScreenHeight() - 28, 16, LIGHTGRAY);
        }

        // Joueur actuel
        DrawText(TextFormat("Joueur %d", joueur + 1), PANEL_X + 10, GetScreenHeight() - 55, 18,(Color) {255, 200, 60, 255});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}