#pragma once

// Initialisation
void initialiser_console(int dezoom_count);

// Fonctions de dessin
void bouger_curseur(int x, int y);
void dessine_pixel_hex(int x, int y, int hex_couleur);
void dessiner_rectangle(int x, int y, int longueur, int hauteur, int hex_couleur);
void afficher_texte(const char* texte, int x, int y, int hex_texte, int hex_fond);


// Image
void dessiner_image_ppm(const char* nom_fichier, int start_x, int start_y);

//affichage
void afficherplateau(int posx, int posy);