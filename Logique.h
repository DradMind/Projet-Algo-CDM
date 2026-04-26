#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// =============================================================
//  STRUCTURES
// =============================================================

typedef struct {
    int  TypePion;      // 0=vide, 1=cromagnon, on fera les dinos plus tard
    int  joueur;        // 0-3
} Pion;

typedef struct {
    int TypeCase;       // 0=Volcan 1=Eau 2=Jungle 3=Prairie 4=Hutte 5=Caverne
    int nbpion;
    int distance;       // distance au volcan (pour déplacements futurs)
} Case;

typedef struct {
    bool bloque;
    int  action;        // 0=Volcan 1=Caverne 2=Hutte 3=Empreinte 4=Oeuf 5=Deplacement
    bool selectionne;
} Des;

typedef struct {
    int numerojoueur;
    int reserve;        // cromagnons en réserve
    int points;
} Joueur;

typedef struct {
    Case   plateau[4][4];
    Pion   PlateauPion[4][4][10];
    Des    Listede[7];
    Joueur Joueurs[4];
    int    eruption_volcan; // 0-6 : avance à chaque face Volcan, éruption à 6
    int    nb_joueurs;
} Jeu;



// =============================================================
//  ÉTAT DU TOUR  –  machine à états pour les actions
// =============================================================

// Sous-états de l'étape 1 (actions des dés)
typedef enum {
    ACTION_MENU,            // joueur choisit quelle action faire
    ACTION_DEPLOYER_CASE,   // joueur clique une case Hutte ou Caverne
    ACTION_DEPLACER_ORIGINE,// joueur clique le cromagnon à déplacer
    ACTION_DEPLACER_DEST,   // joueur clique la case destination
    ACTION_FINI             // plus aucune action possible
} SousEtatAction;

typedef struct {
    int             nb_actions[6]; // comptage des faces restantes
    SousEtatAction  sous_etat;
    int             action_en_cours; // quelle face on est en train de résoudre
    int             selection_menu;  // curseur dans le menu d'actions
    int             orig_ligne;      // pour déplacement : case source
    int             orig_col;
} EtatAction;

// =============================================================
//  FONCTIONS LOGIQUE
// =============================================================

void initialiser_plateau(Jeu* jeu, bool plateauBase, int nbJoueurs);
void logiquede(Jeu* jeu, int joueur);
int  possedetitanosaure(Jeu* jeu, int joueur); // retourne 0 pour l'instant
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage);

// Actions
void init_etat_action(Jeu* jeu, EtatAction* ea, int joueur);
bool traiter_action(Jeu* jeu, EtatAction* ea, int joueur, int case_cliquee_ligne, int case_cliquee_col, bool clic_valide);
// Retourne true quand toutes les actions sont résolues

// Pions
void rajouter_pion(Jeu* jeu, int ligne, int col, int joueur);
void enlever_pion(Jeu* jeu, int ligne, int col, int place);
int  compter_pions_joueur(Jeu* jeu, int joueur, int ligne, int col);
bool case_adjacente(int l1, int c1, int l2, int c2); // voisins directs (4-connexité)

// Volcan / éruption
void avancer_volcan(Jeu* jeu);
bool verifier_eruption(Jeu* jeu); // true si marqueur >= 6
void calculer_points(Jeu* jeu);   // compte les pions sur chaque case → points

// Le reste
int  trouver_volcan_ligne(Jeu* jeu);
int  trouver_volcan_col(Jeu* jeu);
int  joueur_gagnant(Jeu* jeu);    // -1 si pas fini
