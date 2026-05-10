#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// =============================================================
//  STRUCTURES
// =============================================================

// Types de dinosaures (8 types officiels)
typedef enum {
    DINO_AUCUN = 0,
    DINO_TRICERATOPS = 1,
    DINO_PTERANODON = 2,
    DINO_BRONTOSAURE = 3,
    DINO_TITANOSAURE = 4,
    DINO_STEGOSAURE = 5,
    DINO_ANKYLOSAURE = 6,
    DINO_PLESIOSAURE = 7,
    DINO_TYRANNOSAURE = 8
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
    int  action;  // 0=Volcan 1=Caverne 2=Hutte 3=Deplacement 4=Oeuf 5=Deplacement
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
    TypeDino dinos_en_jeu[4];    // Les 4 dinosaures choisis pour la partie
    int    dinos_disponibles[8]; // Stock par type de dino
    int    eruption_volcan;      // obsolète si on utilise le nombre de pions
    int    nb_joueurs;
    int    nb_eruptions;         // nombre total d'éruptions depuis le début
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
    ACTION_CHOISIR_PION,     // joueur choisit quel pion déplacer s'il y a un dino et un cromagnon
    ACTION_VOLCAN_MENU,      // joueur choisit quel pion sacrifier au volcan
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
    int            type_pion_selectionne; // 1 = Cromagnon, 2 = Dino, 0 = Non défini
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
    bool         est_decalage_seul;
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
bool a_dino_sacrifiable(Jeu* jeu, int joueur);

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
void init_phase_decalage(PhaseEruption* pe);
bool joueur_a_majorite_volcan(Jeu* jeu, int joueur);