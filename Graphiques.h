#pragma once
#include "raylib.h"
#include <stdbool.h>
#include "Logique.h"

#define PLATEAU_X      PE(9, GetScreenWidth())       // coin haut-gauche du plateau
#define PLATEAU_Y      PE(6, GetScreenHeight())
#define CASE      PE(10, GetScreenWidth())       // taille d'une case en pixels

#define PANEL_X      440       // panneau latéral (dés)
#define PANEL_Y      0
#define PANEL_W      200
#define DIE_H        52

// =============================================================
//  Couleurs des types de cases
// =============================================================
static const Color COULEUR_CASE[] = {
    { 255,  69,   0, 255 }, // 0 Volcan
    {  30, 144, 255, 255 }, // 1 Eau
    {  34, 139,  34, 255 }, // 2 Jungle
    { 124, 252,   0, 255 }, // 3 Prairie
    { 139,  69,  19, 255 }, // 4 Hutte
    { 169, 169, 169, 255 }, // 5 Caverne
};

// Couleurs des faces de dé (0=Volcan 1=Caverne 2=Hutte 3=Empreinte 4=Oeuf 5=Déplacement)
static const Color COULEUR_DE[] = {
    { 255,  69,   0, 255 }, // Volcan
    { 169, 169, 169, 255 }, // Caverne
    { 139,  69,  19, 255 }, // Hutte
    { 255, 255,   0, 255 }, // Empreinte
    { 255, 105, 180, 255 }, // Oeuf
    {  30, 144, 255, 255 }, // Déplacement
};

static const char* NOM_FACE[] = {
    "Volcan", "Caverne", "Hutte", "Empreinte", "Oeuf", "Deplacement"
};

// =============================================================
//  Fonctions
// =============================================================
void afficherplateau(Jeu* jeu);
void afficher_de(Jeu* jeu, int joueur);
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris);
int PE(float pourcentage, float valeur);