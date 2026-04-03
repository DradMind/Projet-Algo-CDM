#pragma once
#include <stdbool.h>

//188x116 <--- c'est la résolution de la console en mode plein écran avec un dézoom de 8 (ajustable dans main.c)

// Initialisation
void initialiser_console(int dezoom_count);

// Fonctions de dessin
void bouger_curseur(int x, int y);
void dessine_pixel_hex(int x, int y, int hex_couleur);
void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur);
void afficher_texte(const char* texte, int x, int y, int hex_texte, int hex_fond);
void afficher_texte_pixel(const char* texte, int x, int y, int hex_couleur);

// Image
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y);

//affichage
void afficherplateau(int posx, int posy);
void afficher_arrière_plan(void);
void afficher_de(int joueur);