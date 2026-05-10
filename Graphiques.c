#include "Graphiques.h"
#include "Logique.h"
#include <stdio.h>

// =============================================================
//  PE — pourcentage d'une valeur
// =============================================================
int PE(float pourcentage, float valeur) {
    return (int)(valeur / 100.0f * pourcentage);
}

// =============================================================
//  Utilitaires internes
// =============================================================
static void carte(int x, int y, int largeur, int hauteur, Color fond, Color bordure) {
    DrawRectangleRounded((Rectangle) { x, y, largeur, hauteur }, 0.08f, 6, fond);
    DrawRectangleRoundedLines((Rectangle) { x, y, largeur, hauteur }, 0.08f, 6, bordure);
}

static void texte_centre(const char* txt, int x, int y, int largeur_zone, int taille, Color couleur) {
    int largeur_texte = MeasureText(txt, taille);
    DrawText(txt, x + (largeur_zone - largeur_texte) / 2, y, taille, couleur);
}

static void pion(int centre_x, int centre_y, int rayon, Color couleur) {
    DrawCircle(centre_x, centre_y, (float)rayon, couleur);
    DrawCircleLines((float)centre_x, (float)centre_y, (float)(rayon + 1), (Color) { 0, 0, 0, 180 });
}

// =============================================================
//  Clic plateau
// =============================================================
bool clic_sur_plateau(Vector2 souris, int* ligne, int* col) {
    for (int l = 0; l < 4; l++) {
        for (int c = 0; c < 4; c++) {
            Rectangle rect = { (float)(PLATEAU_X + c * (CASE + CASE_ESPACE)), (float)(PLATEAU_Y + l * (CASE + CASE_ESPACE)), (float)CASE, (float)CASE };
            if (CheckCollisionPointRec(souris, rect)) {
                *ligne = l; *col = c;
                return true;
            }
        }
    }
    return false;
}

// =============================================================
//  Haut De Page
// =============================================================
void Dessiner_HDP(Jeu* jeu, int joueur) {
    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();
    int hauteur_bandeau = HDP_H;

    DrawRectangle(0, 0, largeur_ecran, hauteur_bandeau, FOND_SOMBRE);
    DrawLine(0, hauteur_bandeau, largeur_ecran, hauteur_bandeau, COL_BORD_LIT);
    DrawText("COLERE DE LA MONTAGNE DE FEU", PE(2, largeur_ecran), PE(1, hauteur_ecran), PE(3, hauteur_ecran), COL_ACCENT);

    int rayon = PE(2, hauteur_ecran);
    int cercle_x_dep = PE(80, largeur_ecran);
    int cercle_y = hauteur_bandeau / 2;

    for (int i = 0; i < jeu->nb_joueurs; i++) {
        Color couleur = COULEUR_JOUEUR[i];
        if (i != joueur) couleur.a = 70;
        int cx = cercle_x_dep + i * (rayon * 2 + PE(1, largeur_ecran));

        DrawCircle(cx, cercle_y, (float)rayon, couleur);
        if (i == joueur)
            DrawCircleLines((float)cx, (float)cercle_y, (float)(rayon + 2), COL_TEXT_BRIGHT);

        char label[3];
        snprintf(label, sizeof(label), "J%d", i + 1);
        int largeur_label = MeasureText(label, PE(2, hauteur_ecran));
        DrawText(label, cx - largeur_label / 2, cercle_y - PE(1, hauteur_ecran), PE(2, hauteur_ecran), (Color) { 255, 255, 255, i == joueur ? 220 : 100 });

        // Indicateur dino
        if (jeu->Joueurs[i].a_dino) {
            DrawCircle(cx + rayon, cercle_y - rayon,
                (float)(rayon / 2), (Color) { 80, 220, 80, 200 });
        }
    }
}

// =============================================================
//  PLATEAU
// =============================================================
void afficherplateau(Jeu* jeu, EtatAction* ea, int joueur) {
    int origine_x = PLATEAU_X;
    int origine_y = PLATEAU_Y;
    int taille_case = CASE;
    int espace = CASE_ESPACE;
    int taille_total = 4 * taille_case + 3 * espace;

    DrawRectangleRounded((Rectangle) { origine_x - 8, origine_y - 8, taille_total + 16, taille_total + 16 }, 0.04f, 4, (Color) { 12, 8, 3, 255 });
    DrawRectangleRoundedLines((Rectangle) { origine_x - 8, origine_y - 8, taille_total + 16, taille_total + 16 }, 0.04f, 4, COL_BORD);

    for (int ligne = 0; ligne < 4; ligne++) {
        for (int col = 0; col < 4; col++) {
            int type = jeu->plateau[ligne][col].TypeCase;
            int case_x = origine_x + col * (taille_case + espace);
            int case_y = origine_y + ligne * (taille_case + espace);

            Color couleur_fond = (type >= 0 && type < 6) ? COULEUR_CASE[type] : FOND_MID;
            Color couleur_bordure = (type >= 0 && type < 6) ? COULEUR_CASE_BORD[type] : COL_BORD;
            Color couleur_label = (type >= 0 && type < 6) ? COULEUR_CASE_LABEL[type] : COL_TEXT_DIM;

            // Halo orange autour du volcan
            if (type == 0)
                DrawRectangleRounded( (Rectangle) { case_x - 3, case_y - 3, taille_case + 6, taille_case + 6 }, 0.1f, 4, (Color) { 255, 60, 0, 40 });

            // Surlignage selon état du tour
            if (ea != NULL) {
                bool surligne_valide = false;
                bool surligne_origine = false;

                if (ea->sous_etat == ACTION_DEPLOYER_CASE) {
                    int type_cible = (ea->action_en_cours == 1) ? 4 : 5;
                    bool case_ok = (type == type_cible);
                    if (case_ok && jeu->Joueurs[joueur].reserve > 0) surligne_valide = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_ORIGINE) {
                    if (compter_pions_joueur(jeu, joueur, ligne, col) > 0)
                        surligne_valide = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_DEST) {
                    if (ligne == ea->orig_ligne && col == ea->orig_col)
                        surligne_origine = true;
                    else {
                        bool ptero = (jeu->Joueurs[joueur].a_dino && jeu->Joueurs[joueur].dino_possede == DINO_PTERANODON);
                        bool adj = case_adjacente(ea->orig_ligne, ea->orig_col, ligne, col);
                        bool dist2 = ptero && (abs(ea->orig_ligne - ligne) + abs(ea->orig_col - col) <= 2);
                        if (adj || dist2) surligne_valide = true;
                    }
                }

                if (surligne_valide)
                    DrawRectangleRounded( (Rectangle) { case_x - 4, case_y - 4, taille_case + 8, taille_case + 8 },  0.1f, 4, (Color) { 42, 180, 42, 60 });
                if (surligne_origine)
                    DrawRectangleRounded( (Rectangle) { case_x - 4, case_y - 4, taille_case + 8, taille_case + 8 }, 0.1f, 4, (Color) { 240, 140, 0, 80 });
                if (surligne_valide)  couleur_bordure = COL_VALIDE;
                if (surligne_origine) couleur_bordure = COL_ACCENT;
            }

            DrawRectangleRounded((Rectangle) { case_x, case_y, taille_case, taille_case }, 0.1f, 8, couleur_fond);
            DrawRectangleRoundedLines((Rectangle) { case_x, case_y, taille_case, taille_case }, 0.1f, 8, couleur_bordure);

            int taille_label = PE(1, GetScreenHeight());
            texte_centre(NOM_CASE[type], case_x, case_y + taille_case - taille_label - PE(12, GetScreenHeight()), taille_case, taille_label, couleur_label);

            // Pions + dinos
            int nb_pions = 0;
            for (int p = 0; p < 10; p++)
                if (jeu->PlateauPion[ligne][col][p].TypePion != 0) nb_pions++;

            // Indicateur Maître du Volcan
            if (type == 0) {
                int maitre = -1;
                for (int m = 0; m < 4; m++) {
                    if (joueur_a_majorite_volcan(jeu, m)) {
                        maitre = m;
                        break;
                    }
                }
                if (maitre != -1) {
                    char m_buf[32];
                    snprintf(m_buf, sizeof(m_buf), "MAITRE: J%d", maitre + 1);
                    texte_centre(m_buf, case_x, case_y + PE(2, GetScreenHeight()), taille_case, taille_label + 2, COULEUR_JOUEUR[maitre]);
                }
            }

            int rayon_pion = taille_case / 9;
            int espacement = rayon_pion * 2 + 2;
            int debut_x = case_x + (taille_case - nb_pions * espacement + 2) / 2;
            int dessines = 0;

            for (int p = 0; p < 10 && dessines < nb_pions; p++) {
                if (jeu->PlateauPion[ligne][col][p].TypePion == 0) continue;

                int joueur_pion = jeu->PlateauPion[ligne][col][p].joueur;
                Color couleur_pion = (joueur_pion >= 0 && joueur_pion < 4) ? COULEUR_JOUEUR[joueur_pion] : COL_TEXT;

                if (jeu->PlateauPion[ligne][col][p].TypePion == 2) {
                    // Dino : carré avec texte
                    TypeDino d = jeu->PlateauPion[ligne][col][p].dino;
                    const char* d_nom[] = { "", "Tr", "Pt", "Br", "Ti", "St", "An", "Pl", "Ty" };
                    int dx = debut_x + dessines * espacement;
                    int dy = case_y + taille_case - rayon_pion * 2 - PE(1, GetScreenHeight());
                    DrawRectangle(dx, dy, rayon_pion * 2, rayon_pion * 2, couleur_pion);
                    DrawRectangleLines(dx, dy, rayon_pion * 2, rayon_pion * 2, (Color) { 80, 220, 80, 255 });
                    
                    int font_size = rayon_pion * 1.2;
                    int text_w = MeasureText(d_nom[d], font_size);
                    DrawText(d_nom[d], dx + (rayon_pion * 2 - text_w) / 2, dy + (rayon_pion * 2 - font_size) / 2, font_size, COL_TEXT_BRIGHT);
                }
                else {
                    pion(debut_x + dessines * espacement + rayon_pion, case_y + taille_case - rayon_pion - PE(1, GetScreenHeight()), rayon_pion, couleur_pion);
                }
                dessines++;
            }
        }
    }
}

// =============================================================
//  PANNEAU DÉS (droite)
// =============================================================
void dessiner_panneau_des(Jeu* jeu, int joueur) {
    int panneau_x = PANNEAU_X;
    int panneau_largeur = PANNEAU_Y;
    int hauteur_ecran = GetScreenHeight();

    DrawRectangle(panneau_x, HDP_H, panneau_largeur, hauteur_ecran - HDP_H, FOND_SOMBRE);
    DrawRectangleLines(panneau_x, HDP_H, panneau_largeur, hauteur_ecran - HDP_H, COL_BORD_LIT);

    int titre_y = HDP_H + PE(1, hauteur_ecran);
    DrawText("DES", panneau_x + PE(2, panneau_largeur), titre_y, PE(2, hauteur_ecran), COL_ACCENT);

    int nb_des = 5 + possedetitanosaure(jeu, joueur);
    int taille_face = PE(5, hauteur_ecran);
    int taille_police = PE(2, hauteur_ecran);

    for (int i = 0; i < nb_des; i++) {
        int  action = jeu->Listede[i].action;
        bool bloque = jeu->Listede[i].bloque;
        bool selectionne = jeu->Listede[i].selectionne;

        int de_x = TABLEAU_X;
        int de_y = TABLEAU_Y + i * (DE_H + PE(1, hauteur_ecran));

        if (selectionne)
            carte(de_x - 4, de_y - 3, panneau_largeur - PE(3, panneau_largeur), DE_H + 6, (Color) { 13, 26, 13, 255 }, (Color) { 42, 106, 42, 255 });

        Color couleur_face = bloque ? (Color) { 100, 20, 10, 255 } : COULEUR_DE[action];
        DrawRectangleRounded((Rectangle) { de_x, de_y, taille_face, taille_face }, 0.15f, 6, couleur_face);
        DrawRectangleRoundedLines((Rectangle) { de_x, de_y, taille_face, taille_face }, 0.15f, 6, bloque ? RED : BLACK);

        int texte_x = de_x + taille_face + PE(1, panneau_largeur);
        DrawText(NOM_FACE[action], texte_x, de_y + PE(1, hauteur_ecran), taille_police, COL_TEXT);
        if (bloque)
            DrawText("BLOQUE", texte_x, de_y + PE(3, hauteur_ecran), taille_police, (Color) { 220, 60, 30, 255 });
        else if (selectionne)
            DrawText("selectionne", texte_x, de_y + PE(3, hauteur_ecran), taille_police, (Color) { 42, 180, 42, 255 });
    }

    // ----- SCORES -----
    int score_y = HDP_H + PE(58, hauteur_ecran);
    DrawLine(panneau_x + PE(2, panneau_largeur), score_y, panneau_x + panneau_largeur - PE(2, panneau_largeur), score_y, COL_BORD);
    score_y += PE(1, hauteur_ecran);
    DrawText("SCORES", panneau_x + PE(2, panneau_largeur), score_y, PE(2, hauteur_ecran), COL_ACCENT);
    score_y += PE(2, hauteur_ecran) + PE(1, hauteur_ecran);

    for (int i = 0; i < jeu->nb_joueurs; i++) {
        bool actif = (i == joueur);
        int  ligne_y = score_y + i * (PE(5, hauteur_ecran) + PE(1, hauteur_ecran));

        carte(panneau_x + PE(2, panneau_largeur), ligne_y, panneau_largeur - PE(4, panneau_largeur), PE(5, hauteur_ecran), actif ? (Color) { 22, 15, 6, 255 } : FOND_MID, actif ? COL_BORD_LIT : COL_BORD);

        pion(panneau_x + PE(4, panneau_largeur) + PE(2, hauteur_ecran), ligne_y + PE(2, hauteur_ecran) + 2, PE(2, hauteur_ecran), COULEUR_JOUEUR[i]);

        char nom[12];
        snprintf(nom, sizeof(nom), "Joueur %d", i + 1);
        DrawText(nom, panneau_x + PE(4, panneau_largeur) + PE(5, hauteur_ecran), ligne_y + (PE(5, hauteur_ecran) - PE(2, hauteur_ecran)) / 2, PE(2, hauteur_ecran), actif ? COL_TEXT_BRIGHT : COL_TEXT_DIM);

        char points_str[8];
        snprintf(points_str, sizeof(points_str), "%d", jeu->Joueurs[i].points);
        DrawText(points_str, panneau_x + panneau_largeur - PE(8, panneau_largeur), ligne_y + (PE(5, hauteur_ecran) - PE(3, hauteur_ecran)) / 2, PE(3, hauteur_ecran), actif ? COL_ACCENT : COL_TEXT_DIM);

        // Icône / nom dino
        if (jeu->Joueurs[i].a_dino) {
            const char* nom_dino[] = { "", "Tric.", "Ptero", "Bront", "Titan", "Stego", "Ankyl", "Plesi", "Tyran" };
            DrawText(nom_dino[jeu->Joueurs[i].dino_possede], panneau_x + PE(4, panneau_largeur) + PE(5, hauteur_ecran), ligne_y + (PE(5, hauteur_ecran) - PE(2, hauteur_ecran)) / 2 + PE(2, hauteur_ecran) + 2, PE(2, hauteur_ecran) - 2, (Color) { 80, 220, 80, 255 });
        }
    }
}

// =============================================================
//  MENU ACTIONS (étape 1)
// =============================================================
void dessiner_menu_actions(EtatAction* ea, int nb_oeufs) {
    if (ea->sous_etat != ACTION_MENU) return;

    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y - PE(2, GetScreenHeight());
    int boite_h = INFO_H;
    int boite_largeur = PE(40, GetScreenWidth());
    int hauteur_ligne = boite_h / 6;
    int taille_police = PE(2, GetScreenHeight());

    carte(boite_x, boite_y, boite_largeur, boite_h + hauteur_ligne , FOND_SOMBRE, COL_BORD_LIT);
    DrawText("CHOISIR UNE ACTION", boite_x + PE(2, boite_largeur), boite_y + hauteur_ligne / 2 - PE(1, GetScreenHeight()), taille_police - 2, COL_ACCENT_DIM);

    const char* labels[] = { "Caverne", "Hutte", "Deplacement", "Oeuf", "Terminer le tour" };
    const int   faces[] = { 1, 2, 3, 4, -1 };
    char buf[40];

    for (int i = 0; i < 5; i++) {
        int  ligne_y = boite_y + hauteur_ligne + i * (hauteur_ligne + 2 + PE(0.1, GetScreenHeight())) ;
        bool selectionne = (ea->selection_menu == i);
        bool dispo = (faces[i] == -1) || (ea->nb_actions[faces[i]] > 0);

        Color couleur_fond = selectionne ? (Color) { 30, 22, 10, 255 } : FOND_MID;
        Color couleur_bord = selectionne ? COL_ACCENT : COL_BORD;
        Color couleur_texte = dispo ? (selectionne ? COL_TEXT_BRIGHT : COL_TEXT) : COL_TEXT_DIM;

        carte(boite_x + PE(1, boite_largeur), ligne_y, boite_largeur - PE(2, boite_largeur), hauteur_ligne, couleur_fond, couleur_bord);
        if (selectionne)
            DrawText(">", boite_x + PE(2, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, COL_ACCENT);

        if (i == 3)
            snprintf(buf, sizeof(buf), "Oeufs  (%d dispo, min 2)", nb_oeufs);
        else if (faces[i] != -1)
            snprintf(buf, sizeof(buf), "%s  (%d restant%s)", labels[i], ea->nb_actions[faces[i]], ea->nb_actions[faces[i]] > 1 ? "s" : "");
        else
            snprintf(buf, sizeof(buf), "%s", labels[i]);

        DrawText(buf, boite_x + PE(5, boite_largeur),ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, couleur_texte);
    }

    int aide_y = boite_y + boite_h + hauteur_ligne - taille_police;
    DrawText("Haut/Bas : naviguer   Entree : valider   T : terminer", boite_x + PE(2, boite_largeur), aide_y, taille_police - 2, COL_TEXT_DIM);
}

// =============================================================
//  MENU CHOIX DINO
// =============================================================
static void dessiner_menu_dino(Jeu* jeu, EtatAction* ea) {
    if (ea->sous_etat != ACTION_OEUF_CHOISIR) return;

    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;
    int boite_h = INFO_H;
    int boite_largeur = PE(40, GetScreenWidth());
    int hauteur_ligne = boite_h / 5;
    int taille_police = PE(2, GetScreenHeight());

    carte(boite_x, boite_y, boite_largeur, boite_h, FOND_SOMBRE, COL_BORD_LIT);
    DrawText("CHOISIR UN DINOSAURE", boite_x + PE(2, boite_largeur), boite_y + hauteur_ligne / 2, taille_police - 2, COL_ACCENT_DIM);

    const char* noms_tous[] = {
        "", "Triceratops (3)", "Pteranodon (2)", 
        "Brontosaure (3)", "Titanosaure (5)",
        "Stegosaure (3)", "Ankylosaure (3)",
        "Plesiosaure (2)", "Tyrannosaure (4)"
    };
    
    for (int i = 0; i < 4; i++) {
        TypeDino td = jeu->dinos_en_jeu[i];
        int  ligne_y = boite_y + hauteur_ligne + i * (hauteur_ligne + 2);
        bool selectionne = (ea->selection_dino == i);

        carte(boite_x + PE(1, boite_largeur), ligne_y, boite_largeur - PE(2, boite_largeur), hauteur_ligne, selectionne ? (Color) { 13, 26, 13, 255 } : FOND_MID, selectionne ? COL_VALIDE : COL_BORD);
        if (selectionne)
            DrawText(">", boite_x + PE(2, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, COL_VALIDE);
        DrawText(noms_tous[td], boite_x + PE(5, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, selectionne ? COL_TEXT_BRIGHT : COL_TEXT);
    }
    DrawText("Haut/Bas: naviguer   Entree: confirmer   E: retour", boite_x + PE(2, boite_largeur), boite_y + boite_h - taille_police - 4, taille_police - 2, COL_TEXT_DIM);
}

// =============================================================
//  MENU CHOISIR PION (Dino ou Cromagnon)
// =============================================================
static void dessiner_menu_choix_pion(EtatAction* ea) {
    if (ea->sous_etat != ACTION_CHOISIR_PION) return;

    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;
    int boite_h = INFO_H;
    int boite_largeur = PE(40, GetScreenWidth());
    int hauteur_ligne = boite_h / 4;
    int taille_police = PE(2, GetScreenHeight());

    carte(boite_x, boite_y, boite_largeur, boite_h, FOND_SOMBRE, COL_BORD_LIT);
    DrawText("QUEL PION DEPLACER ?", boite_x + PE(2, boite_largeur),
        boite_y + hauteur_ligne / 2, taille_police - 2, COL_ACCENT_DIM);

    const char* noms[] = {
        "Cromagnon",
        "Dinosaure"
    };
    for (int i = 0; i < 2; i++) {
        int type_pion = i + 1; // 1 = Cromagnon, 2 = Dino
        int  ligne_y = boite_y + hauteur_ligne + i * (hauteur_ligne + 2);
        bool selectionne = (ea->type_pion_selectionne == type_pion);

        carte(boite_x + PE(1, boite_largeur), ligne_y, boite_largeur - PE(2, boite_largeur), hauteur_ligne, selectionne ? (Color) { 13, 26, 13, 255 } : FOND_MID, selectionne ? COL_VALIDE : COL_BORD);
        if (selectionne)
            DrawText(">", boite_x + PE(2, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, COL_VALIDE);
        DrawText(noms[i], boite_x + PE(5, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, selectionne ? COL_TEXT_BRIGHT : COL_TEXT);
    }
    DrawText("Haut/Bas : choisir   Entree : confirmer   ESC : retour", boite_x + PE(2, boite_largeur),  boite_y + boite_h - taille_police - 4, taille_police - 2, COL_TEXT_DIM);
}

// =============================================================
//  MENU VOLCAN (Sacrifice Cromagnon ou Dino)
// =============================================================
static void dessiner_menu_volcan(Jeu* jeu, int joueur, EtatAction* ea) {
    if (ea->sous_etat != ACTION_VOLCAN_MENU) return;

    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;
    int boite_h = INFO_H;
    int boite_largeur = PE(40, GetScreenWidth());
    int hauteur_ligne = boite_h / 4;
    int taille_police = PE(2, GetScreenHeight());

    carte(boite_x, boite_y, boite_largeur, boite_h, FOND_SOMBRE, COL_BORD_LIT);
    
    char titre[64];
    snprintf(titre, sizeof(titre), "SACRIFICE VOLCAN (%d restant%s)", ea->nb_actions[0], ea->nb_actions[0] > 1 ? "s" : "");
    DrawText(titre, boite_x + PE(2, boite_largeur), boite_y + hauteur_ligne / 2, taille_police - 2, COL_ACCENT_DIM);

    const char* noms[] = {
        "Sacrifier un Cromagnon",
        "Sacrifier un Dinosaure"
    };
    for (int i = 0; i < 2; i++) {
        int  ligne_y = boite_y + hauteur_ligne + i * (hauteur_ligne + 2);
        bool selectionne = (ea->selection_menu == i);
        bool dispo = (i == 0) ? true : a_dino_sacrifiable(jeu, joueur);

        Color col_fond = selectionne ? (Color) { 30, 22, 10, 255 } : FOND_MID;
        Color col_bord = selectionne ? COL_ACCENT : COL_BORD;
        Color col_text = dispo ? (selectionne ? COL_TEXT_BRIGHT : COL_TEXT) : COL_TEXT_DIM;

        carte(boite_x + PE(1, boite_largeur), ligne_y, boite_largeur - PE(2, boite_largeur), hauteur_ligne, col_fond, col_bord);
        if (selectionne)
            DrawText(">", boite_x + PE(2, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, COL_ACCENT);
        DrawText(noms[i], boite_x + PE(5, boite_largeur), ligne_y + (hauteur_ligne - taille_police) / 2, taille_police, col_text);
    }
    DrawText("Haut/Bas : choisir   Entree : valider", boite_x + PE(2, boite_largeur), boite_y + boite_h - taille_police - 4, taille_police - 2, COL_TEXT_DIM);
}

// =============================================================
//  BANNIERE D'AIDE pendant DEPLOYER / DEPLACER
// =============================================================
static void dessiner_hint_action(EtatAction* ea) {
    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;
    int boite_h = INFO_H;
    int boite_largeur = PE(40, GetScreenWidth());
    int taille_police = PE(2, GetScreenHeight());

    carte(boite_x, boite_y, boite_largeur, boite_h, FOND_SOMBRE, COL_ACCENT);

    const char* titre = "";
    const char* description = "";

    if (ea->sous_etat == ACTION_DEPLOYER_CASE) {
        if (ea->action_en_cours == 1) {
            titre = "DEPLOYER : CAVERNE";
            description = "Cliquez sur une caverne libre du plateau";
        }
        else {
            titre = "DEPLOYER : HUTTE";
            description = "Cliquez sur une hutte libre du plateau";
        }
    }
    else if (ea->sous_etat == ACTION_DEPLACER_ORIGINE) {
        titre = "DEPLACEMENT (1/2)";
        description = "Cliquez sur la case du pion a deplacer";
    }
    else if (ea->sous_etat == ACTION_DEPLACER_DEST) {
        titre = "DEPLACEMENT (2/2)";
        description = "Cliquez sur la case de destination (adjacente)";
    }

    DrawText(titre, boite_x + PE(3, boite_largeur), boite_y + PE(15, boite_h), taille_police + 2, COL_ACCENT);
    DrawText(description, boite_x + PE(3, boite_largeur), boite_y + PE(45, boite_h), taille_police, COL_TEXT_BRIGHT);
    DrawText("Touche E : annuler et revenir au menu", boite_x + PE(3, boite_largeur), boite_y + boite_h - taille_police - 6, taille_police - 2, COL_TEXT_DIM);
}

// =============================================================
//  INFO STRIP (étape 0)
// =============================================================
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer) {
    const char* phases[] = { "Lancer des des","Actions des des","Eruption !","Fin de tour" };
    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;
    int boite_h = INFO_H;
    int largeur_boite = INFO_BOITE_L;
    int gap = PE(1, GetScreenWidth());
    int taille_petite = PE(2, GetScreenHeight());
    int taille_grande = PE(4, GetScreenHeight());

    // PHASE
    carte(boite_x, boite_y, largeur_boite, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("PHASE", boite_x + PE(2, largeur_boite), boite_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);
    DrawText(phases[etape], boite_x + PE(2, largeur_boite), boite_y + PE(5, boite_h) + taille_petite + PE(2, boite_h), taille_petite, COL_TEXT_BRIGHT);
    char tour_txt[24];
    snprintf(tour_txt, sizeof(tour_txt), "Tour du joueur %d", joueur + 1);
    DrawText(tour_txt, boite_x + PE(2, largeur_boite), boite_y + boite_h - taille_petite - PE(5, boite_h), taille_petite - 2, COL_TEXT_DIM);

    // LANCERS
    int boite2_x = boite_x + largeur_boite + gap;
    carte(boite2_x, boite_y, largeur_boite, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("LANCERS", boite2_x + PE(2, largeur_boite), boite_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);
    char lancer_str[4];
    snprintf(lancer_str, sizeof(lancer_str), "%d", nblancer);
    int largeur_lancer = MeasureText(lancer_str, taille_grande);
    DrawText(lancer_str, boite2_x + (largeur_boite - largeur_lancer) / 2, boite_y + boite_h / 2 - taille_grande / 2, taille_grande, COL_ACCENT);

    int max_lancers = nb_lancers_total(jeu, joueur);
    char total_str[8];
    snprintf(total_str, sizeof(total_str), "/ %d", max_lancers);
    DrawText(total_str, boite2_x + (largeur_boite + largeur_lancer) / 2 + 4, boite_y + boite_h / 2 - taille_petite / 2, taille_petite, COL_TEXT_DIM);

    // RESERVE
    int boite3_x = boite_x + 2 * (largeur_boite + gap);
    carte(boite3_x, boite_y, largeur_boite, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("RESERVE", boite3_x + PE(2, largeur_boite), boite_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);
    char reserve_str[4];
    snprintf(reserve_str, sizeof(reserve_str), "%d", jeu->Joueurs[joueur].reserve);
    int largeur_reserve = MeasureText(reserve_str, taille_grande);
    DrawText(reserve_str, boite3_x + (largeur_boite - largeur_reserve) / 2, boite_y + boite_h / 2 - taille_grande / 2, taille_grande, COL_TEXT_BRIGHT);

    // OEUFS
    int boite4_x = boite_x + 3 * (largeur_boite + gap);
    carte(boite4_x, boite_y, largeur_boite, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("OEUFS", boite4_x + PE(2, largeur_boite), boite_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);
    char oeufs_str[4];
    snprintf(oeufs_str, sizeof(oeufs_str), "%d", jeu->Joueurs[joueur].oeufs);
    int largeur_oeufs = MeasureText(oeufs_str, taille_grande);
    DrawText(oeufs_str, boite4_x + (largeur_boite - largeur_oeufs) / 2, boite_y + boite_h / 2 - taille_grande / 2, taille_grande, COL_VALIDE);

    // SEUIL
    int boite5_x = boite_x + 4 * (largeur_boite + gap);
    carte(boite5_x, boite_y, largeur_boite, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("VICTOIRE", boite5_x + PE(2, largeur_boite), boite_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);
    char seuil_str[8];
    snprintf(seuil_str, sizeof(seuil_str), "%d pts", seuil_victoire(jeu->nb_joueurs));
    DrawText(seuil_str, boite5_x + PE(2, largeur_boite), boite_y + boite_h / 2 - taille_petite / 2, taille_petite, COL_TEXT_DIM);

    // RESERVE DINOS
    int boite6_x = boite_x + PE(42, GetScreenWidth());
    int boite6_y = boite_y - PE(15.5, GetScreenHeight());
    float largeur_boite_dinos = 2.6 * largeur_boite;
    carte(boite6_x, boite6_y, largeur_boite_dinos, boite_h, FOND_SOMBRE, COL_BORD);
    DrawText("DINOSAURES DISPONIBLES", boite6_x + PE(2, largeur_boite_dinos), boite6_y + PE(5, boite_h), taille_petite - 2, COL_ACCENT_DIM);

    const char* trigrammes[] = { "", "Tri", "Pte", "Bro", "Tit", "Ste", "Ank", "Ple", "Tyr" };
    
    char d_buf1[128];
    char d_buf2[128];
    TypeDino d1 = jeu->dinos_en_jeu[0];
    TypeDino d2 = jeu->dinos_en_jeu[1];
    TypeDino d3 = jeu->dinos_en_jeu[2];
    TypeDino d4 = jeu->dinos_en_jeu[3];

    snprintf(d_buf1, sizeof(d_buf1), "%s:%d/2    %s:%d/2", 
             trigrammes[d1], jeu->dinos_disponibles[d1 - 1], 
             trigrammes[d2], jeu->dinos_disponibles[d2 - 1]);
             
    snprintf(d_buf2, sizeof(d_buf2), "%s:%d/2    %s:%d/2", 
             trigrammes[d3], jeu->dinos_disponibles[d3 - 1], 
             trigrammes[d4], jeu->dinos_disponibles[d4 - 1]);

    DrawText(d_buf1, boite6_x + PE(2, largeur_boite_dinos), boite6_y + boite_h / 3, taille_petite - 2, COL_TEXT_BRIGHT);
    DrawText(d_buf2, boite6_x + PE(2, largeur_boite_dinos), boite6_y + 2 * boite_h / 3, taille_petite - 2, COL_TEXT_BRIGHT);
}

// =============================================================
//  STATUS BAR
// =============================================================
void dessiner_statusbar(int etape, bool achoisides) {
    int barre_y = STATUS_Y;
    int largeur_ecran = GetScreenWidth();
    int hauteur_barre = GetScreenHeight() - barre_y;

    DrawRectangle(0, barre_y, largeur_ecran, hauteur_barre, FOND_SOMBRE);
    DrawLine(0, barre_y, largeur_ecran, barre_y, COL_BORD_LIT);

    int texte_y = barre_y + (hauteur_barre - PE(2, GetScreenHeight())) / 2;
    int taille_police = PE(2, GetScreenHeight());

    const char* phases[] = { "Lancer des des","Actions","Eruption","Fin de tour" };
    int idx = etape < 4 ? etape : 0;
    DrawText(phases[idx], PE(2, largeur_ecran), texte_y, taille_police, COL_ACCENT);
    DrawLine(PE(14, largeur_ecran), barre_y + PE(15, hauteur_barre), PE(14, largeur_ecran), barre_y + PE(85, hauteur_barre), COL_BORD);

    const char* msg = "";
    if (etape == 0)
        msg = achoisides ? "ESPACE pour lancer les des" : "Clic pour bloquer/debloquer un de  |  ENTREE pour valider";
    else if (etape == 1)
        msg = "Haut/Bas + ENTREE pour choisir  |  Clic plateau pour placer/deplacer  |  ESC pour annuler  |  T pour terminer";
    else if (etape == 2)
        msg = "ESPACE/ENTREE : voir les scores  |  Fleches : deplacer le volcan  |  ENTREE : confirmer";
    DrawText(msg, PE(15, largeur_ecran), texte_y, taille_police, COL_TEXT_DIM);
}

// =============================================================
//  ÉCRAN ÉRUPTION
// =============================================================
void affichage_eruption(Jeu* jeu, PhaseEruption* pe, int joueur) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, NULL, joueur);
    dessiner_panneau_des(jeu, joueur);

    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();
    int boite_largeur = PE(44, largeur_ecran);
    int boite_hauteur = PE(22, hauteur_ecran);
    int boite_x = PLATEAU_X;
    int boite_y = INFO_Y;

    if (pe->phase == ERUPTION_VOLCAN_SCORE) {
        carte(boite_x, boite_y, boite_largeur, boite_hauteur, FOND_SOMBRE, COL_ACCENT);
        texte_centre("ERUPTION !", boite_x, boite_y + PE(8, boite_hauteur), boite_largeur, PE(4, hauteur_ecran), COL_ACCENT);

        // Récap pions présents sur le volcan
        int vl = trouver_volcan_ligne(jeu);
        int vc = trouver_volcan_col(jeu);
        int totaux[4] = { 0 };
        if (vl >= 0)
            for (int p = 0; p < 10; p++)
                if (jeu->PlateauPion[vl][vc][p].TypePion != 0)
                    totaux[jeu->PlateauPion[vl][vc][p].joueur]++;

        char buf[48];
        int ligne_y = boite_y + PE(35, boite_hauteur);
        int taille_police = PE(2, hauteur_ecran);
        for (int i = 0; i < jeu->nb_joueurs; i++) {
            if (totaux[i] > 0) {
                snprintf(buf, sizeof(buf), "J%d : %d pion(s) sur le volcan", i + 1, totaux[i]);
                DrawText(buf, boite_x + PE(3, boite_largeur), ligne_y, taille_police, COULEUR_JOUEUR[i]);
                ligne_y += taille_police + 4;
            }
        }
        texte_centre("ESPACE ou ENTREE pour continuer", boite_x, boite_y + PE(80, boite_hauteur), boite_largeur, taille_police - 2, COL_TEXT_DIM);
    }
    else if (pe->phase == ERUPTION_DEPLACEMENT) {
        carte(boite_x, boite_y, boite_largeur, boite_hauteur, FOND_SOMBRE, COL_BORD_LIT);
        texte_centre("DEPLACER LE VOLCAN", boite_x, boite_y + PE(8, boite_hauteur), boite_largeur, PE(3, hauteur_ecran), COL_ACCENT);

        char buf[32];
        snprintf(buf, sizeof(buf), "Deplacements restants : %d", pe->moves_restants);
        int taille_police = PE(2, hauteur_ecran);
        DrawText(buf, boite_x + PE(3, boite_largeur), boite_y + PE(45, boite_hauteur), taille_police, COL_TEXT);
        texte_centre("Fleches pour deplacer  |  ENTREE pour valider", boite_x, boite_y + PE(78, boite_hauteur), boite_largeur, taille_police - 2, COL_TEXT_DIM);
    }

    dessiner_statusbar(2, false);
}

// =============================================================
//  ÉCRAN DE FIN
// =============================================================
void affichage_fin(Jeu* jeu, int gagnant) {
    int largeur_ecran = GetScreenWidth();
    int hauteur_ecran = GetScreenHeight();
    DrawRectangle(0, 0, largeur_ecran, hauteur_ecran, (Color) { 0, 0, 0, 160 });

    int boite_largeur = PE(50, largeur_ecran);
    int boite_hauteur = PE(50, hauteur_ecran);
    int boite_x = (largeur_ecran - boite_largeur) / 2;
    int boite_y = (hauteur_ecran - boite_hauteur) / 2;
    carte(boite_x, boite_y, boite_largeur, boite_hauteur, FOND_SOMBRE, COL_BORD_LIT);

    int taille_grande = PE(5, hauteur_ecran);
    int taille_moyenne = PE(3, hauteur_ecran);
    int taille_petite = PE(2, hauteur_ecran);

    texte_centre("FIN DE PARTIE", boite_x, boite_y + PE(5, boite_hauteur),
        boite_largeur, taille_grande, COL_ACCENT);

    // Détection d'égalité
    int meilleur_score = -1, nb_a_meilleur = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        if (jeu->Joueurs[i].points > meilleur_score) {
            meilleur_score = jeu->Joueurs[i].points;
            nb_a_meilleur = 1;
        }
        else if (jeu->Joueurs[i].points == meilleur_score) {
            nb_a_meilleur++;
        }
    }

    if (gagnant >= 0 && nb_a_meilleur == 1) {
        char msg[40];
        snprintf(msg, sizeof(msg), "Joueur %d remporte la partie !", gagnant + 1);
        texte_centre(msg, boite_x, boite_y + PE(28, boite_hauteur), boite_largeur, taille_moyenne, COULEUR_JOUEUR[gagnant]);
    }
    else {
        texte_centre("Egalite !", boite_x, boite_y + PE(28, boite_hauteur), boite_largeur, taille_moyenne, COL_ACCENT);
    }

    DrawText("Scores finaux :", boite_x + PE(10, boite_largeur), boite_y + PE(45, boite_hauteur), taille_petite, COL_TEXT_DIM);
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        char score_str[40];
        snprintf(score_str, sizeof(score_str), "Joueur %d : %d pts  (seuil : %d)", i + 1, jeu->Joueurs[i].points, seuil_victoire(jeu->nb_joueurs));
        bool est_gagnant = (jeu->Joueurs[i].points == meilleur_score);
        DrawText(score_str, boite_x + PE(10, boite_largeur), boite_y + PE(52, boite_hauteur) + i * (taille_petite + 4), taille_petite, est_gagnant ? COL_ACCENT : COL_TEXT_DIM);
    }
    texte_centre("E pour quitter", boite_x, boite_y + PE(88, boite_hauteur),
        boite_largeur, taille_petite - 2, COL_TEXT_DIM);
}

// =============================================================
//  RENDU COMPLET – étape 0 (lancer des dés)
// =============================================================
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, NULL, joueur);
    dessiner_panneau_des(jeu, joueur);
    dessiner_info_strip(jeu, joueur, etape, nblancer);
    dessiner_statusbar(etape, achoisides);
}

// =============================================================
//  RENDU COMPLET – étape 1 (actions)
// =============================================================
void affichage_actions(Jeu* jeu, int joueur, EtatAction* ea) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, ea, joueur);
    dessiner_panneau_des(jeu, joueur);

    int sous = ea->sous_etat;
    if (sous == ACTION_OEUF_CHOISIR) dessiner_menu_dino(jeu, ea);
    if (sous == ACTION_CHOISIR_PION) dessiner_menu_choix_pion(ea);
    if (sous == ACTION_VOLCAN_MENU) dessiner_menu_volcan(jeu, joueur, ea);

    if (sous == ACTION_DEPLOYER_CASE || sous == ACTION_DEPLACER_ORIGINE ||  sous == ACTION_DEPLACER_DEST) { dessiner_hint_action(ea);
    }
    else if (sous == ACTION_MENU) {
        dessiner_menu_actions(ea, jeu->Joueurs[joueur].oeufs);
    }
    else if (sous != ACTION_OEUF_CHOISIR && sous != ACTION_CHOISIR_PION && sous != ACTION_VOLCAN_MENU) {
        dessiner_hint_action(ea);
    }

    dessiner_statusbar(1, false);
}

// =============================================================
//  FENETRE
// =============================================================
void initialiser_fenetre(void) {
    int ecran = GetCurrentMonitor();
    InitWindow(GetMonitorWidth(ecran), GetMonitorHeight(ecran), "Colere de la Montagne de Feu");
    SetWindowPosition(0, 0);
    SetTargetFPS(GetMonitorRefreshRate(ecran));
	ToggleBorderlessWindowed();
}
