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

void texte_centre(const char* txt, int x, int y, int w, int taille, Color col) {
    int Longueur = MeasureText(txt, taille);
    DrawText(txt, x + (w - Longueur) / 2, y, taille, col);
}

void pion(int cx, int cy, int r, Color couleur) {
    DrawCircle(cx, cy, (float)r, couleur);
    DrawCircleLines((float)cx, (float)cy, (float)(r + 1), (Color) { 0, 0, 0, 180 });
}

// =============================================================
//  Clic plateau
// =============================================================
bool clic_sur_plateau(Vector2 souris, int* ligne, int* col) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            Rectangle r = {
                (float)(PLATEAU_X + y * (CASE + CASE_ESPACE)),
                (float)(PLATEAU_Y + x * (CASE + CASE_ESPACE)),
                (float)CASE, (float)CASE
            };
            if (CheckCollisionPointRec(souris, r)) {
                *ligne = x; *col = y;
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
    int L = GetScreenWidth();
    int H = HDP_H;
    DrawRectangle(0, 0, L, H, FOND_SOMBRE);
    DrawLine(0, H, L, H, COL_BORD_LIT);
    DrawText("COLERE DE LA MONTAGNE DE FEU", PE(2, L), PE(1, GetScreenHeight()), PE(3, GetScreenHeight()), COL_ACCENT);

    int rayon = PE(2, GetScreenHeight());
    int cercle_x = PE(80, L);
    int cercle_y = H / 2;

    for (int i = 0; i < jeu->nb_joueurs; i++) {
        Color c = COULEUR_JOUEUR[i];
        if (i != joueur) c.a = 70;
        int cx = cercle_x + i * (rayon * 2 + PE(1, L));
        DrawCircle(cx, cercle_y, (float)rayon, c);
        if (i == joueur)
            DrawCircleLines((float)cx, (float)cercle_y, (float)(rayon + 2), COL_TEXT_BRIGHT);

        char label[3];
        snprintf(label, sizeof(label), "J%d", i + 1);
        int lw = MeasureText(label, PE(2, GetScreenHeight()));
        DrawText(label, cx - lw / 2, cercle_y - PE(1, GetScreenHeight()),
            PE(2, GetScreenHeight()),
            (Color) {
            255, 255, 255, i == joueur ? 220 : 100
        });

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
    int bx = PLATEAU_X, by = PLATEAU_Y;
    int cs = CASE, espace = CASE_ESPACE;
    int tot = 4 * cs + 3 * espace;

    DrawRectangleRounded((Rectangle) { bx - 8, by - 8, tot + 16, tot + 16 }, 0.04f, 4, (Color) { 12, 8, 3, 255 });
    DrawRectangleRoundedLines((Rectangle) { bx - 8, by - 8, tot + 16, tot + 16 }, 0.04f, 4, COL_BORD);

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int type = jeu->plateau[row][col].TypeCase;
            int cx = bx + col * (cs + espace);
            int cy = by + row * (cs + espace);

            Color fond = (type >= 0 && type < 6) ? COULEUR_CASE[type] : FOND_MID;
            Color bordure = (type >= 0 && type < 6) ? COULEUR_CASE_BORD[type] : COL_BORD;
            Color lblcol = (type >= 0 && type < 6) ? COULEUR_CASE_LABEL[type] : COL_TEXT_DIM;

            if (type == 0)
                DrawRectangleRounded((Rectangle) { cx - 3, cy - 3, cs + 6, cs + 6 }, 0.1f, 4,
                    (Color) {
                255, 60, 0, 40
            });

            if (ea != NULL) {
                bool hl_vert = false, hl_orange = false;

                if (ea->sous_etat == ACTION_DEPLOYER_CASE) {
                    int type_cible = (ea->action_en_cours == 1) ? 5 : 4;
                    bool brachio = (jeu->Joueurs[joueur].a_dino &&
                        jeu->Joueurs[joueur].dino_possede == DINO_BRACHIO);
                    bool ok = brachio ? (type != 0) : (type == type_cible);
                    if (ok && jeu->Joueurs[joueur].reserve > 0) hl_vert = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_ORIGINE) {
                    if (compter_pions_joueur(jeu, joueur, row, col) > 0 && type != 0)
                        hl_vert = true;
                }
                else if (ea->sous_etat == ACTION_DEPLACER_DEST) {
                    if (row == ea->orig_ligne && col == ea->orig_col)
                        hl_orange = true;
                    else {
                        bool ptero = (jeu->Joueurs[joueur].a_dino &&
                            jeu->Joueurs[joueur].dino_possede == DINO_PTERODACTYLE);
                        bool adj = case_adjacente(ea->orig_ligne, ea->orig_col, row, col);
                        bool dist2 = ptero &&
                            (abs(ea->orig_ligne - row) + abs(ea->orig_col - col) <= 2);
                        if (adj || dist2) hl_vert = true;
                    }
                }

                if (hl_vert)
                    DrawRectangleRounded((Rectangle) { cx - 4, cy - 4, cs + 8, cs + 8 }, 0.1f, 4,
                        (Color) {
                    42, 180, 42, 60
                });
                if (hl_orange)
                    DrawRectangleRounded((Rectangle) { cx - 4, cy - 4, cs + 8, cs + 8 }, 0.1f, 4,
                        (Color) {
                    240, 140, 0, 80
                });
                if (hl_vert)   bordure = COL_VALIDE;
                if (hl_orange) bordure = COL_ACCENT;
            }

            DrawRectangleRounded((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, fond);
            DrawRectangleRoundedLines((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, bordure);

            int ft = PE(1, GetScreenHeight());
            texte_centre(NOM_CASE[type], cx, cy + cs - ft - PE(1, GetScreenHeight()), cs, ft, lblcol);

            // Pions + dinos
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

                    if (jeu->PlateauPion[row][col][p].TypePion == 2) {
                        // Dino : carré vert au lieu du cercle
                        DrawRectangle(startx + drawn * spacing, cy + cs - pr * 2 - PE(1, GetScreenHeight()),
                            pr * 2, pr * 2, pc);
                        DrawRectangleLines(startx + drawn * spacing, cy + cs - pr * 2 - PE(1, GetScreenHeight()),
                            pr * 2, pr * 2, (Color) { 80, 220, 80, 255 });
                    }
                    else {
                        pion(startx + drawn * spacing + pr,
                            cy + cs - pr - PE(1, GetScreenHeight()), pr, pc);
                    }
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

    DrawRectangle(PX, HDP_H, PY, H - HDP_H, FOND_SOMBRE);
    DrawRectangleLines(PX, HDP_H, PY, H - HDP_H, COL_BORD_LIT);

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
            carte(rx - 4, ry - 3, PY - PE(3, PY), DE_H + 6,
                (Color) {
            13, 26, 13, 255
        }, (Color) { 42, 106, 42, 255 });

        Color fond_de = bloque ? (Color) { 100, 20, 10, 255 } : COULEUR_DE[action];
        DrawRectangleRounded((Rectangle) { rx, ry, face_sz, face_sz }, 0.15f, 6, fond_de);
        DrawRectangleRoundedLines((Rectangle) { rx, ry, face_sz, face_sz }, 0.15f, 6,
            bloque ? RED : BLACK);

        int tx2 = rx + face_sz + PE(1, PY);
        DrawText(NOM_FACE[action], tx2, ry + PE(1, H), Police, COL_TEXT);
        if (bloque)
            DrawText("BLOQUE", tx2, ry + PE(3, H), Police, (Color) { 220, 60, 30, 255 });
        else if (sel)
            DrawText("selectionne", tx2, ry + PE(3, H), Police, (Color) { 42, 180, 42, 255 });
    }

    // Scores
    int sy = HDP_H + PE(58, H);
    DrawLine(PX + PE(2, PY), sy, PX + PY - PE(2, PY), sy, COL_BORD);
    sy += PE(1, H);
    DrawText("SCORES", PX + PE(2, PY), sy, PE(2, H), COL_ACCENT);
    sy += PE(2, H) + PE(1, H);

    for (int i = 0; i < jeu->nb_joueurs; i++) {
        bool actif = (i == joueur);
        int  ry2 = sy + i * (PE(5, H) + PE(1, H));
        carte(PX + PE(2, PY), ry2, PY - PE(4, PY), PE(5, H),
            actif ? (Color) { 22, 15, 6, 255 } : FOND_MID,
            actif ? COL_BORD_LIT : COL_BORD);
        pion(PX + PE(4, PY) + PE(2, H), ry2 + PE(2, H) + 2, PE(2, H), COULEUR_JOUEUR[i]);

        char nom[12];
        snprintf(nom, sizeof(nom), "Joueur %d", i + 1);
        DrawText(nom, PX + PE(4, PY) + PE(5, H), ry2 + (PE(5, H) - PE(2, H)) / 2,
            PE(2, H), actif ? COL_TEXT_BRIGHT : COL_TEXT_DIM);

        char pts[8];
        snprintf(pts, sizeof(pts), "%d", jeu->Joueurs[i].points);
        DrawText(pts, PX + PY - PE(8, PY), ry2 + (PE(5, H) - PE(3, H)) / 2,
            PE(3, H), actif ? COL_ACCENT : COL_TEXT_DIM);

        // Icône dino si le joueur en a un
        if (jeu->Joueurs[i].a_dino) {
            const char* nom_dino[] = { "", "Tric.", "Ptero", "Brach", "Titan" };
            DrawText(nom_dino[jeu->Joueurs[i].dino_possede],
                PX + PE(4, PY) + PE(5, H),
                ry2 + (PE(5, H) - PE(2, H)) / 2 + PE(2, H) + 2,
                PE(2, H) - 2, (Color) { 80, 220, 80, 255 });
        }
    }
}

// =============================================================
//  MENU ACTIONS (avec option Oeuf)
// =============================================================
void dessiner_menu_actions(EtatAction* ea, int nb_oeufs) {
    if (ea->sous_etat != ACTION_MENU) return;

    int bx = PLATEAU_X;
    int by = INFO_Y;
    int h = INFO_H;
    int fw = PE(40, GetScreenWidth());
    int fh = h / 6;
    int ft = PE(2, GetScreenHeight());

    carte(bx, by, fw, h + PE(2, GetScreenHeight()), FOND_SOMBRE, COL_BORD_LIT);
    DrawText("CHOISIR UNE ACTION", bx + PE(2, fw), by + fh / 2, ft - 2, COL_ACCENT_DIM);

    const char* labels[] = { "Caverne", "Hutte", "Déplacement", "Oeuf", "Terminer le tour" };
    const int   faces[] = { 1, 2, 3, 4, -1 };
    char buf[40];

    for (int i = 0; i < 5; i++) {
        int  ry = by + fh + i * (fh + 2);
        bool sel = (ea->selection_menu == i);
        bool dispo = (faces[i] == -1) || (ea->nb_actions[faces[i]] > 0);

        Color fond_l = sel ? (Color) { 30, 22, 10, 255 } : FOND_MID;
        Color bord_l = sel ? COL_ACCENT : COL_BORD;
        Color txt_col = dispo ? (sel ? COL_TEXT_BRIGHT : COL_TEXT) : COL_TEXT_DIM;

        carte(bx + PE(50, fw), ry, fw - PE(2, fw), fh, fond_l, bord_l);
        if (sel) DrawText(">", bx + PE(2, fw), ry + (fh - ft) / 2, ft, COL_ACCENT);

        if (i == 3)
            snprintf(buf, sizeof(buf), "Oeufs  (%d dispo, min 2)", nb_oeufs);
        else if (faces[i] != -1)
            snprintf(buf, sizeof(buf), "%s  (%d restant%s)",
                labels[i], ea->nb_actions[faces[i]],
                ea->nb_actions[faces[i]] > 1 ? "s" : "");
        else
            snprintf(buf, sizeof(buf), "%s", labels[i]);

        DrawText(buf, bx + PE(5, fw), ry + (fh - ft) / 2, ft, txt_col);
    }

    int help_y = by + h - ft - 4;
    DrawText("Haut/Bas : naviguer   Entree : valider   T : terminer",
        bx + PE(2, fw), help_y, ft - 2, COL_TEXT_DIM);
}

// =============================================================
//  MENU CHOIX DINO (éclosion d'oeuf)
// =============================================================
static void dessiner_menu_dino(EtatAction* ea) {
    if (ea->sous_etat != ACTION_OEUF_CHOISIR) return;

    int bx = PLATEAU_X;
    int by = INFO_Y;
    int h = INFO_H;
    int fw = PE(40, GetScreenWidth());
    int fh = h / 5;
    int ft = PE(2, GetScreenHeight());

    carte(bx, by, fw, h, FOND_SOMBRE, COL_BORD_LIT);
    DrawText("CHOISIR UN DINOSAURE", bx + PE(2, fw), by + fh / 2, ft - 2, COL_ACCENT_DIM);

    const char* noms[] = { "Triceratops  (+1 de)", "Pterodactyle (+2 cases)", "Brachiosaurus (partout)", "Titanosaure  (+1 de)" };
    for (int i = 0; i < 4; i++) {
        int  ry = by + fh + i * (fh + 2);
        bool sel = (ea->selection_dino == i);
        carte(bx + PE(1, fw), ry, fw - PE(2, fw), fh,
            sel ? (Color) { 13, 26, 13, 255 } : FOND_MID,
            sel ? COL_VALIDE : COL_BORD);
        if (sel) DrawText(">", bx + PE(2, fw), ry + (fh - ft) / 2, ft, COL_VALIDE);
        DrawText(noms[i], bx + PE(5, fw), ry + (fh - ft) / 2, ft,
            sel ? COL_TEXT_BRIGHT : COL_TEXT);
    }
    DrawText("Haut/Bas : choisir   Entree : confirmer",
        bx + PE(2, fw), by + h - ft - 4, ft - 2, COL_TEXT_DIM);
}

// =============================================================
//  INFO STRIP
// =============================================================
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer) {
    const char* phases[] = { "Lancer des des","Actions des des","Eruption !","Fin de tour" };
    int bx = PLATEAU_X;
    int y = INFO_Y;
    int h = INFO_H;
    int cw = INFO_BOITE_L;
    int gap = PE(1, GetScreenWidth());
    int flab = PE(2, GetScreenHeight());
    int fval = PE(4, GetScreenHeight());

    carte(bx, y, cw, h, FOND_SOMBRE, COL_BORD);
    DrawText("PHASE", bx + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    DrawText(phases[etape], bx + PE(2, cw), y + PE(5, h) + flab + PE(2, h), flab, COL_TEXT_BRIGHT);
    char tour[24]; snprintf(tour, sizeof(tour), "Tour du joueur %d", joueur + 1);
    DrawText(tour, bx + PE(2, cw), y + h - flab - PE(5, h), flab - 2, COL_TEXT_DIM);

    int bx2 = bx + cw + gap;
    carte(bx2, y, cw, h, FOND_SOMBRE, COL_BORD);
    DrawText("LANCERS", bx2 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char lstr[4]; snprintf(lstr, sizeof(lstr), "%d", nblancer);
    int lw = MeasureText(lstr, fval);
    DrawText(lstr, bx2 + (cw - lw) / 2, y + h / 2 - fval / 2, fval, COL_ACCENT);
    DrawText("/ 3", bx2 + (cw + lw) / 2 + 4, y + h / 2 - flab / 2, flab, COL_TEXT_DIM);

    int bx3 = bx + 2 * (cw + gap);
    carte(bx3, y, cw, h, FOND_SOMBRE, COL_BORD);
    DrawText("RESERVE", bx3 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char rstr[4]; snprintf(rstr, sizeof(rstr), "%d", jeu->Joueurs[joueur].reserve);
    int rw = MeasureText(rstr, fval);
    DrawText(rstr, bx3 + (cw - rw) / 2, y + h / 2 - fval / 2, fval, COL_TEXT_BRIGHT);

    // Seuil de victoire
    int bx4 = bx + 3 * (cw + gap);
    carte(bx4, y, cw, h, FOND_SOMBRE, COL_BORD);
    DrawText("SEUIL", bx4 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char sstr[8]; snprintf(sstr, sizeof(sstr), "%d pts", seuil_victoire(jeu->nb_joueurs));
    DrawText(sstr, bx4 + PE(2, cw), y + h / 2 - flab / 2, flab, COL_TEXT_DIM);
}

// =============================================================
//  STATUS BAR
// =============================================================
void dessiner_statusbar(int etape, bool achoisides) {
    int sy = STATUS_Y;
    int sw = GetScreenWidth();
    int sh = GetScreenHeight() - sy;
    DrawRectangle(0, sy, sw, sh, FOND_SOMBRE);
    DrawLine(0, sy, sw, sy, COL_BORD_LIT);

    int fy = sy + (sh - PE(2, GetScreenHeight())) / 2;
    int ftaille = PE(2, GetScreenHeight());
    const char* phases[] = { "Lancer des des","Actions","Eruption","Fin de tour" };
    int idx = etape < 4 ? etape : 0;
    DrawText(phases[idx], PE(2, sw), fy, ftaille, COL_ACCENT);
    DrawLine(PE(14, sw), sy + PE(15, sh), PE(14, sw), sy + PE(85, sh), COL_BORD);

    const char* msg = "";
    if (etape == 0)
        msg = achoisides
        ? "ESPACE pour lancer les des"
        : "Clic pour bloquer/debloquer un de  |  ENTREE pour valider";
    else if (etape == 1)
        msg = "Haut/Bas + ENTREE pour choisir  |  Clic plateau pour placer/deplacer  |  T pour terminer";
    else if (etape == 2)
        msg = "ESPACE/ENTREE : voir les scores  |  Fleches : deplacer le volcan  |  ENTREE : confirmer";
    DrawText(msg, PE(15, sw), fy, ftaille, COL_TEXT_DIM);
}

// =============================================================
//  ÉCRAN ÉRUPTION
// =============================================================
void affichage_eruption(Jeu* jeu, PhaseEruption* pe, int joueur) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, NULL, joueur);
    dessiner_panel_des(jeu, joueur);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int bw = PE(44, sw);
    int bh = PE(22, sh);
    int bx = PLATEAU_X;
    int by = INFO_Y;

    if (pe->phase == ERUPTION_VOLCAN_SCORE) {
        carte(bx, by, bw, bh, FOND_SOMBRE, COL_ACCENT);
        texte_centre("ERUPTION !", bx, by + PE(8, bh), bw, PE(4, sh), COL_ACCENT);

        // Points volcan
        int vl = trouver_volcan_ligne(jeu);
        int vc = trouver_volcan_col(jeu);
        int totaux[4] = { 0 };
        if (vl >= 0)
            for (int p = 0; p < 10; p++)
                if (jeu->PlateauPion[vl][vc][p].TypePion != 0)
                    totaux[jeu->PlateauPion[vl][vc][p].joueur]++;

        char buf[48];
        int ty = by + PE(35, bh);
        int ft = PE(2, sh);
        for (int i = 0; i < jeu->nb_joueurs; i++) {
            if (totaux[i] > 0) {
                snprintf(buf, sizeof(buf), "J%d : %d pion(s) sur le volcan", i + 1, totaux[i]);
                DrawText(buf, bx + PE(3, bw), ty, ft, COULEUR_JOUEUR[i]);
                ty += ft + 4;
            }
        }
        texte_centre("ESPACE ou ENTREE pour continuer", bx, by + PE(80, bh), bw, ft - 2, COL_TEXT_DIM);
    }
    else if (pe->phase == ERUPTION_DEPLACEMENT) {
        carte(bx, by, bw, bh, FOND_SOMBRE, COL_BORD_LIT);
        texte_centre("DEPLACER LE VOLCAN", bx, by + PE(8, bh), bw, PE(3, sh), COL_ACCENT);

        char buf[32];
        snprintf(buf, sizeof(buf), "Deplacements restants : %d", pe->moves_restants);
        int ft = PE(2, sh);
        DrawText(buf, bx + PE(3, bw), by + PE(45, bh), ft, COL_TEXT);
        texte_centre("Fleches pour deplacer  |  ENTREE pour valider",
            bx, by + PE(78, bh), bw, ft - 2, COL_TEXT_DIM);
    }

    dessiner_statusbar(2, false);
}

// =============================================================
//  ÉCRAN DE FIN
// =============================================================
void affichage_fin(Jeu* jeu, int gagnant) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    DrawRectangle(0, 0, sw, sh, (Color) { 0, 0, 0, 160 });

    int bw = PE(50, sw), bh = PE(50, sh);
    int bx = (sw - bw) / 2, by = (sh - bh) / 2;
    carte(bx, by, bw, bh, FOND_SOMBRE, COL_BORD_LIT);

    int ft_big = PE(5, sh), ft_med = PE(3, sh), ft_sml = PE(2, sh);
    texte_centre("FIN DE PARTIE", bx, by + PE(5, bh), bw, ft_big, COL_ACCENT);

    if (gagnant >= 0) {
        char msg[40];
        snprintf(msg, sizeof(msg), "Joueur %d remporte la partie !", gagnant + 1);
        texte_centre(msg, bx, by + PE(28, bh), bw, ft_med, COULEUR_JOUEUR[gagnant]);
    }

    DrawText("Scores finaux :", bx + PE(10, bw), by + PE(45, bh), ft_sml, COL_TEXT_DIM);
    for (int i = 0; i < jeu->nb_joueurs; i++) {
        char sc[32];
        snprintf(sc, sizeof(sc), "Joueur %d : %d pts  (seuil : %d)",
            i + 1, jeu->Joueurs[i].points, seuil_victoire(jeu->nb_joueurs));
        DrawText(sc, bx + PE(10, bw), by + PE(52, bh) + i * (ft_sml + 4),
            ft_sml, i == gagnant ? COL_ACCENT : COL_TEXT_DIM);
    }
    texte_centre("ECHAP pour quitter", bx, by + PE(88, bh), bw, ft_sml - 2, COL_TEXT_DIM);
}

// =============================================================
//  RENDU COMPLET – étape 0
// =============================================================
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, NULL, joueur);
    dessiner_panel_des(jeu, joueur);
    dessiner_info_strip(jeu, joueur, etape, nblancer);
    dessiner_statusbar(etape, achoisides);
}

// =============================================================
//  RENDU COMPLET – étape 1
// =============================================================
void affichage_actions(Jeu* jeu, int joueur, EtatAction* ea) {
    ClearBackground(FOND);
    Dessiner_HDP(jeu, joueur);
    afficherplateau(jeu, ea, joueur);
    dessiner_panel_des(jeu, joueur);
    if (ea->sous_etat == ACTION_OEUF_CHOISIR)
        dessiner_menu_dino(ea);
    else
        dessiner_menu_actions(ea, jeu->Joueurs[joueur].oeufs);
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
}