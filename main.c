#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

// =============================================================
//  Étapes du tour
// =============================================================
int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

    Jeu jeu = { 0 };
    EtatAction ea = { 0 };

    initialiser_plateau(&jeu, false, 2);

    int joueur = 0;
    int etape = 0;
    int nblancer = 3;
    bool achoisides = true;  // true = joueur doit appuyer sur ESPACE
    int gagnant = -1;

    while (!WindowShouldClose()) {

        Vector2 souris = GetMousePosition();

        // ============================================================
        //  LOGIQUE
        // ============================================================

        if (etape == 0) {

            // Lancer avec ESPACE
            if (achoisides && IsKeyPressed(KEY_SPACE)) {
                logiquede(&jeu, joueur);
                achoisides = false;
                nblancer--;
            }

            // Bloquer/débloquer un dé au clic
            if (!achoisides)
                selectiondes(&jeu, joueur, souris, true);

            // ENTREE : valider la sélection et relancer ou passer aux actions
            if (!achoisides && IsKeyPressed(KEY_ENTER)) {
                if (nblancer > 0) {
                    achoisides = true;  // prêt pour le prochain lancer
                }
                else {
                    // Plus de lancers : passer aux actions
                    init_etat_action(&jeu, &ea, joueur);

                    // Si éruption déclenchée par les faces Volcan
                    if (verifier_eruption(&jeu)) {
                        calculer_points(&jeu);
                        gagnant = joueur_gagnant(&jeu);
                        etape = 3;
                    }
                    else {
                        etape = (ea.sous_etat == ACTION_FINI)
                            ? 2
                            : 1;
                    }
                }
            }
        }

        else if (etape == 1) {

            // Clic sur le plateau → transmis à traiter_action
            int clic_l = -1;
            int clic_c = -1;
            bool clic_valide = false;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                clic_valide = clic_sur_plateau(souris, &clic_l, &clic_c);

            bool fini = traiter_action(&jeu, &ea, joueur, clic_l, clic_c, clic_valide);

            if (fini) {
                if (verifier_eruption(&jeu)) {
                    calculer_points(&jeu);
                    gagnant = joueur_gagnant(&jeu);
                    etape = 3;
                }
                else {
                    etape = 2;
                }
            }
        }

        else if (etape == 2) {
            // Réinitialiser pour le joueur suivant
            joueur++;
            nblancer = 3;
            achoisides = true;

            // Débloquer tous les dés pour le nouveau joueur
            for (int i = 0; i < 7; i++) {
                jeu.Listede[i].bloque = false;
                jeu.Listede[i].selectionne = false;
                jeu.Listede[i].action = 0;
            }

            etape = 0;
        }

        else if (etape == 3) {
            // Attendre ECHAP pour quitter
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }

        // ============================================================
        //  RENDU
        // ============================================================
        BeginDrawing();

        if (etape == 0) {
            affichage_jeu(&jeu, joueur, 0, nblancer, achoisides);

        }
        else if (etape == 1) {
            affichage_actions(&jeu, joueur, &ea);

        }
        else if (etape == 3) {
            // Afficher le plateau en arrière-plan puis l'écran de fin
            affichage_jeu(&jeu, joueur, 0, 0, false);
            affichage_fin(&jeu, gagnant);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}