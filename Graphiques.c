#include "Graphiques.h"
#include "Logique.h"
#include <stdio.h>

// =============================================================
//  PE
// =============================================================
int PE(float pourcentage, float valeur) {
    return (int)(valeur / 100.0f * pourcentage);
}

// =============================================================
//  Utilitaires internes
// =============================================================
void carte(int x, int y, int w, int h, Color fond, Color bordure) {
    DrawRectangleRounded((Rectangle) { x, y, w, h }, 0.08f, 6, fond);
    DrawRectangleRoundedLines((Rectangle) { x, y, w, h }, 0.08f, 6, bordure);
}


//C'est juste pour center le texte 
void texte_centre(const char* txt, int x, int y, int w, int taille, Color col) {
    int Longueur = MeasureText(txt, taille);
    DrawText(txt, x + (w - Longueur) / 2, y, taille, col);
}

//Pour dessiner les pions, à voir si on change avec des images
void pion(int cx, int cy, int r, Color couleur) {
    DrawCircle(cx, cy, (float)r, couleur);
    DrawCircleLines((float)cx, (float)cy, (float)(r + 1), (Color) { 0, 0, 0, 180 });
}

// =============================================================
//  La fonciton permet de savoir ou le joueur a cliqué et si il a cliqué (marche que sur le plateau par contre) FONCTIONNELLE
// =============================================================
bool clic_sur_plateau(Vector2 souris, int* ligne, int* col) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            Rectangle rectangle = { (float)(PLATEAU_X + y * (CASE + CASE_ESPACE)), (float)(PLATEAU_Y + x * (CASE + CASE_ESPACE)), (float)CASE, (float)CASE };
            if (CheckCollisionPointRec(souris, rectangle)) {
                *ligne = x;
                *col = y;
                return true;
            }
        }
    }
    return false;
}

// =============================================================
//  Haut De Page FONCTIONNELLE
// =============================================================
void Dessiner_HDP(Jeu* jeu, int joueur) {
    int L = GetScreenWidth();
    int H = HDP_H;
    DrawRectangle(0, 0, L, H, COL_BG_DARK);
    DrawLine(0, H, L, H, COL_BORDER_LIT);
    DrawText("COLERE DE LA MONTAGNE DE FEU", PE(2, L), PE(1, GetScreenHeight()), PE(3, GetScreenHeight()), COL_ACCENT);

    int rayon = PE(2, GetScreenHeight());
    int cercle_x = PE(80, L);
    int cercle_y = H / 2;
    for (int i = 0; i < 4; i++) {
        Color c = COULEUR_JOUEUR[i];

        if (i != joueur) {
            c.a = 70; // l'alpha de la couleur du joueur p  as actif
        }

        int cx = cercle_x + i * (rayon * 2 + PE(1, L));
        DrawCircle(cx, cercle_y, (float)rayon, c);

        if (i == joueur) {
            DrawCircleLines((float)cx, (float)cercle_y, (float)(rayon + 2), COL_TEXT_BRIGHT);
        }

        char labeljoueur[3];
		snprintf(labeljoueur, sizeof(labeljoueur), "J%d", i + 1); //permet de convertir un integer en string et de le stocker dans labeljoueur comme ça on peut l'afficher (j'ai pas trouvé autrement)
        int lw = MeasureText(labeljoueur, PE(2, GetScreenHeight()));
        DrawText(labeljoueur, cx - lw / 2, cercle_y - PE(1, GetScreenHeight()), PE(2, GetScreenHeight()), (Color) { 255, 255, 255, i == joueur ? 220 : 100 }); //en fonction de si on est le joueur actif ou pas (ça s'illumine)
    }
}

// =============================================================
//  PLATEAU  (avec surbrillance selon EtatAction)
// =============================================================
void afficherplateau(Jeu* jeu, EtatAction* ea, int joueur) {
    int bx = PLATEAU_X;
    int by = PLATEAU_Y;
    int cs = CASE;
    int espace = CASE_ESPACE;  
    int tot = 4 * cs + 3 * espace;

    DrawRectangleRounded((Rectangle) { bx - 8, by - 8, tot + 16, tot + 16 }, 0.04f, 4, (Color) { 12, 8, 3, 255 });
    DrawRectangleRoundedLines((Rectangle) { bx - 8, by - 8, tot + 16, tot + 16 }, 0.04f, 4, COL_BORDER);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int type = jeu->plateau[row][col].TypeCase;
            int cx = bx + col * (cs + espace);
            int cy = by + row * (cs + espace);

            Color fond = (type >= 0 && type < 6) ? COULEUR_CASE[type] : COL_BG_MID;
            Color bordure = (type >= 0 && type < 6) ? COULEUR_CASE_BORD[type] : COL_BORDER;
            Color lblcol = (type >= 0 && type < 6) ? COULEUR_CASE_LABEL[type] : COL_TEXT_DIM;

            // Halo volcan
            if (type == 0)
                DrawRectangleRounded((Rectangle) { cx - 3, cy - 3, cs + 6, cs + 6 }, 0.1f, 4, (Color) { 255, 60, 0, 40 });

            // ── Surbrillance selon l'état d'action ──
            if (ea != NULL) {
                bool hl_vert = false;
                bool hl_orange = false;

                if (ea->sous_etat == ACTION_DEPLOYER_CASE) {
                    int type_cible = (ea->action_en_cours == 1) ? 5 : 4;
                    if (type == type_cible && jeu->Joueurs[joueur].reserve > 0)
                        hl_vert = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_ORIGINE) {
                    if (compter_pions_joueur(jeu, joueur, row, col) > 0 && type != 0)
                        hl_vert = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_DEST) {
                    if (row == ea->orig_ligne && col == ea->orig_col)
                        hl_orange = true; // case source sélectionnée
                    else if (case_adjacente(ea->orig_ligne, ea->orig_col, row, col))
                        hl_vert = true;
                }

                if (hl_vert)
                    DrawRectangleRounded((Rectangle) { cx - 4, cy - 4, cs + 8, cs + 8 }, 0.1f, 4, (Color) {42, 180, 42, 60});

                if (hl_orange)
                    DrawRectangleRounded((Rectangle) { cx - 4, cy - 4, cs + 8, cs + 8 }, 0.1f, 4,(Color) {240, 140, 0, 80});

                if (hl_vert)
                    bordure = COL_VALIDE;
                if (hl_orange)
                    bordure = COL_ACCENT;
            }

            DrawRectangleRounded((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, fond);
            DrawRectangleRoundedLines((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, bordure);

            int ft = PE(1, GetScreenHeight());
            texte_centre(NOM_CASE[type], cx, cy + cs - ft - PE(1, GetScreenHeight()), cs, ft, lblcol);

            // Pions
            int nb = 0;
            for (int p = 0; p < 10; p++)
                if (jeu->PlateauPion[row][col][p].TypePion != 0) nb++;

            int pr = cs / 9;
            int spacing = pr * 2 + 2;
            int startx = cx + (cs - nb * spacing + 2) / 2;
            int drawn = 0;
            for (int p = 0; p < 10 && drawn < nb; p++) {
                if (jeu->PlateauPion[row][col][p].TypePion != 0) {
                    int pj = jeu->PlateauPion[row][col][p].joueur;
                    Color pc = (pj >= 0 && pj < 4) ? COULEUR_JOUEUR[pj] : COL_TEXT;
                    pion(startx + drawn * spacing + pr, cy + cs - pr - PE(1, GetScreenHeight()), pr, pc);
                    drawn++;
                }
            }
        }
    }
}

// =============================================================
//  PANNEAU DÉS
// =============================================================
void dessiner_panel_des(Jeu* jeu, int joueur) {
    int PX = PANNEAU_X;
    int PY = PANNEAU_Y;
    int H = GetScreenHeight();

    DrawRectangle(PX, HDP_H, PY, H - HDP_H, COL_BG_DARK);
	DrawRectangleLines(PX, HDP_H, PY, H - HDP_H, COL_BORDER_LIT);

    int y = HDP_H + PE(1, H);
    DrawText("DES", PX + PE(2, PY), y, PE(2, H), COL_ACCENT);

    int nb_des = 5 + possedetitanosaure(jeu, joueur);
    int face_sz = PE(5, H);
    int Police = PE(2, H);

    for (int i = 0; i < nb_des; i++) {
        int  action = jeu->Listede[i].action;
        bool bloque = jeu->Listede[i].bloque;
        bool sel = jeu->Listede[i].selectionne;

        int rx = TABLEAU_X;
        int ry = TABLEAU_Y + i * (DE_H + PE(1, H));

        if (sel)
            carte(rx - 4, ry - 3, PY - PE(3, PY), DE_H + 6, (Color) { 13, 26, 13, 255 }, (Color) { 42, 106, 42, 255 });

        Color fond_de = bloque ? (Color) { 100, 20, 10, 255 } : COULEUR_DE[action];
        DrawRectangleRounded((Rectangle) { rx, ry, face_sz, face_sz }, 0.15f, 6, fond_de);
        DrawRectangleRoundedLines((Rectangle) { rx, ry, face_sz, face_sz }, 0.15f, 6, bloque ? RED : BLACK);

        int tx2 = rx + face_sz + PE(1, PY);
		DrawText(NOM_FACE[action], tx2, ry + PE(1, H), Police, COL_TEXT); //Affichage du nom de la face du dé à côté du dé

        if (bloque)
            DrawText("BLOQUE", tx2, ry + PE(3, H), Police, (Color) { 220, 60, 30, 255 }); //Affichage des mots bloques et séléctionné 
        else if (sel)
            DrawText("selectionne", tx2, ry + PE(3, H), Police, (Color) { 42, 180, 42, 255 });
    }

    // Scores
    int sy = HDP_H + PE(58, H);
    DrawLine(PX + PE(2, PY), sy, PX + PY - PE(2, PY), sy, COL_BORDER); sy += PE(1, H);
    DrawText("SCORES", PX + PE(2, PY), sy, PE(2, H), COL_ACCENT); sy += PE(2, H) + PE(1, H);

    for (int i = 0; i < 4; i++) {
        bool actif = false;
        if (i == joueur)
            actif = true;
        int ry2 = sy + i * (PE(5, H) + PE(1, H));
        carte(PX + PE(2, PY), ry2, PY - PE(4, PY), PE(5, H),actif ? (Color) { 22, 15, 6, 255 } : COL_BG_MID, actif ? COL_BORDER_LIT : COL_BORDER);
        pion(PX + PE(4, PY) + PE(2, H), ry2 + PE(2, H) + 2, PE(2, H), COULEUR_JOUEUR[i]);
        char nom[12]; 
        snprintf(nom, sizeof(nom), "Joueur %d", i + 1);
        DrawText(nom, PX + PE(4, PY) + PE(5, H), ry2 + (PE(5, H) - PE(2, H)) / 2, PE(2, H), actif ? COL_TEXT_BRIGHT : COL_TEXT_DIM);
        char pts[8]; 
        snprintf(pts, sizeof(pts), "%d", jeu->Joueurs[i].points);
        DrawText(pts, PX + PY - PE(8, PY), ry2 + (PE(5, H) - PE(3, H)) / 2, PE(3, H), actif ? COL_ACCENT : COL_TEXT_DIM);
    }
}

// =============================================================
//  MENU D'ACTIONS (affiché sous le plateau pendant étape 1)
// =============================================================
void dessiner_menu_actions(EtatAction* ea) {
    if (ea->sous_etat != ACTION_MENU) return;

    int bx = PLATEAU_X;
    int by = INFO_Y;
    int h = INFO_H;
    int fw = PE(40, GetScreenWidth());
    int fh = h / 5;
    int ft = PE(2, GetScreenHeight());

    carte(bx, by, fw, h, COL_BG_DARK, COL_BORDER_LIT);
    DrawText("CHOISIR UNE ACTION", bx + PE(2, fw), by + fh / 2, ft - 2, COL_ACCENT_DIM);

    const char* labels[] = { "Caverne", "Hutte", "Empreinte", "Terminer le tour" };
    const int   faces[] = { 1, 2, 3, -1 };
    const char* suffixes[] = { "", "", "", "" };
    char buf[32];

    for (int i = 0; i < 4; i++) {
        int ry = by + fh + i * (fh + 2);
        bool sel = (ea->selection_menu == i);
        bool dispo = (faces[i] == -1) || (ea->nb_actions[faces[i]] > 0);

        Color fond_l = sel ? (Color) { 30, 22, 10, 255 } : COL_BG_MID;
        Color bord_l = sel ? COL_ACCENT : COL_BORDER;
        Color txt_col = dispo ? (sel ? COL_TEXT_BRIGHT : COL_TEXT) : COL_TEXT_DIM;

        carte(bx + PE(1, fw), ry, fw - PE(2, fw), fh, fond_l, bord_l);

        if (sel) DrawText(">", bx + PE(2, fw), ry + (fh - ft) / 2, ft, COL_ACCENT);

        if (faces[i] != -1) {
            snprintf(buf, sizeof(buf), "%s  (%d restant%s)",
                labels[i],
                ea->nb_actions[faces[i]],
                ea->nb_actions[faces[i]] > 1 ? "s" : "");
        }
        else {
            snprintf(buf, sizeof(buf), "%s", labels[i]);
        }
        DrawText(buf, bx + PE(5, fw), ry + (fh - ft) / 2, ft, txt_col);
    }

    // Aide touches
    int help_y = by + h - ft - 4;
    DrawText("Haut/Bas : naviguer   Entree : valider   T : terminer",
        bx + PE(2, fw), help_y, ft - 2, COL_TEXT_DIM);
}

// =============================================================
//  INFO STRIP (étape 0 : lancer des dés)
// =============================================================
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer) {
    const char* phases[] = {
        "Lancer des des","Actions des des","Seismes","Fin de tour"
    };
    int bx = PLATEAU_X;
    int y = INFO_Y;
    int h = INFO_H;
    int cw = INFO_BOITE_L;
    int gap = PE(1, GetScreenWidth());
    int flab = PE(2, GetScreenHeight());
    int fval = PE(4, GetScreenHeight());

    carte(bx, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("PHASE", bx + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    DrawText(phases[etape], bx + PE(2, cw), y + PE(5, h) + flab + PE(2, h), flab, COL_TEXT_BRIGHT);
    char tour[24]; snprintf(tour, sizeof(tour), "Tour du joueur %d", joueur + 1);
    DrawText(tour, bx + PE(2, cw), y + h - flab - PE(5, h), flab - 2, COL_TEXT_DIM);

    int bx2 = bx + cw + gap;
    carte(bx2, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("LANCERS", bx2 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char lstr[4]; snprintf(lstr, sizeof(lstr), "%d", nblancer);
    int lw = MeasureText(lstr, fval);
    DrawText(lstr, bx2 + (cw - lw) / 2, y + h / 2 - fval / 2, fval, COL_ACCENT);
    DrawText("/ 3", bx2 + (cw + lw) / 2 + 4, y + h / 2 - flab / 2, flab, COL_TEXT_DIM);

    int bx3 = bx + 2 * (cw + gap);
    carte(bx3, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("RESERVE", bx3 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char rstr[4]; snprintf(rstr, sizeof(rstr), "%d", jeu->Joueurs[joueur].reserve);
    int rw = MeasureText(rstr, fval);
    DrawText(rstr, bx3 + (cw - rw) / 2, y + h / 2 - fval / 2, fval, COL_TEXT_BRIGHT);
}

// =============================================================
//  STATUS BAR
// =============================================================
void dessiner_statusbar(int etape, bool achoisides) {
    int sy = STATUS_Y;
    int sw = GetScreenWidth();
    int sh = GetScreenHeight() - sy;
    DrawRectangle(0, sy, sw, sh, COL_BG_DARK);
    DrawLine(0, sy, sw, sy, COL_BORDER_LIT);

    int fy = sy + (sh - PE(2, GetScreenHeight())) / 2;
    int ftaille = PE(2, GetScreenHeight());
    const char* phases[] = { "Lancer des des","Actions","Seismes","Fin de tour" };
    DrawText(phases[etape], PE(2, sw), fy, ftaille, COL_ACCENT);
    DrawLine(PE(14, sw), sy + PE(15, sh), PE(14, sw), sy + PE(85, sh), COL_BORDER);

    const char* msg = "";
    if (etape == 0)
        msg = achoisides ? "ESPACE pour lancer les des"
        : "Clic pour bloquer/debloquer un de  |  ENTREE pour valider";
    else if (etape == 1)
        msg = "Haut/Bas + ENTREE pour choisir une action  |  Clic sur le plateau pour placer/deplacer  |  T pour terminer";
    DrawText(msg, PE(15, sw), fy, ftaille, COL_TEXT_DIM);
}

// =============================================================
//  ÉCRAN DE FIN
// =============================================================
void affichage_fin(Jeu* jeu, int gagnant) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    ClearBackground(COL_BG);

    // Overlay sombre
    DrawRectangle(0, 0, sw, sh, (Color) { 0, 0, 0, 160 });

    int bw = PE(50, sw);
    int bh = PE(50, sh);
    int bx = (sw - bw) / 2;
    int by = (sh - bh) / 2;
    carte(bx, by, bw, bh, COL_BG_DARK, COL_BORDER_LIT);

    int ft_big = PE(5, sh);
    int ft_med = PE(3, sh);
    int ft_sml = PE(2, sh);

    texte_centre("FIN DE PARTIE", bx, by + PE(5, bh), bw, ft_big, COL_ACCENT);

    if (gagnant >= 0) {
        char msg[32];
        snprintf(msg, sizeof(msg), "Joueur %d remporte la partie !", gagnant + 1);
        // Couleur du joueur gagnant
        texte_centre(msg, bx, by + PE(28, bh), bw, ft_med, COULEUR_JOUEUR[gagnant]);
    }

    // Scores finaux
    DrawText("Scores finaux :", bx + PE(10, bw), by + PE(45, bh), ft_sml, COL_TEXT_DIM);
    for (int i = 0; i < 4; i++) {
        char sc[24];
        snprintf(sc, sizeof(sc), "Joueur %d : %d pts", i + 1, jeu->Joueurs[i].points);
        DrawText(sc, bx + PE(10, bw), by + PE(52, bh) + i * (ft_sml + 4), ft_sml,
            i == gagnant ? COL_ACCENT : COL_TEXT_DIM);
    }

    texte_centre("Appuyez sur ECHAP pour quitter", bx, by + PE(88, bh), bw, ft_sml - 2, COL_TEXT_DIM);
}

// =============================================================
//  RENDU COMPLET – étape 0 (lancers)
// =============================================================
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides) {
    ClearBackground(COL_BG);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, NULL, joueur);
    dessiner_panel_des(jeu, joueur);
    dessiner_info_strip(jeu, joueur, etape, nblancer);
    dessiner_statusbar(etape, achoisides);
}

// =============================================================
//  RENDU COMPLET – étape 1 (actions)
// =============================================================
void affichage_actions(Jeu* jeu, int joueur, EtatAction* ea) {
    ClearBackground(COL_BG);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, ea, joueur);
    dessiner_panel_des(jeu, joueur);
    dessiner_menu_actions(ea);
    dessiner_statusbar(1, false);
}

// =============================================================
//  FENETRE
// =============================================================
void initialiser_fenetre(void) {
    // 1. Tell Raylib to respect High-DPI scaling from the start
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);

    // 2. IMPORTANT: Do not hardcode 3840 unless you want a window 
    // that covers your ENTIRE desktop across scaling.
    // 1920x1080 is much safer as a "logical" resolution.
    InitWindow(1920, 1080, "Colere de la Montagne de Feu");

    // 3. Set the window to appear on the correct monitor
    // and maximize it if you want it to fill the screen properly
    if (IsWindowReady()) {
        SetTargetFPS(60);
    }
}