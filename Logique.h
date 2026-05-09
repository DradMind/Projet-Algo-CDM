#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// =============================================================
//  STRUCTURES
// =============================================================

// Types de dinosaures (4 types, chacun avec un pouvoir)
typedef enum {
    DINO_AUCUN = 0,
    DINO_TRICERATOPS = 1,  // +1 lancer par tour (4 lancers au lieu de 3)
    DINO_PTERODACTYLE = 2,  // déplacement de 2 cases au lieu de 1
    DINO_BRACHIO = 3,   // peut déployer sur n'importe quelle case (pas seulement hutte/caverne)
    DINO_TITANOSAURE = 4,   // +1 dé supplémentaire
} TypeDino;

typedef struct {
    int       TypePion;   // 0=vide, 1=cromagnon, 2=dinosaure
    int       joueur;     // 0-3
    TypeDino  dino;       // si TypePion==2, quel dino
} Pion;

typedef struct {
    int TypeCase;   // 0=Volcan 1=Eau 2=Jungle 3=Prairie 4=Hutte 5=Caverne
    int nbpion;
    int distance;
} Case;

typedef struct {
    bool bloque;
    int  action;  // 0=Volcan 1=Caverne 2=Hutte 3=Empreinte 4=Oeuf 5=Deplacement
    bool selectionne;
} Des;

typedef struct {
    int      numerojoueur;
    int      reserve;       // cromagnons en réserve
    int      points;
    TypeDino dino_possede;  // dino actif du joueur (1 max pour l'instant)
    bool     a_dino;        // true si le joueur possède un dino sur le plateau
    int      dino_ligne;    // position du dino sur le plateau
    int      dino_col;
    int      oeufs;         // oeufs en attente d'éclosion
} Joueur;

typedef struct {
    Case   plateau[4][4];
    Pion   PlateauPion[4][4][10];
    Des    Listede[7];
    Joueur Joueurs[4];
    int    eruption_volcan;  // 0-6
    int    nb_joueurs;
    int    nb_eruptions;     // nombre total d'éruptions depuis le début
} Jeu;

// =============================================================
//  ÉTAT DU TOUR
// =============================================================

typedef enum {
    ACTION_MENU,
    ACTION_DEPLOYER_CASE,
    ACTION_DEPLACER_ORIGINE,
    ACTION_DEPLACER_DEST,
    ACTION_OEUF_CHOISIR,     // joueur choisit quel dino faire éclore
    ACTION_FINI
} SousEtatAction;

typedef struct {
    int            nb_actions[6];
    SousEtatAction sous_etat;
    int            action_en_cours;
    int            selection_menu;
    int            orig_ligne;
    int            orig_col;
    int            selection_dino;   // curseur dans le menu d'éclosion
} EtatAction;

// =============================================================
//  ÉRUPTION – état de la phase séisme
// =============================================================
typedef enum {
    ERUPTION_VOLCAN_SCORE,   // calcul des points volcan
    ERUPTION_REGION_SCORE,   // calcul des points régions
    ERUPTION_DEPLACEMENT,    // joueur contrôle le volcan aux flèches
    ERUPTION_RESET,          // nettoyage et remise à zéro
    ERUPTION_FINI
} EtatEruption;

typedef struct {
    EtatEruption phase;
    bool         points_calcules;
    int          moves_restants;     // déplacements de tuile autorisés (1 par éruption)
} PhaseEruption;

// =============================================================
//  FONCTIONS LOGIQUE
// =============================================================

void initialiser_plateau(Jeu* jeu, bool plateauBase, int nbJoueurs);
void logiquede(Jeu* jeu, int joueur);
int  possedetitanosaure(Jeu* jeu, int joueur);
int  nb_lancers_total(Jeu* jeu, int joueur);   // 3 (+1 si TRICERATOPS)
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage);

// Actions
void init_etat_action(Jeu* jeu, EtatAction* ea, int joueur);
bool traiter_action(Jeu* jeu, EtatAction* ea, int joueur, int clic_ligne, int clic_col, bool clic_valide);

// Pions
void rajouter_pion(Jeu* jeu, int ligne, int col, int joueur);
void rajouter_dino(Jeu* jeu, int ligne, int col, int joueur, TypeDino type);
void enlever_pion(Jeu* jeu, int ligne, int col, int place);
int  compter_pions_joueur(Jeu* jeu, int joueur, int ligne, int col);
int  compter_total_case(Jeu* jeu, int ligne, int col);
bool case_adjacente(int l1, int c1, int l2, int c2);

// Volcan / éruption
void avancer_volcan(Jeu* jeu);
bool verifier_volcan(Jeu* jeu);
void calculer_points(Jeu* jeu);
void calculer_points_eruption(Jeu* jeu);  // points majorité volcan + régions
void executer_eruption_reset(Jeu* jeu);   // vide le volcan, replace les pions
void init_phase_eruption(PhaseEruption* pe);
bool traiter_eruption(Jeu* jeu, PhaseEruption* pe); // true = éruption terminée

// Régions (flood-fill pour trouver régions connectées)
int  taille_region(Jeu* jeu, int ligne, int col, bool visite[4][4]);
int  joueur_majoritaire_region(Jeu* jeu, bool cellules[4][4]);

// Fin de partie
int  seuil_victoire(int nb_joueurs);
bool partie_terminee(Jeu* jeu);
int  joueur_gagnant(Jeu* jeu);

// Utilitaires
int  trouver_volcan_ligne(Jeu* jeu);
int  trouver_volcan_col(Jeu* jeu);