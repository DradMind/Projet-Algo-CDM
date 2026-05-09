#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

    Jeu jeu = { 0 };
    EtatAction   ea = { 0 };
    PhaseEruption pe = { 0 };

    initialiser_plateau(&jeu, false, 2);

    int  joueur = 0;
    int  etape = 0;   // 0=lancers 1=actions 2=eruption 3=fin_tour 4=fin_partie
    int  nblancer = nb_lancers_total(&jeu, joueur);
    bool achoisides = true;
    int  gagnant = -1;

    while (!WindowShouldClose()) {

        Vector2 souris = GetMousePosition();

        // ============================================================
        //  LOGIQUE
        // ============================================================

        // ── Étape 0 : lancers ──
        if (etape == 0) {
            if (achoisides && IsKeyPressed(KEY_SPACE)) {
                logiquede(&jeu, joueur);
                achoisides = false;
                nblancer--;
            } 
            if (!achoisides)
                selectiondes(&jeu, joueur, souris, true);

            if (!achoisides && IsKeyPressed(KEY_ENTER)) {
                if (nblancer > 0) {
                    achoisides = true;
                }
                else {
                    init_etat_action(&jeu, &ea, joueur);
                    if (ea.sous_etat == ACTION_FINI) {
                        if (verifier_volcan(&jeu)) {
                            init_phase_eruption(&pe);
                            etape = 2;
                        } else if (joueur_a_majorite_volcan(&jeu, joueur)) {
                            init_phase_decalage(&pe);
                            etape = 5;
                        } else {
                            etape = 3;
                        }
                    } else {
                        etape = 1;
                    }
                }
            }
        }

        // ── Étape 1 : actions ──
        else if (etape == 1) {
            int  clic_l = -1, clic_c = -1;
            bool clic_valide = false;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                clic_valide = clic_sur_plateau(souris, &clic_l, &clic_c);

            bool fini = traiter_action(&jeu, &ea, joueur, clic_l, clic_c, clic_valide);
            if (fini) {
                if (verifier_volcan(&jeu)) {
                    init_phase_eruption(&pe);
                    etape = 2;
                } else if (joueur_a_majorite_volcan(&jeu, joueur)) {
                    init_phase_decalage(&pe);
                    etape = 5;
                } else {
                    etape = 3;
                }
            }
        }

        // ── Étape 2 : éruption ──
        else if (etape == 2) {
            bool eruption_finie = traiter_eruption(&jeu, &pe);
            if (eruption_finie) {
                if (partie_terminee(&jeu)) {
                    gagnant = joueur_gagnant(&jeu);
                    etape = 4;
                }
                else {
                    etape = 3;
                }
            }
        }

        // ── Étape 3 : fin de tour, passer au joueur suivant ──
        else if (etape == 3) {
            joueur = (joueur + 1) % jeu.nb_joueurs;
            nblancer = nb_lancers_total(&jeu, joueur);
            achoisides = true;
            for (int i = 0; i < 7; i++) {
                jeu.Listede[i].bloque = false;
                jeu.Listede[i].selectionne = false;
                jeu.Listede[i].action = 0;
            }
            etape = 0;
        }

        // ── Étape 4 : fin de partie ──
        else if (etape == 4) {
            if (IsKeyPressed(KEY_E)) break;
        }

        // ── Étape 5 : décalage ──
        else if (etape == 5) {
            bool decalage_fini = traiter_eruption(&jeu, &pe);
            if (decalage_fini) {
                etape = 3;
            }
        }

        // ============================================================
        //  RENDU
        // ============================================================
        BeginDrawing();

        if (etape == 0)
            affichage_jeu(&jeu, joueur, 0, nblancer, achoisides);
        else if (etape == 1)
            affichage_actions(&jeu, joueur, &ea);
        else if (etape == 2 || etape == 5)
            affichage_eruption(&jeu, &pe, joueur);
        else if (etape == 4) {
            affichage_jeu(&jeu, joueur, 0, 0, false);
            affichage_fin(&jeu, gagnant);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}