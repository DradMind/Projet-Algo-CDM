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
    }

    if (plateauBase) {
        int base[4][4] = {
            { 1, 1, 3, 4 },
            { 3, 2, 2, 2 },
            { 1, 3, 0, 3 },
            { 3, 5, 2, 2 }
		}; //ça marche sur plusieurs ligne perso je met comme ça pour pas faire un truc super long

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                jeu->plateau[i][j].TypeCase = base[i][j];
    }
    else {
        int pool[16] = { 0, 1,1,1, 2,2,2,2,2, 3,3,3,3,3, 4, 5 };
        // mélange de Fisher-Yates
        for (int i = 15; i > 0; i--) {
            int r = rand() % (i + 1);
            int tmp = pool[i];
            pool[i] = pool[r];
            pool[r] = tmp;
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
    (void)jeu; (void)joueur;
    return 0; // dinos pas encore fait
}

bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage) {
    // La position des dés est définie dans Graphiques.h via TABLEAU_X/Y/DE_H
    // On recalcule ici de façon identique à afficher_de()
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

void rajouter_pion(Jeu* jeu, int ligne, int col, int joueur) { // la fonction ne rajoute pas encore les dinos 
    for (int p = 0; p < 10; p++) {
        if (jeu->PlateauPion[ligne][col][p].TypePion == 0) {
            jeu->PlateauPion[ligne][col][p].TypePion = 1;
            jeu->PlateauPion[ligne][col][p].joueur = joueur;
            jeu->plateau[ligne][col].nbpion++;
            return;
        }
    }
}

void enlever_pion(Jeu* jeu, int ligne, int col, int place) { //tout a 0 comme si y avait rien
    if (jeu->PlateauPion[ligne][col][place].TypePion != 0) {
        jeu->PlateauPion[ligne][col][place].TypePion = 0;
        jeu->PlateauPion[ligne][col][place].joueur = 0;
        jeu->plateau[ligne][col].nbpion--;
    }
}

int compter_pions_joueur(Jeu* jeu, int joueur, int ligne, int col) {
    int nb = 0;
    for (int p = 0; p < 10; p++)
        if (jeu->PlateauPion[ligne][col][p].TypePion == 1 && jeu->PlateauPion[ligne][col][p].joueur == joueur)
            nb++;
    return nb;
}

bool case_adjacente(int ligne1, int colonne1, int ligne2, int colonne2) {
    int dl = ligne1 - ligne2;
    int dc = colonne1 - colonne2;
    // Voisins 4-connexes (pas de diagonale)
    return (dl == 0 && (dc == 1 || dc == -1)) ||
        (dc == 0 && (dl == 1 || dl == -1));
}

// =============================================================
//  VOLCAN / ÉRUPTION
// =============================================================

int trouver_volcan_ligne(Jeu* jeu) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (jeu->plateau[i][j].TypeCase == 0) 
                return i;
    return -1;
}

int trouver_volcan_col(Jeu* jeu) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (jeu->plateau[i][j].TypeCase == 0) 
                return j;
    return -1;
}

void avancer_volcan(Jeu* jeu) {
    jeu->eruption_volcan++;
}

bool verifier_eruption(Jeu* jeu) {
    return jeu->eruption_volcan >= 6;
}

// Calcul des points : 1 pt par cromagnon sur une case non-volcan
void calculer_points(Jeu* jeu) {
    for (int i = 0; i < 4; i++)
        jeu->Joueurs[i].points = 0;

    for (int l = 0; l < 4; l++) {
        for (int c = 0; c < 4; c++) {
            if (jeu->plateau[l][c].TypeCase == 0) continue; // volcan = 0 pts
            for (int p = 0; p < 10; p++) {
                if (jeu->PlateauPion[l][c][p].TypePion == 1) {
                    int pj = jeu->PlateauPion[l][c][p].joueur;
                    jeu->Joueurs[pj].points++;
                }
            }
        }
    }
}

int joueur_gagnant(Jeu* jeu) {
    if (!verifier_eruption(jeu)){
        return -1;
    }
    calculer_points(jeu);
    int best = -1;
    int best_pts = -1;
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

    // Volcan obligatoire : traité immédiatement à l'init
    // (on avance le marqueur autant de fois qu'il y a de faces Volcan)
    for (int v = 0; v < ea->nb_actions[0]; v++)
        avancer_volcan(jeu);
    ea->nb_actions[0] = 0;

    ea->sous_etat = ACTION_MENU;
    ea->action_en_cours = -1;
    ea->orig_ligne = -1;
    ea->orig_col = -1;

    // Si rien à faire, passer directement à FINI
    if (ea->nb_actions[1] == 0 && ea->nb_actions[2] == 0 &&
        ea->nb_actions[3] == 0)
        ea->sous_etat = ACTION_FINI;
}

// Retourne true quand le joueur a fini toutes ses actions
bool traiter_action(Jeu* jeu, EtatAction* ea, int joueur, int clic_ligne, int clic_col, bool clic_valide) {
    if (ea->sous_etat == ACTION_FINI) return true;

    // MENU : choix avec flèches + ENTREE
    if (ea->sous_etat == ACTION_MENU) {
        int nb_options = 4; // Caverne, Hutte, Empreinte, Terminer

        if (IsKeyPressed(KEY_UP))
            ea->selection_menu = (ea->selection_menu - 1 + nb_options) % nb_options;
        if (IsKeyPressed(KEY_DOWN))
            ea->selection_menu = (ea->selection_menu + 1) % nb_options;

        if (IsKeyPressed(KEY_ENTER)) {
            int selection = ea->selection_menu;

            if (selection == 0 && ea->nb_actions[1] > 0) {   // Caverne
                ea->action_en_cours = 1;
                ea->sous_etat = ACTION_DEPLOYER_CASE;
            }
            else if (selection == 1 && ea->nb_actions[2] > 0) { // Hutte
                ea->action_en_cours = 2;
                ea->sous_etat = ACTION_DEPLOYER_CASE;
            }
            else if (selection == 2 && ea->nb_actions[3] > 0) { // Empreinte
                ea->action_en_cours = 3;
                ea->sous_etat = ACTION_DEPLACER_ORIGINE;
            }
            else if (selection == 3) {                          // Terminer
                ea->sous_etat = ACTION_FINI;
                return true;
            }
        }
        return false;
    }

    // ── DEPLOYER : clic sur une case du bon type ──
    if (ea->sous_etat == ACTION_DEPLOYER_CASE && clic_valide) {
        int type_cible = (ea->action_en_cours == 1) ? 5 : 4; // Caverne=5, Hutte=4
        if (jeu->plateau[clic_ligne][clic_col].TypeCase == type_cible &&
            jeu->Joueurs[joueur].reserve > 0) {

            jeu->Joueurs[joueur].reserve--;
            rajouter_pion(jeu, clic_ligne, clic_col, joueur);
            ea->nb_actions[ea->action_en_cours]--;
            ea->sous_etat = ACTION_MENU;
        }
        // (clic invalide ignoré silencieusement)
        return false;
    }

    // ── DEPLACER ORIGINE : clic sur un de ses cromagnons ──
    if (ea->sous_etat == ACTION_DEPLACER_ORIGINE && clic_valide) {
        if (compter_pions_joueur(jeu, joueur, clic_ligne, clic_col) > 0 && jeu->plateau[clic_ligne][clic_col].TypeCase != 0) { // pas le volcan
            ea->orig_ligne = clic_ligne;
            ea->orig_col = clic_col;
            ea->sous_etat = ACTION_DEPLACER_DEST;
        }
        return false;
    }

    // ── DEPLACER DESTINATION : clic sur une case adjacente ──
    if (ea->sous_etat == ACTION_DEPLACER_DEST && clic_valide) {
        // Annuler et rechoisir si on reclique la même case
        if (clic_ligne == ea->orig_ligne && clic_col == ea->orig_col) {
            ea->sous_etat = ACTION_DEPLACER_ORIGINE;
            return false;
        }

        if (case_adjacente(ea->orig_ligne, ea->orig_col, clic_ligne, clic_col)) {
            // Trouver la place du pion sur la case source
            for (int p = 0; p < 10; p++) {
                if (jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p].TypePion == 1 && jeu->PlateauPion[ea->orig_ligne][ea->orig_col][p].joueur == joueur) {
                    enlever_pion(jeu, ea->orig_ligne, ea->orig_col, p);
                    rajouter_pion(jeu, clic_ligne, clic_col, joueur);
                    ea->nb_actions[3]--;
                    ea->orig_ligne = -1;
                    ea->orig_col = -1;
                    ea->sous_etat = ACTION_MENU;
                    break;
                }
            }
        }
        return false;
    }

    // Vérifier si plus rien à faire
    if (ea->nb_actions[1] == 0 && ea->nb_actions[2] == 0 &&ea->nb_actions[3] == 0) {
        ea->sous_etat = ACTION_FINI;
        return true;
    }

    return false;
}