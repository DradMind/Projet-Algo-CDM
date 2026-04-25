#pragma once
#include "raylib.h"
#include <stdbool.h>
#include "Logique.h"

// =============================================================
//  PE  –  Pourcentage d'une valeur (ex: PE(5, GetScreenWidth()))
// =============================================================
int PE(float pourcentage, float valeur);

// =============================================================
//  LAYOUT (tout en %)
//
//  ┌──────────────────────────────────────────────────┐
//  │               TOPBAR  (5% hauteur)               │
//  ├───────────────────────────┬──────────────────────┤
//  │                           │                      │
//  │       PLATEAU 4×4         │   PANNEAU DÉS        │
//  │    (cases + pions)        │  (dés + scores)      │
//  │                           │                      │
//  ├──────────┬────────────────┴──────────────────────┤
//  │INFO STRIP│         (bas gauche, 15% hauteur)      │
//  ├──────────┴───────────────────────────────────────┤
//  │               STATUSBAR  (7% hauteur)            │
//  └──────────────────────────────────────────────────┘
// =============================================================

// Topbar
#define TOPBAR_H        PE(5,  GetScreenHeight())

// Plateau
#define PLATEAU_X       PE(3,  GetScreenWidth())
#define PLATEAU_Y       (TOPBAR_H + PE(3, GetScreenHeight()))
#define CASE            PE(15, GetScreenHeight())   // taille d'une case
#define CASE_GAP        PE(1,  GetScreenHeight())   // espace entre cases

// Panneau dés (droite)
#define PANEL_X         PE(72, GetScreenWidth())
#define PANEL_W         (GetScreenWidth() - PANEL_X)

// Dés
#define TABLEAU_X       (PANEL_X + PE(1, GetScreenWidth()))
#define TABLEAU_Y       (TOPBAR_H + PE(5, GetScreenHeight()))
#define DIE_H           PE(7,  GetScreenHeight())

// Bande d'info (bas, sous le plateau)
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

// Cases
static const Color COULEUR_CASE[] = {
    {  90,  21,   0, 255},  // 0 Volcan
    {   8,  32,  64, 255},  // 1 Eau
    {  14,  45,  14, 255},  // 2 Jungle
    {  26,  58,   8, 255},  // 3 Prairie
    {  45,  26,   6, 255},  // 4 Hutte
    {  28,  28,  28, 255},  // 5 Caverne
};
static const Color COULEUR_CASE_BORDER[] = {
    { 255,  80,  16, 255},  // Volcan
    {  30, 100, 200, 255},  // Eau
    {  34, 139,  34, 255},  // Jungle
    { 100, 200,   0, 255},  // Prairie
    { 139,  69,  19, 255},  // Hutte
    { 120, 120, 120, 255},  // Caverne
};
static const Color COULEUR_CASE_LABEL[] = {
    { 255, 128,  96, 255},  // Volcan
    {  96, 168, 255, 255},  // Eau
    { 112, 216,  96, 255},  // Jungle
    { 168, 232,  64, 255},  // Prairie
    { 208, 144,  96, 255},  // Hutte
    { 176, 176, 176, 255},  // Caverne
};
static const char* NOM_CASE[] = {
    "VOLCAN","EAU","JUNGLE","PRAIRIE","HUTTE","CAVERNE"
};

// Dés
static const Color COULEUR_DE[] = {
    { 255,  69,   0, 255},  // 0 Volcan
    { 169, 169, 169, 255},  // 1 Caverne
    { 139,  69,  19, 255},  // 2 Hutte
    { 255, 255,   0, 255},  // 3 Empreinte
    { 255, 105, 180, 255},  // 4 Oeuf
    {  30, 144, 255, 255},  // 5 Deplacement
};
static const char* NOM_FACE[] = {
    "Volcan","Caverne","Hutte","Empreinte","Oeuf","Deplacement"
};

// Joueurs
static const Color COULEUR_JOUEUR[] = {
    { 192,  57,  43, 255},  // J1 rouge
    {  41, 128, 185, 255},  // J2 bleu
    {  39, 174,  96, 255},  // J3 vert
    { 142,  68, 173, 255},  // J4 violet
};

// =============================================================
//  Fonctions
// =============================================================
void initialiser_fenetre(void);

// Rendu complet (appel unique depuis main, entre BeginDrawing/EndDrawing)
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides);

// Sous-fonctions (disponibles séparément si besoin)
void dessiner_topbar(Jeu* jeu, int joueur);
void afficherplateau(Jeu* jeu);
void dessiner_panel_des(Jeu* jeu, int joueur);
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer);
void dessiner_statusbar(int etape, bool achoisides);

// Sélection dés à la souris (depuis main)
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage);