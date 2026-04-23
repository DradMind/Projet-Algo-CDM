#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

    //La variable locale qui contient TOUT le jeu !
    Jeu Jeu = { 0 }; // {0} met tout à zéro proprement par défaut

    initialiser_plateau(&Jeu, false, 2);

    int joueur = 0;
    bool partie = true;
    bool fin_jeu = false;
    int etapejeu = 0;
    int nblancer = 3;
    bool achoisides = true; // true quand le joueur DOIT appuyer sur ESPACE pour lancer

    while (!WindowShouldClose() && partie && !fin_jeu) {
        Vector2 pos_souris = GetMousePosition();

        if (etapejeu == 0) {
            if (nblancer > 0) {
                // Lancer les dés avec ESPACE
                if (achoisides && IsKeyPressed(KEY_SPACE)) {
                    logiquede(&Jeu, joueur);
                    achoisides = false;
                    nblancer--;
                }

                // Sélection des dés à garder
                if (!achoisides) {
                    selectiondes(&Jeu, joueur, pos_souris, true); // Met en surbrillance les dés cliqués

                    // Si le joueur appuie sur ENTREE, on valide sa sélection pour relancer
                    if (IsKeyPressed(KEY_ENTER)) {
                        achoisides = true; // prêt pour le prochain lancer
                        if (nblancer == 0) {
                            etapejeu = 1;
                        }
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
            // séismes
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
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color) { 200, 200, 200, 255 }); // Fond gris clair
		afficherplateau(&Jeu);
        afficher_de(&Jeu, joueur);

        // Messages HUD selon l'étape
        if (etapejeu == 0) {
            if (achoisides)
                DrawText("Appuyez sur ESPACE pour lancer les des", TABLEAU_X + 10, GetScreenHeight() - 50, 16, YELLOW);
            else
                DrawText("Cliquez les des a garder puis ENTRER", TABLEAU_X + 10, GetScreenHeight() - 50, 16, SKYBLUE);

            DrawText(TextFormat("Lancers restants : %d", nblancer), TABLEAU_X + 10, GetScreenHeight() - 28, 16, LIGHTGRAY);
        }

        // Joueur actuel
        DrawText(TextFormat("Joueur %d", joueur + 1), TABLEAU_X + 10, GetScreenHeight() - 75, 18, (Color) { 255, 200, 60, 255 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}