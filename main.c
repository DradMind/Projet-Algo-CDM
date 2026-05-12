#include "raylib.h"
#include "Graphiques.h"
#include "Logique.h"
#include <time.h>

int main(void) {
    srand((unsigned int)time(NULL));
    initialiser_fenetre();

	Jeu jeu = { 0 }; //On définit tout à 0 pour éviter les problèmes de valeurs indéfinies
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
        //  BOUCLE PRINCIPALE DE LOGIQUE
        //  La boucle de jeu est divisée en plusieurs "étapes" (phases du tour d'un joueur).
        // ============================================================

        // ── Étape 0 : Lancer et sélection des dés ──
        // Le joueur lance ses dés, peut bloquer certains résultats qu'il souhaite conserver,
        // puis relancer les dés restants (jusqu'à épuisement de ses lancers autorisés).
        if (etape == 0) {
            if (achoisides && IsKeyPressed(KEY_SPACE)) {
                logiquede(&jeu, joueur);
                achoisides = false;
                nblancer--;
            } 
            if (!achoisides)
                selectiondes(&jeu, joueur, souris, true);

            // Validation de la sélection avec Entrée
            if (!achoisides && IsKeyPressed(KEY_ENTER)) {
                if (nblancer > 0) {
                    achoisides = true; // Encore des lancers possibles
                }
                else {
                    // Fin des lancers : On prépare l'état pour la phase d'actions
                    init_etat_action(&jeu, &ea, joueur);
                    if (ea.sous_etat == ACTION_FINI) {
                        // S'il n'a aucune action valide, on passe directement à la suite du tour :
                        // 1. Vérifier si le volcan entre en éruption (6 pions)
                        if (verifier_volcan(&jeu)) {
                            init_phase_eruption(&pe);
                            etape = 2;
                        // 2. Sinon, s'il a la majorité sur le volcan, il peut faire un "décalage" (séisme)
                        } else if (joueur_a_majorite_volcan(&jeu, joueur)) {
                            init_phase_decalage(&pe);
                            etape = 5;
                        // 3. Sinon, c'est la fin de son tour
                        } else {
                            etape = 3;
                        }
                    } else {
                        // Le joueur a des actions à jouer
                        etape = 1;
                    }
                }
            }
        }

        // ── Étape 1 : Exécution des actions ──
        // Le joueur dépense les actions obtenues sur ses dés (déployer, déplacer, incuber, etc.)
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

        // ── Étape 2 : Éruption Volcanique ──
        // Cette phase s'enclenche lorsqu'il y a au moins 6 pions sur la case Volcan.
        // Elle engendre le calcul des points, puis le joueur majoritaire peut déplacer une case du plateau.
        else if (etape == 2) {
            bool eruption_finie = traiter_eruption(&jeu, &pe);
            if (eruption_finie) {
                // L'éruption est terminée. Vérifie si les conditions de victoire sont remplies
                if (partie_terminee(&jeu)) {
                    gagnant = joueur_gagnant(&jeu);
                    etape = 4; // Phase de victoire
                }
                else {
                    etape = 3; // Fin de tour classique
                }
            }
        }

        // ── Étape 3 : Fin de tour ──
        // On passe la main au joueur suivant et on réinitialise ses lancers de dés
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

        // ── Étape 4 : Fin de partie ──
        // Un joueur a atteint le score de victoire. Le jeu s'arrête.
        else if (etape == 4) {
            if (IsKeyPressed(KEY_E)) break;
        }

        // ── Étape 5 : Séisme (Décalage de terrain) ──
        // Identique à la seconde partie de l'éruption, mais se déclenche juste
        // parce que le joueur a la majorité sur le volcan (sans forcément 6 pions)
        else if (etape == 5) {
            bool seisme_fini = traiter_eruption(&jeu, &pe);
            if (seisme_fini) {
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