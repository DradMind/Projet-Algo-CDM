#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

// =============================================================
//  Étapes du tour
// =============================================================
typedef enum {
    ETAPE_LANCER = 0,  // lancer / bloquer les dés
    ETAPE_ACTIONS = 1,  // résoudre les actions des dés
    ETAPE_FIN_TOUR = 2,  // passer au joueur suivant
    ETAPE_FIN_JEU = 3,  // éruption : fin de partie
} EtapeJeu;

int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

    Jeu      jeu = { 0 };
    EtatAction ea = { 0 };

    initialiser_plateau(&jeu, false, 2);

    int       joueur = 0;
    EtapeJeu  etape = ETAPE_LANCER;
    int       nblancer = 3;
    bool      achoisides = true;  // true = joueur doit appuyer sur ESPACE
    int       gagnant = -1;

    while (!WindowShouldClose()) {

        Vector2 souris = GetMousePosition();

        // ============================================================
        //  LOGIQUE
        // ============================================================

        if (etape == ETAPE_LANCER) {

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
                        etape = ETAPE_FIN_JEU;
                    }
                    else {
                        etape = (ea.sous_etat == ACTION_FINI)
                            ? ETAPE_FIN_TOUR
                            : ETAPE_ACTIONS;
                    }
                }
            }
        }

        else if (etape == ETAPE_ACTIONS) {

            // Clic sur le plateau → transmis à traiter_action
            int  clic_l = -1, clic_c = -1;
            bool clic_valide = false;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                clic_valide = clic_sur_plateau(souris, &clic_l, &clic_c);

            bool fini = traiter_action(&jeu, &ea, joueur,
                clic_l, clic_c, clic_valide);

            if (fini) {
                if (verifier_eruption(&jeu)) {
                    calculer_points(&jeu);
                    gagnant = joueur_gagnant(&jeu);
                    etape = ETAPE_FIN_JEU;
                }
                else {
                    etape = ETAPE_FIN_TOUR;
                }
            }
        }

        else if (etape == ETAPE_FIN_TOUR) {
            // Réinitialiser pour le joueur suivant
            joueur = (joueur + 1) % jeu.nb_joueurs;
            nblancer = 3;
            achoisides = true;

            // Débloquer tous les dés pour le nouveau joueur
            for (int i = 0; i < 7; i++) {
                jeu.Listede[i].bloque = false;
                jeu.Listede[i].selectionne = false;
                jeu.Listede[i].action = 0;
            }

            etape = ETAPE_LANCER;
        }

        else if (etape == ETAPE_FIN_JEU) {
            // Attendre ECHAP pour quitter
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }

        // ============================================================
        //  RENDU
        // ============================================================
        BeginDrawing();

        if (etape == ETAPE_LANCER) {
            affichage_jeu(&jeu, joueur, 0, nblancer, achoisides);

        }
        else if (etape == ETAPE_ACTIONS) {
            affichage_actions(&jeu, joueur, &ea);

        }
        else if (etape == ETAPE_FIN_JEU) {
            // Afficher le plateau en arrière-plan puis l'écran de fin
            affichage_jeu(&jeu, joueur, 0, 0, false);
            affichage_fin(&jeu, gagnant);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}