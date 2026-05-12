#pragma once
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// =============================================================
//  STRUCTURES
// =============================================================

// Types de dinosaures 
// Chaque dinosaure a un pouvoir unique qui s'active à un moment précis du jeu :
// - TRICERATOPS : +1 à la valeur des groupes lors du calcul de majorité
// - PTERANODON : Peut voler vers n'importe quelle case Jungle ou Prairie
// - BRONTOSAURE : Compte pour 2 pions lors du calcul de majorité
// - TITANOSAURE : Donne 1 dé supplémentaire par tour
// - STEGOSAURE : Permet de déclencher des séismes même sans majorité s'il est sur le Volcan
// - ANKYLOSAURE : Donne +2 points bonus en cas de victoire stricte d'une région
// - PLESIOSAURE : Permet d'aller sur l'Eau et fait marquer des points aux Lagons
// - TYRANNOSAURE : "Mange" (renvoie en réserve) un Cro-Magnon ennemi lors de son arrivée
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

// Représentation d'un Joueur
typedef struct {
    int      numerojoueur;          // Identifiant du joueur (0 à 3)
    int      reserve;               // Nombre de Cro-Magnons disponibles dans la réserve du joueur
    int      points;                // Score actuel du joueur
    TypeDino dino_possede;          // Le type de dinosaure actuellement actif du joueur
    bool     a_dino;                // true si le joueur a fait éclore un dinosaure
    int      dino_ligne;            // Position Y du dinosaure sur le plateau (si posé)
    int      dino_col;              // Position X du dinosaure sur le plateau (si posé)
    int      oeufs;                 // Nombre d'actions "Oeuf" accumulées, prêtes à être dépensées
} Joueur;

// Structure principale de l'état du jeu
typedef struct {
    Case   plateau[4][4];                // Grille 4x4 des tuiles du plateau
    Pion   PlateauPion[4][4][10];        // Pile de pions (jusqu'à 10) sur chaque tuile
    Des    Listede[7];                   // Les dés d'actions (5 par défaut + 1 pour Titanosaure)
    Joueur Joueurs[4];                   // Les statistiques des 4 joueurs
    TypeDino dinos_en_jeu[4];            // Les 4 dinosaures tirés au sort pour cette partie
    int    dinos_disponibles[8];         // Quantité restante dans la boîte (2 par espèce choisie, 0 sinon)
    int    eruption_volcan;              // Compteur obsolète (désormais basé sur le nombre de pions sur le volcan)
    int    nb_joueurs;                   // Nombre de joueurs actifs dans la partie
    int    nb_eruptions;                 // Nombre total d'éruptions ayant eu lieu (déclenche la fin du jeu)
} Jeu;

// =============================================================
//  ÉTAT DU TOUR
// =============================================================

typedef enum {
    ACTION_MENU,
    ACTION_DEPLOYER_CASE,
    ACTION_DEPLACER_ORIGINE,
    ACTION_DEPLACER_DEST,
    ACTION_OEUF_CHOISIR,     // le joueur choisit quel dino faire éclore
    ACTION_CHOISIR_PION,     // le joueur choisit quel pion déplacer s'il y a un dino et un cromagnon
    ACTION_VOLCAN_MENU,      // le joueur choisit quel pion sacrifier au volcan
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
int  nb_lancers_total(Jeu* jeu, int joueur);   // 3 (+1 si titanausore)
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

// Régions (flood-fill pour trouver LES régions connectées)
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