#pragma once

// ===================================================================================
// RÉSOLUTION VIRTUELLE DU JEU
// Changez ces deux valeurs pour définir combien de "pixels" votre jeu utilise.
// La taille de la police est calculée automatiquement pour remplir l'écran.
// ===================================================================================
#define CANVAS_LARGEUR  80   // nombre de pixels horizontaux
#define CANVAS_HAUTEUR  45   // nombre de pixels verticaux

// Initialisation
int  calculer_dezoom_auto(void);
void initialiser_console(void);

// Déplacement du curseur (coordonnées en "pixels" virtuels)
void bouger_curseur(int x, int y);

// Dessin de base
void dessine_pixel_hex(int x, int y, int hex_couleur);

// Formes
void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur);
void dessiner_cercle(int centre_x, int centre_y, int rayon, int hex_couleur);

// Image
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y);