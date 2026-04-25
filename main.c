#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

    Jeu Jeu = { 0 };
    initialiser_plateau(&Jeu, false, 2);

    int  joueur = 0;
    bool partie = true;
    bool fin_jeu = false;
    int  etapejeu = 0;
    int  nblancer = 3;
    bool achoisides = true;

    while (!WindowShouldClose() && partie && !fin_jeu) {
        Vector2 pos_souris = GetMousePosition();

        // ============ LOGIQUE ============

        if (etapejeu == 0) {
            if (nblancer > 0) {
                if (achoisides && IsKeyPressed(KEY_SPACE)) {
                    logiquede(&Jeu, joueur);
                    achoisides = false;
                    nblancer--;
                }
                if (!achoisides) {
                    // Clic gauche = bloquer/débloquer un dé
                    selectiondes(&Jeu, joueur, pos_souris, true);

                    if (IsKeyPressed(KEY_ENTER)) {
                        achoisides = true;
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
            // action_des(&Jeu, joueur);  // encore bloquant, à faire
            etapejeu = 2;

        }
        else if (etapejeu == 2) {
            etapejeu = 3;

        }
        else if (etapejeu == 3) {
            joueur = (joueur + 1) % 2;
            etapejeu = 0;
            nblancer = 3;
        }

        // ============ RENDU ============
        BeginDrawing();
        affichage_jeu(&Jeu, joueur, etapejeu, nblancer, achoisides);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}