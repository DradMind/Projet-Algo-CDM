#pragma once
#include "raylib.h"
#include <stdbool.h>
#include "Logique.h"

// =============================================================
//  PE  –  Pourcentage d'une valeur
// =============================================================
int PE(float pourcentage, float valeur);

// =============================================================
//  LAYOUT
// =============================================================

// Topbar
#define TOPBAR_H        PE(5,  GetScreenHeight())

// Plateau
#define PLATEAU_X       PE(3,  GetScreenWidth())
#define PLATEAU_Y       (TOPBAR_H + PE(3, GetScreenHeight()))
#define CASE            PE(15, GetScreenHeight())
#define CASE_GAP        PE(1,  GetScreenHeight())

// Panneau dés (droite)
#define PANEL_X         PE(72, GetScreenWidth())
#define PANEL_W         (GetScreenWidth() - PANEL_X)
#define TABLEAU_X       (PANEL_X + PE(1, GetScreenWidth()))
#define TABLEAU_Y       (TOPBAR_H + PE(5, GetScreenHeight()))
#define DIE_H           PE(7,  GetScreenHeight())

// Bande d'info
#define INFO_Y          (PLATEAU_Y + 4 * (CASE + CASE_GAP) + PE(2, GetScreenHeight()))
#define INFO_H          PE(10, GetScreenHeight())
#define INFO_CARD_W     PE(20, GetScreenWidth())

// Status bar
#define STATUS_Y        PE(88, GetScreenHeight())

// =============================================================
//  Palette
// =============================================================
#define COL_BG          (Color){  8,   5,   2, 255}
#define COL_BG_DARK     (Color){ 17,  13,   7, 255}
#define COL_BG_MID      (Color){ 25,  18,  10, 255}
#define COL_BORDER      (Color){ 61,  36,  16, 255}
#define COL_BORDER_LIT  (Color){240, 104,   5, 255}
#define COL_ACCENT      (Color){240, 104,   5, 255}
#define COL_ACCENT_DIM  (Color){140,  60,   5, 255}
#define COL_TEXT        (Color){212, 184, 150, 255}
#define COL_TEXT_DIM    (Color){122,  96,  64, 255}
#define COL_TEXT_BRIGHT (Color){240, 210, 160, 255}
#define COL_VALID       (Color){ 42, 180,  42, 255}
#define COL_INVALID     (Color){180,  42,  42, 255}

// Cases
static const Color COULEUR_CASE[] = {
    {  90,  21,   0, 255},
    {   8,  32,  64, 255},
    {  14,  45,  14, 255},
    {  26,  58,   8, 255},
    {  45,  26,   6, 255},
    {  28,  28,  28, 255},
};
static const Color COULEUR_CASE_BORDER[] = {
    { 255,  80,  16, 255},
    {  30, 100, 200, 255},
    {  34, 139,  34, 255},
    { 100, 200,   0, 255},
    { 139,  69,  19, 255},
    { 120, 120, 120, 255},
};
static const Color COULEUR_CASE_LABEL[] = {
    { 255, 128,  96, 255},
    {  96, 168, 255, 255},
    { 112, 216,  96, 255},
    { 168, 232,  64, 255},
    { 208, 144,  96, 255},
    { 176, 176, 176, 255},
};
static const char* NOM_CASE[] = {
    "VOLCAN","EAU","JUNGLE","PRAIRIE","HUTTE","CAVERNE"
};

// Dés
static const Color COULEUR_DE[] = {
    { 255,  69,   0, 255},
    { 169, 169, 169, 255},
    { 139,  69,  19, 255},
    { 255, 255,   0, 255},
    { 255, 105, 180, 255},
    {  30, 144, 255, 255},
};
static const char* NOM_FACE[] = {
    "Volcan","Caverne","Hutte","Empreinte","Oeuf","Deplacement"
};

// Joueurs
static const Color COULEUR_JOUEUR[] = {
    { 192,  57,  43, 255},
    {  41, 128, 185, 255},
    {  39, 174,  96, 255},
    { 142,  68, 173, 255},
};

// =============================================================
//  Fonctions
// =============================================================
void initialiser_fenetre(void);

// Rendu complet selon l'étape
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides);
void affichage_actions(Jeu* jeu, int joueur, EtatAction* ea);
void affichage_fin(Jeu* jeu, int gagnant);

// Sous-fonctions
void dessiner_topbar(Jeu* jeu, int joueur);
void afficherplateau(Jeu* jeu, EtatAction* ea, int joueur);
void dessiner_panel_des(Jeu* jeu, int joueur);
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer);
void dessiner_statusbar(int etape, bool achoisides);
void dessiner_menu_actions(EtatAction* ea);
void dessiner_marqueur_volcan(Jeu* jeu);

// Utilitaire : convertit un clic souris en ligne/col du plateau (-1 si hors plateau)
bool clic_sur_plateau(Vector2 souris, int* ligne, int* col);

// Sélection dés
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage);