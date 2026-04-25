#include "Graphiques.h"
#include "Logique.h"

void initialiser_fenetre(void) {
    InitWindow(1, 1, "");

    int ecran = GetCurrentMonitor();
    int Largeur_ecran = GetMonitorWidth(ecran);
    int Hauteur_ecran = GetMonitorHeight(ecran);

    int Largeur_voulu = 1280;
    int Hauteur_voulu = 720;

    if (Largeur_ecran > 1920 || Hauteur_ecran > 1080) { // pour calibrer la bonne résolution entre moi et toi (et le prof)
        Largeur_voulu = 2560;
        Hauteur_voulu = 1440;
    }

    // 4. On redimensionne et on affiche la fenêtre
    SetWindowSize(Largeur_voulu, Hauteur_voulu);
    SetWindowTitle("Colere de la Montagne de Feu");
	//SetWindowState(FLAG_WINDOW_UNDECORATED); // Enlève les bordures de la fenêtre (à voir)
    SetTargetFPS(60);
}

void affichage_jeu(Jeu* jeu, int joueur) {
    ClearBackground((Color) { 8, 5, 2, 255 }); // Couleur du fond du jeu(

	DrawRectangle(0, PE(5, GetScreenHeight()), PE(70, GetScreenWidth()), PE(70, GetScreenHeight()), (Color) { 17, 13, 7, 255 }); // Fond espace plateau
	DrawRectangleLines(0, PE(5, GetScreenHeight()), PE(70, GetScreenWidth()), PE(70, GetScreenHeight()), (Color) { 240, 104, 5, 255 }); // Bordure espace plateau

	DrawRectangleRounded((Rectangle) { TABLEAU_X - PE(1, GetScreenWidth()), TABLEAU_Y - PE(1, GetScreenWidth()), DIE_H + PE(5,GetScreenWidth()), DIE_H * (5 + possedetitanosaure(jeu, joueur)) + PE(5,GetScreenHeight()) }, 0.05f, 2, (Color) { 8, 5, 2, 255 }); // Fond et bordure espace latéral
	DrawRectangleRoundedLines((Rectangle) { TABLEAU_X - PE(1, GetScreenWidth()), TABLEAU_Y - PE(1, GetScreenWidth()), DIE_H + PE(5, GetScreenWidth()), DIE_H* (5 + possedetitanosaure(jeu, joueur)) + PE(5, GetScreenHeight())}, 0.05f, 2, (Color) { 240, 104, 5, 255 });
    
	DrawRectangle(0, 0, GetScreenWidth(), PE(5, GetScreenHeight()), (Color) { 17, 13, 7, 255 }); // Fond espace Titre/Joueurs
	DrawRectangleLines(0, 0, GetScreenWidth(), PE(5, GetScreenHeight()), (Color) { 240, 104, 5, 255 }); // Bordure espace Titre/Joueurs
	DrawText("Colère de la Montagne de Feu", PE(1, GetScreenWidth()), PE(1, GetScreenHeight()), PE(3, GetScreenHeight()), (Color) { 240, 104, 5, 255 }); //Titre du jeu



}

// -------------------------------------------------------------
//  Plateau 4×4
// -------------------------------------------------------------
void afficherplateau(Jeu* jeu) {
    // Le fond marron du plateau (un peu plus grand que les cases)
    int bordure = PE(1, GetScreenWidth()); // 1% d'épaisseur
	DrawRectangleRoundedLines((Rectangle) { PLATEAU_X - bordure, PLATEAU_Y - bordure, (4 * (CASE + PE(1, GetScreenHeight()))) + (2 * bordure), (4 * (CASE + PE(1, GetScreenHeight()))) + (2 * bordure) }, 0.05f, 2, (Color) { 60, 50, 35, 255 });
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            int type = jeu->plateau[y][x].TypeCase;
            Color couleur = (type >= 0 && type <= 5) ? COULEUR_CASE[type] : BLACK;

            // Calcul la position de chaque case
            Rectangle Case = {PLATEAU_X + (x * (CASE + PE(1,GetScreenHeight()))), PLATEAU_Y + (y * (CASE + PE(1,GetScreenHeight()))), CASE, CASE};

            //DrawRectangleRec(Case, couleur);
			DrawRectangleRounded(Case, 0.1f, 10, couleur); // Bordure noire
			//DrawRectangleRoundedLines(Case, 0.1f, 10, (Color) { 0, 0, 0, 255 });
        }
    }
}

// -------------------------------------------------------------
//  Dés dans le panneau latéral
// -------------------------------------------------------------
void afficher_de(Jeu* jeu, int joueur) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = TABLEAU_X + 10;
        int y = TABLEAU_Y + 10 + i * (DIE_H + 6);
		Rectangle rect = { x, y, DIE_H, DIE_H }; //on créer un rectangle pour chaque dé

        // Change la couleur si le dé est sélectionné
        Color dieColor = jeu->Listede[i].selectionne ? GREEN : COULEUR_DE[jeu->Listede[i].action];
		DrawRectangleRounded(rect, 0.1f, 10, dieColor); // Fond du dé
		DrawRectangleRoundedLines(rect, 0.1f, 10, BLACK); // Bordure noire

        if (jeu->Listede[i].bloque) {
            DrawRectangleRec(rect, (Color) { 255, 0, 0, 255 }); // Rouge pour les dés bloqués
            DrawRectangleLinesEx(rect, 2, BLACK);
        }
    }
}

bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = TABLEAU_X + 10;
        int y = TABLEAU_Y + 10 + i * (DIE_H + 6);
        Rectangle rect = { (float)x, (float)y, (float)DIE_H, (float)DIE_H };

        // Si la souris est sur le dé ET que l'utilisateur vient de faire un clic gauche
        if (CheckCollisionPointRec(pos_souris, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (blocage) {
                jeu->Listede[i].bloque = !jeu->Listede[i].bloque;
                return true;
            }
            else {
                jeu->Listede[i].selectionne = !jeu->Listede[i].selectionne; // Toggle sélection
                return true;
            }
        }
    }
    return false; // TRÈS IMPORTANT : renvoyer false si rien n'est cliqué !
}

int PE(float pourcentage, float valeur) { //sa sert surtout pour des pourcentages de l'écran, pour des écrans de taille différents
    int x = valeur / 100 * pourcentage;
    return x;
}