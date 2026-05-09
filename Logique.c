#define _CRT_SECURE_NO_WARNINGS
#include "Logique.h"
#include "Graphiques.h"
#include <string.h>

// =============================================================
//  INITIALISATION
// =============================================================

void initialiser_plateau(Jeu* jeu, bool plateauBase, int nbJoueurs) {
    srand((unsigned int)time(NULL));
    memset(jeu, 0, sizeof(Jeu));
    jeu->nb_joueurs = nbJoueurs;

    for (int i = 0; i < nbJoueurs; i++) {
        jeu->Joueurs[i].numerojoueur = i;
        jeu->Joueurs[i].reserve = 10;
        jeu->Joueurs[i].points = 0;
        jeu->Joueurs[i].a_dino = false;
        jeu->Joueurs[i].dino_possede = DINO_AUCUN;
        jeu->Joueurs[i].oeufs = 0;
    }
    
    for (int i = 0; i < 4; i++) {
        jeu->dinos_disponibles[i] = 2; // 2 de chaque espèce
    }

    if (plateauBase) {
        int base[4][4] = {
            { 1, 1, 3, 4 },
            { 3, 2, 2, 2 },
            { 1, 3, 0, 3 },
            { 3, 5, 2, 2 }
        };
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                jeu->plateau[i][j].TypeCase = base[i][j];
    }
    else {
        int pool[16] = { 0, 1,1,1, 2,2,2,2,2, 3,3,3,3,3, 4, 5 };
        for (int i = 15; i > 0; i--) {
            int r = rand() % (i + 1);
            int tmp = pool[i]; pool[i] = pool[r]; pool[r] = tmp;
        }
        int idx = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                jeu->plateau[i][j].TypeCase = pool[idx++];
    }
}

// =============================================================
//  DÉS
// =============================================================

void logiquede(Jeu* jeu, int joueur) {
    int nb = 5 + possedetitanosaure(jeu, joueur);
    for (int i = 0; i < nb; i++)
        if (!jeu->Listede[i].bloque)
            jeu->Listede[i].action = rand() % 6;
}

int possedetitanosaure(Jeu* jeu, int joueur) {
    if (jeu->Joueurs[joueur].a_dino &&
        jeu->Joueurs[joueur].dino_possede == DINO_TITANOSAURE)
        return 1;
    return 0;
}

int nb_lancers_total(Jeu* jeu, int joueur) {
    if (jeu->Joueurs[joueur].a_dino &&
        jeu->Joueurs[joueur].dino_possede == DINO_TRICERATOPS)
        return 4;
    return 3;
}

bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = TABLEAU_X;
        int y = TABLEAU_Y + i * (DE_H + PE(1, GetScreenHeight()));
        Rectangle rect = { (float)x, (float)y, (float)DE_H, (float)DE_H };
        if (CheckCollisionPointRec(pos_souris, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (blocage)
                jeu->Listede[i].bloque = !jeu->Listede[i].bloque;
            else
                jeu->Listede[i].selectionne = !jeu->Listede[i].selectionne;
            return true;
        }
    }
    return false;
}

// =============================================================
//  PIONS
// =============================================================

void rajouter_pion(Jeu* jeu, int ligne, int col, int joueur) {
    for (int p = 0; p < 10; p++) {
        if (jeu->PlateauPion[ligne][col][p].TypePion == 0) {
            jeu->PlateauPion[ligne][col][p].TypePion = 1;
            jeu->PlateauPion[ligne][col][p].joueur = joueur;
            jeu->PlateauPion[ligne][col][p].dino = DINO_AUCUN;
            jeu->plateau[ligne][col].nbpion++;
            return;
        }
    }
}

void rajouter_dino(Jeu* jeu, int ligne, int col, int joueur, TypeDino type) {
    for (int p = 0; p < 10; p++) {
        if (jeu->PlateauPion[ligne][col][p].TypePion == 0) {
            jeu->PlateauPion[ligne][col][p].TypePion = 2;
            jeu->PlateauPion[ligne][col][p].joueur = joueur;
            jeu->PlateauPion[ligne][col][p].dino = type;
            jeu->plateau[ligne][col].nbpion++;
            jeu->Joueurs[joueur].a_dino = true;
            jeu->Joueurs[joueur].dino_possede = type;
            jeu->Joueurs[joueur].dino_ligne = ligne;
            jeu->Joueurs[joueur].dino_col = col;
            return;
        }
    }
}

void enlever_pion(Jeu* jeu, int ligne, int col, int place) {
    if (jeu->PlateauPion[ligne][col][place].TypePion != 0) {
        jeu->PlateauPion[ligne][col][place].TypePion = 0;
        jeu->PlateauPion[ligne][col][place].joueur = 0;
        jeu->PlateauPion[ligne][col][place].dino = DINO_AUCUN;
        jeu->plateau[ligne][col].nbpion--;
    }
}

int compter_pions_joueur(Jeu* jeu, int joueur, int ligne, int col) {
    int nb = 0;
    for (int p = 0; p < 10; p++)
        if (jeu->PlateauPion[ligne][col][p].TypePion >= 1 &&
            jeu->PlateauPion[ligne][col][p].joueur == joueur)
            nb++;
    return nb;
}

int compter_total_case(Jeu* jeu, int ligne, int col) {
    int nb = 0;
    for (int p = 0; p < 10; p++)
        if (jeu->PlateauPion[ligne][col][p].TypePion != 0)
            nb++;
    return nb;
}

bool case_adjacente(int ligne1, int col1, int ligne2, int col2) {
    int dl = ligne1 - ligne2;
    int dc = col1 - col2;
    return (dl == 0 && (dc == 1 || dc == -1)) || (dc == 0 && (dl == 1 || dl == -1));
}

// =============================================================
//  VOLCAN / ÉRUPTION
// =============================================================

int trouver_volcan_ligne(Jeu* jeu) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (jeu->plateau[i][j].TypeCase == 0) return i;
    return -1;
}

int trouver_volcan_col(Jeu* jeu) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (jeu->plateau[i][j].TypeCase == 0) return j;
    return -1;
}

void avancer_volcan(Jeu* jeu) {
    jeu->eruption_volcan++;
}

bool verifier_volcan(Jeu* jeu) {
    int vl = trouver_volcan_ligne(jeu);
    int vc = trouver_volcan_col(jeu);
    if (vl < 0) return false;
    
    int nb_pions = 0;
    for (int p = 0; p < 10; p++) {
        if (jeu->PlateauPion[vl][vc][p].TypePion != 0) {
            nb_pions++;
        }
    }
    return nb_pions >= 6; // Seuil d'éruption basé sur les pions
}

// =============================================================
//  RÉGIONS (flood-fill 4-connexe sur cases de même type)
// =============================================================

// Remplit 'cellules' avec les cases de la région connectée à (l,c)
// Retourne la taille de la région
static int flood_fill(Jeu* jeu, int l, int c, int type, bool visite[4][4], bool cellules[4][4]) {
    if (l < 0 || l >= 4 || c < 0 || c >= 4) return 0;
    if (visite[l][c]) return 0;
    if (jeu->plateau[l][c].TypeCase != type) return 0;

    visite[l][c] = true;
    cellules[l][c] = true;
    int taille = 1;
    taille += flood_fill(jeu, l - 1, c, type, visite, cellules);
    taille += flood_fill(jeu, l + 1, c, type, visite, cellules);
    taille += flood_fill(jeu, l, c - 1, type, visite, cellules);
    taille += flood_fill(jeu, l, c + 1, type, visite, cellules);
    return taille;
}

int taille_region(Jeu* jeu, int ligne, int col, bool visite[4][4]) {
    bool cellules[4][4] = { {false} };
    return flood_fill(jeu, ligne, col, jeu->plateau[ligne][col].TypeCase, visite, cellules);
}

// Retourne le joueur majoritaire dans un ensemble de cases, ou -1 si égalité
int joueur_majoritaire_region(Jeu* jeu, bool cellules[4][4]) {
    int totaux[4] = { 0 };
    for (int l = 0; l < 4; l++)
        for (int c = 0; c < 4; c++)
            if (cellules[l][c])
                for (int p = 0; p < 10; p++)
                    if (jeu->PlateauPion[l][c][p].TypePion != 0)
                        totaux[jeu->PlateauPion[l][c][p].joueur]++;

    int best = -1, best_val = 0;
    bool egalite = false;
    for (int i = 0; i < 4; i++) {
        if (totaux[i] > best_val) {
            best_val = totaux[i];
            best = i;
            egalite = false;
        }
        else if (totaux[i] == best_val && best_val > 0) {
            egalite = true;
        }
    }
    return egalite ? -1 : best;
}

// =============================================================
//  CALCUL DES POINTS (éruption)
// =============================================================

// Points volcan : majorité = 3pts, présents mais pas majoritaires = 1pt
static void points_volcan(Jeu* jeu) {
    int vl = trouver_volcan_ligne(jeu);
    int vc = trouver_volcan_col(jeu);
    if (vl < 0) return;

    int totaux[4] = { 0 };
    for (int p = 0; p < 10; p++)
        if (jeu->PlateauPion[vl][vc][p].TypePion != 0)
            totaux[jeu->PlateauPion[vl][vc][p].joueur]++;

    int best_val = 0;
    for (int i = 0; i < 4; i++)
        if (totaux[i] > best_val) best_val = totaux[i];

    if (best_val == 0) return;

    bool egalite = false;
    int  best_j = -1;
    for (int i = 0; i < 4; i++) {
        if (totaux[i] == best_val) {
            if (best_j == -1) best_j = i;
            else egalite = true;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (totaux[i] == 0) continue;
        if (!egalite && totaux[i] == best_val)
            jeu->Joueurs[i].points += 3;
        else
            jeu->Joueurs[i].points += 1;
    }
}

// Points régions : pour chaque région Jungle(2) ou Prairie(3),
// le majoritaire gagne autant de pts que la taille de la région
static void points_regions(Jeu* jeu) {
    bool visite[4][4] = { {false} };

    for (int l = 0; l < 4; l++) {
        for (int c = 0; c < 4; c++) {
            if (visite[l][c]) continue;
            int type = jeu->plateau[l][c].TypeCase;
            // On score uniquement Jungle et Prairie
            if (type != 2 && type != 3) {
                visite[l][c] = true;
                continue;
            }

            bool cellules[4][4] = { {false} };
            int  taille = flood_fill(jeu, l, c, type, visite, cellules);
            int  gagnant = joueur_majoritaire_region(jeu, cellules);

            if (gagnant >= 0)
                jeu->Joueurs[gagnant].points += taille;
        }
    }
}

void calculer_points(Jeu* jeu) {
    // Remise à zéro puis recompte simple (utilisé pour l'affichage en cours de partie)
    for (int i = 0; i < 4; i++) jeu->Joueurs[i].points = 0;
    for (int l = 0; l < 4; l++)
        for (int c = 0; c < 4; c++) {
            if (jeu->plateau[l][c].TypeCase == 0) continue;
            for (int p = 0; p < 10; p++)
                if (jeu->PlateauPion[l][c][p].TypePion == 1) {
                    int pj = jeu->PlateauPion[l][c][p].joueur;
                    jeu->Joueurs[pj].points++;
                }
        }
}

void calculer_points_eruption(Jeu* jeu) {
    points_volcan(jeu);
    points_regions(jeu);
}

// =============================================================
//  RESET APRÈS ÉRUPTION
//  - Vide le volcan et les régions scorées
//  - Replace les pions éliminés en réserve
// =============================================================
void executer_eruption_reset(Jeu* jeu) {
    int vl = trouver_volcan_ligne(jeu);
    int vc = trouver_volcan_col(jeu);

    // Remettre les pions du volcan en réserve
    if (vl >= 0) {
        for (int p = 0; p < 10; p++) {
            if (jeu->PlateauPion[vl][vc][p].TypePion != 0) {
                int j = jeu->PlateauPion[vl][vc][p].joueur;
                if (jeu->PlateauPion[vl][vc][p].TypePion == 1)
                    jeu->Joueurs[j].reserve++;
                // dino remis en réserve commune
                if (jeu->PlateauPion[vl][vc][p].TypePion == 2) {
                    TypeDino d = jeu->PlateauPion[vl][vc][p].dino;
                    jeu->Joueurs[j].a_dino = false;
                    jeu->Joueurs[j].dino_possede = DINO_AUCUN;
                    if (d >= 1 && d <= 4) jeu->dinos_disponibles[d - 1]++;
                }
                enlever_pion(jeu, vl, vc, p);
            }
        }
    }

    // Vider les régions Jungle et Prairie scorées, remettre pions en réserve
    bool visite[4][4] = { {false} };
    for (int l = 0; l < 4; l++) {
        for (int c = 0; c < 4; c++) {
            if (visite[l][c]) continue;
            int type = jeu->plateau[l][c].TypeCase;
            if (type != 2 && type != 3) { visite[l][c] = true; continue; }

            bool cellules[4][4] = { {false} };
            flood_fill(jeu, l, c, type, visite, cellules);

            // Vider toutes les cases de cette région
            for (int rl = 0; rl < 4; rl++)
                for (int rc = 0; rc < 4; rc++)
                    if (cellules[rl][rc])
                        for (int p = 0; p < 10; p++)
                            if (jeu->PlateauPion[rl][rc][p].TypePion != 0) {
                                int j = jeu->PlateauPion[rl][rc][p].joueur;
                                if (jeu->PlateauPion[rl][rc][p].TypePion == 1)
                                    jeu->Joueurs[j].reserve++;
                                if (jeu->PlateauPion[rl][rc][p].TypePion == 2) {
                                    TypeDino d = jeu->PlateauPion[rl][rc][p].dino;
                                    jeu->Joueurs[j].a_dino = false;
                                    jeu->Joueurs[j].dino_possede = DINO_AUCUN;
                                    if (d >= 1 && d <= 4) jeu->dinos_disponibles[d - 1]++;
                                }
                                enlever_pion(jeu, rl, rc, p);
                            }
        }
    }

    jeu->eruption_volcan = 0;
    jeu->nb_eruptions++;
}

// =============================================================
//  PHASE ÉRUPTION (machine à états)
// =============================================================

void init_phase_eruption(PhaseEruption* pe) {
    pe->phase = ERUPTION_VOLCAN_SCORE;
    pe->points_calcules = false;
    pe->moves_restants = 1; // 1 déplacement de tuile autorisé
    pe->est_decalage_seul = false;
}

void init_phase_decalage(PhaseEruption* pe) {
    pe->phase = ERUPTION_DEPLACEMENT;
    pe->points_calcules = true;
    pe->moves_restants = 1;
    pe->est_decalage_seul = true;
}

bool joueur_a_majorite_volcan(Jeu* jeu, int joueur) {
    int vl = trouver_volcan_ligne(jeu);
    int vc = trouver_volcan_col(jeu);
    if (vl < 0) return false;

    int totaux[4] = { 0 };
    for (int p = 0; p < 10; p++)
        if (jeu->PlateauPion[vl][vc][p].TypePion != 0)
            totaux[jeu->PlateauPion[vl][vc][p].joueur]++;

    int best_val = totaux[joueur];
    if (best_val == 0) return false;

    for (int i = 0; i < 4; i++) {
        if (i != joueur && totaux[i] > best_val) return false;
    }
    return true;
}

// Retourne true quand toute la phase éruption est terminée
bool traiter_eruption(Jeu* jeu, PhaseEruption* pe) {
    if (pe->phase == ERUPTION_VOLCAN_SCORE) {
        if (!pe->points_calcules) {
            calculer_points_eruption(jeu);
            pe->points_calcules = true;
        }
        // Passer à la suite sur ESPACE ou ENTREE
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            pe->phase = ERUPTION_DEPLACEMENT;
        }
        return false;
    }

    if (pe->phase == ERUPTION_DEPLACEMENT) {
        // Le joueur déplace le volcan avec les flèches
        if (pe->moves_restants > 0) {
            int posx = trouver_volcan_col(jeu);
            int posy = trouver_volcan_ligne(jeu);
            bool bouge = false;

            if (IsKeyPressed(KEY_UP)) {
                // Décaler colonne posx vers le haut
                Case c0 = jeu->plateau[0][posx];
                Pion p0[10]; memcpy(p0, jeu->PlateauPion[0][posx], sizeof(p0));
                for (int l = 0; l < 3; l++) {
                    jeu->plateau[l][posx] = jeu->plateau[l+1][posx];
                    memcpy(jeu->PlateauPion[l][posx], jeu->PlateauPion[l+1][posx], sizeof(jeu->PlateauPion[0][0]));
                }
                jeu->plateau[3][posx] = c0;
                memcpy(jeu->PlateauPion[3][posx], p0, sizeof(p0));
                bouge = true;
            }
            else if (IsKeyPressed(KEY_DOWN)) {
                // Décaler colonne posx vers le bas
                Case c3 = jeu->plateau[3][posx];
                Pion p3[10]; memcpy(p3, jeu->PlateauPion[3][posx], sizeof(p3));
                for (int l = 3; l > 0; l--) {
                    jeu->plateau[l][posx] = jeu->plateau[l-1][posx];
                    memcpy(jeu->PlateauPion[l][posx], jeu->PlateauPion[l-1][posx], sizeof(jeu->PlateauPion[0][0]));
                }
                jeu->plateau[0][posx] = c3;
                memcpy(jeu->PlateauPion[0][posx], p3, sizeof(p3));
                bouge = true;
            }
            else if (IsKeyPressed(KEY_LEFT)) {
                // Décaler ligne posy vers la gauche
                Case c0 = jeu->plateau[posy][0];
                Pion p0[10]; memcpy(p0, jeu->PlateauPion[posy][0], sizeof(p0));
                for (int c = 0; c < 3; c++) {
                    jeu->plateau[posy][c] = jeu->plateau[posy][c+1];
                    memcpy(jeu->PlateauPion[posy][c], jeu->PlateauPion[posy][c+1], sizeof(jeu->PlateauPion[0][0]));
                }
                jeu->plateau[posy][3] = c0;
                memcpy(jeu->PlateauPion[posy][3], p0, sizeof(p0));
                bouge = true;
            }
            else if (IsKeyPressed(KEY_RIGHT)) {
                // Décaler ligne posy vers la droite
                Case c3 = jeu->plateau[posy][3];
                Pion p3[10]; memcpy(p3, jeu->PlateauPion[posy][3], sizeof(p3));
                for (int c = 3; c > 0; c--) {
                    jeu->plateau[posy][c] = jeu->plateau[posy][c-1];
                    memcpy(jeu->PlateauPion[posy][c], jeu->PlateauPion[posy][c-1], sizeof(jeu->PlateauPion[0][0]));
                }
                jeu->plateau[posy][0] = c3;
                memcpy(jeu->PlateauPion[posy][0], p3, sizeof(p3));
                bouge = true;
            }

            if (bouge) pe->moves_restants--;
        }

        // Valider avec ENTREE (ou automatique si plus de moves)
        if (pe->moves_restants == 0 || IsKeyPressed(KEY_ENTER)) {
            if (pe->est_decalage_seul) {
                pe->phase = ERUPTION_FINI;
            } else {
                pe->phase = ERUPTION_RESET;
            }
        }
        return false;
    }

    if (pe->phase == ERUPTION_RESET) {
        executer_eruption_reset(jeu);
        pe->phase = ERUPTION_FINI;
        return false;
    }

    return (pe->phase == ERUPTION_FINI);
}

// =============================================================
//  FIN DE PARTIE
// =============================================================

int seuil_victoire(int nb_joueurs) {
    // Seuils indicatifs selon nombre de joueurs
    switch (nb_joueurs) {
    case 2: return 12;
    case 3: return 10;
    case 4: return  9;
    default: return 12;
    }
}

bool partie_terminee(Jeu* jeu) {
    int seuil = seuil_victoire(jeu->nb_joueurs);
    for (int i = 0; i < jeu->nb_joueurs; i++)
        if (jeu->Joueurs[i].points >= seuil) return true;
    return false;
}

int joueur_gagnant(Jeu* jeu) {
    int best = -1, best_pts = -1;
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        if (jeu->Joueurs[i].points > best_pts) {
            best_pts = jeu->Joueurs[i].points;
            best = i;
        }
    }
    return best;
}

// =============================================================
//  MACHINE À ÉTATS DES ACTIONS
// =============================================================

void init_etat_action(Jeu* jeu, EtatAction* ea, int joueur) {
    memset(ea, 0, sizeof(EtatAction));
    int nb = 5 + possedetitanosaure(jeu, joueur);
    for (int i = 0; i < nb; i++)
        ea->nb_actions[jeu->Listede[i].action]++;

    for (int v = 0; v < ea->nb_actions[0]; v++)
        avancer_volcan(jeu);
    ea->nb_actions[0] = 0;

    // Les faces Oeuf s'ajoutent à la réserve d'oeufs du joueur
    jeu->Joueurs[joueur].oeufs += ea->nb_actions[4];
    ea->nb_actions[4] = 0; // pas une action directe, c'est un stock

    // La face Deplacement (5) compte aussi comme une action de déplacement (3)
    ea->nb_actions[3] += ea->nb_actions[5];
    ea->nb_actions[5] = 0;

    ea->sous_etat = ACTION_MENU;
    ea->action_en_cours = -1;
    ea->orig_ligne = -1;
    ea->orig_col = -1;
    ea->selection_dino = 0;
}

bool traiter_action(Jeu* jeu, EtatAction* ea, int joueur,
    int clic_ligne, int clic_col, bool clic_valide) {
    if (ea->sous_etat == ACTION_FINI) return true;

    // ── MENU ──
    if (ea->sous_etat == ACTION_MENU) {
        int nb_options = 5;
        if (IsKeyPressed(KEY_UP))
            ea->selection_menu = (ea->selection_menu - 1 + nb_options) % nb_options;
        if (IsKeyPressed(KEY_DOWN))
            ea->selection_menu = (ea->selection_menu + 1) % nb_options;

        if (IsKeyPressed(KEY_ENTER)) {
            int sel = ea->selection_menu;

            if (sel == 0 && ea->nb_actions[1] > 0) {
                ea->action_en_cours = 1;
                ea->sous_etat = ACTION_DEPLOYER_CASE;
            }
            else if (sel == 1 && ea->nb_actions[2] > 0) {
                ea->action_en_cours = 2;
                ea->sous_etat = ACTION_DEPLOYER_CASE;
            }
            else if (sel == 2 && ea->nb_actions[3] > 0) {
                ea->action_en_cours = 3;
                ea->sous_etat = ACTION_DEPLACER_ORIGINE;
            }
            else if (sel == 3 && !jeu->Joueurs[joueur].a_dino) {
                // Vérifier qu'on a assez d'oeufs pour au moins un dino
                int min_cout = 2;
                if (jeu->Joueurs[joueur].oeufs >= min_cout) {
                    ea->sous_etat = ACTION_OEUF_CHOISIR;
                    ea->selection_dino = 0;
                }
            }
            else if (sel == 4) {
                ea->sous_etat = ACTION_FINI;
                return true;
            }
        }
        if (IsKeyPressed(KEY_T)) { ea->sous_etat = ACTION_FINI; return true; }
        return false;
    }

    // ── DEPLOYER ──
    if (ea->sous_etat == ACTION_DEPLOYER_CASE) {
        if (IsKeyPressed(KEY_E)) {
            ea->sous_etat = ACTION_MENU;
            ea->action_en_cours = -1;
            return false;
        }
        if (!clic_valide) return false;

        int type_cible = (ea->action_en_cours == 1) ? 5 : 4;
        bool brachio = (jeu->Joueurs[joueur].a_dino &&
            jeu->Joueurs[joueur].dino_possede == DINO_BRACHIO);
        bool case_ok = brachio
            ? (jeu->plateau[clic_ligne][clic_col].TypeCase != 0)
            : (jeu->plateau[clic_ligne][clic_col].TypeCase == type_cible);

        if (case_ok && jeu->Joueurs[joueur].reserve > 0) {
            jeu->Joueurs[joueur].reserve--;
            rajouter_pion(jeu, clic_ligne, clic_col, joueur);
            ea->nb_actions[ea->action_en_cours]--;
            ea->sous_etat = ACTION_MENU;
            ea->action_en_cours = -1;
        }
        return false;
    }

    // ── DÉPLACER ORIGINE ──
    if (ea->sous_etat == ACTION_DEPLACER_ORIGINE) {
        if (IsKeyPressed(KEY_E)) {
            ea->sous_etat = ACTION_MENU;
            return false;
        }
        if (!clic_valide) return false;

        if (compter_pions_joueur(jeu, joueur, clic_ligne, clic_col) > 0 &&
            jeu->plateau[clic_ligne][clic_col].TypeCase != 0) {
            ea->orig_ligne = clic_ligne;
            ea->orig_col = clic_col;
            ea->sous_etat = ACTION_DEPLACER_DEST;
        }
        return false;
    }

    // ── DÉPLACER DESTINATION ──
    if (ea->sous_etat == ACTION_DEPLACER_DEST) {
        if (IsKeyPressed(KEY_E)) {
            ea->sous_etat = ACTION_MENU;
            ea->orig_ligne = -1;
            ea->orig_col = -1;
            return false;
        }
        if (!clic_valide) return false;

        if (clic_ligne == ea->orig_ligne && clic_col == ea->orig_col) {
            ea->sous_etat = ACTION_DEPLACER_ORIGINE;
            return false;
        }
        int p_choisi = -1;
        for (int p = 0; p < 10; p++) {
            if (jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p].TypePion >= 1 &&
                jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p].joueur == joueur) {
                p_choisi = p;
                break;
            }
        }
        if (p_choisi == -1) return false;

        int dl = abs(ea->orig_ligne - clic_ligne);
        int dc = abs(ea->orig_col - clic_col);
        bool adj = case_adjacente(ea->orig_ligne, ea->orig_col, clic_ligne, clic_col);

        int type_pion = jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p_choisi].TypePion;
        TypeDino dino_pion = jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p_choisi].dino;

        bool dist2_saut_eau = false;
        if (type_pion == 1 && !adj && ea->nb_actions[3] >= 2) {
            if ((dl == 2 && dc == 0) || (dl == 0 && dc == 2)) {
                int mid_l = (ea->orig_ligne + clic_ligne) / 2;
                int mid_c = (ea->orig_col + clic_col) / 2;
                if (jeu->plateau[mid_l][mid_c].TypeCase == 1) { // Eau
                    dist2_saut_eau = true;
                }
            }
        }

        int dest_type = jeu->plateau[clic_ligne][clic_col].TypeCase;
        bool valide = false;
        int cout = 1;

        if (dest_type != 0) { // Ne peut pas aller sur le volcan
            if (type_pion == 1) { // Cromagnon
                if (dest_type != 1) { // Ne peut pas s'arrêter sur l'eau
                    if (adj) { valide = true; cout = 1; }
                    else if (dist2_saut_eau) { valide = true; cout = 2; }
                }
            } else if (type_pion == 2) { // Dinosaure
                if (adj) { valide = true; cout = 1; }
                else if (dino_pion == DINO_PTERODACTYLE && (dl + dc <= 2)) { valide = true; cout = 1; }
            }
        }

        if (valide) {
            enlever_pion(jeu, ea->orig_ligne, ea->orig_col, p_choisi);
            if (type_pion == 1) {
                rajouter_pion(jeu, clic_ligne, clic_col, joueur);
            } else {
                rajouter_dino(jeu, clic_ligne, clic_col, joueur, dino_pion);
            }
            ea->nb_actions[3] -= cout;
            ea->orig_ligne = -1;
            ea->orig_col = -1;
            ea->sous_etat = ACTION_MENU;
        }
        return false;
    }

    // ── OEUF : choisir un dino ──
    if (ea->sous_etat == ACTION_OEUF_CHOISIR) {
        int cout_dino[] = { 0, 2, 2, 3, 3 }; // DINO_AUCUN, TRIC, PTERO, BRACH, TITAN
        int nb_dinos = 4;
        if (IsKeyPressed(KEY_UP))
            ea->selection_dino = (ea->selection_dino - 1 + nb_dinos) % nb_dinos;
        if (IsKeyPressed(KEY_DOWN))
            ea->selection_dino = (ea->selection_dino + 1) % nb_dinos;

        // Annuler avec ECHAP
        if (IsKeyPressed(KEY_E)) {
            ea->sous_etat = ACTION_MENU;
            return false;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            TypeDino types[] = { DINO_TRICERATOPS, DINO_PTERODACTYLE,
                                 DINO_BRACHIO, DINO_TITANOSAURE };
            TypeDino choisi = types[ea->selection_dino];
            int cout = cout_dino[choisi];

            if (jeu->Joueurs[joueur].oeufs >= cout && jeu->dinos_disponibles[choisi - 1] > 0) {
                jeu->Joueurs[joueur].oeufs -= cout;
                jeu->dinos_disponibles[choisi - 1]--;

                // Poser le dino sur une case du joueur, sinon première case non-volcan
                bool pose = false;
                for (int l = 0; l < 4 && !pose; l++)
                    for (int c = 0; c < 4 && !pose; c++)
                        if (jeu->plateau[l][c].TypeCase != 0 &&
                            compter_pions_joueur(jeu, joueur, l, c) > 0) {
                            rajouter_dino(jeu, l, c, joueur, choisi);
                            pose = true;
                        }
                if (!pose)
                    for (int l = 0; l < 4 && !pose; l++)
                        for (int c = 0; c < 4 && !pose; c++)
                            if (jeu->plateau[l][c].TypeCase != 0) {
                                rajouter_dino(jeu, l, c, joueur, choisi);
                                pose = true;
                            }

                ea->sous_etat = ACTION_MENU;
            }
            // Si pas assez d'oeufs ou dino indisponible, on reste dans le menu dino
        }
        return false;
    }

    return false; // jamais ACTION_FINI automatiquement, seulement via "Terminer"
}