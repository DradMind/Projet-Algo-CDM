#include "Graphiques.h"
#include "Logique.h"
#include <stdio.h>

// =============================================================
//  Graphiques.c  –  Rendu complet style mockup
// =============================================================

// -------------------------------------------------------------
//  PE  –  helper pourcentage (défini ici, déclaré dans .h)
// -------------------------------------------------------------
int PE(float pourcentage, float valeur) {
    return (int)(valeur / 100.0f * pourcentage);
}

// -------------------------------------------------------------
//  Utilitaires internes
// -------------------------------------------------------------

// Dessine un rectangle arrondi avec fond + bordure
static void carte(int x, int y, int w, int h, Color fond, Color bordure) {
    DrawRectangleRounded((Rectangle) { x, y, w, h }, 0.08f, 6, fond);
    DrawRectangleRoundedLines((Rectangle) { x, y, w, h }, 0.08f, 6, bordure);
}

// Texte centré horizontalement dans une zone
static void texte_centre(const char* txt, int x, int y, int w, int taille, Color col) {
    int tw = MeasureText(txt, taille);
    DrawText(txt, x + (w - tw) / 2, y, taille, col);
}

// Pion (cercle avec contour)
static void pion(int cx, int cy, int r, Color couleur) {
    DrawCircle(cx, cy, (float)r, couleur);
    DrawCircleLines((float)cx, (float)cy, (float)(r + 1), (Color) { 0, 0, 0, 160 });
}

// Icône géométrique d'une case (pas d'emoji, formes raylib)
static void icone_case(int type, int cx, int cy, int sz) {
    switch (type) {
    case 0: // Volcan : triangle + point chaud
        DrawTriangle(
            (Vector2) {
            cx, cy - sz / 2
        },
            (Vector2) {
            cx - sz / 2, cy + sz / 2
        },
            (Vector2) {
            cx + sz / 2, cy + sz / 2
        },
            (Color) {
            180, 40, 0, 180
        }
        );
        DrawCircle(cx, cy - sz / 2 + sz / 5, (float)(sz / 6), (Color) { 255, 160, 0, 220 });
        break;
    case 1: // Eau : deux vagues
        DrawRectangle(cx - sz / 2, cy - sz / 6, sz, sz / 4, (Color) { 30, 144, 255, 160 });
        DrawRectangle(cx - sz / 3, cy + sz / 6, sz * 2 / 3, sz / 5, (Color) { 30, 144, 255, 100 });
        break;
    case 2: // Jungle : arbre (triangle + tronc)
        DrawTriangle(
            (Vector2) {
            cx, cy - sz / 2
        },
            (Vector2) {
            cx - sz / 2, cy + sz / 4
        },
            (Vector2) {
            cx + sz / 2, cy + sz / 4
        },
            (Color) {
            34, 139, 34, 180
        }
        );
        DrawRectangle(cx - sz / 8, cy + sz / 4, sz / 4, sz / 4, (Color) { 100, 60, 20, 180 });
        break;
    case 3: // Prairie : touffes d'herbe
        for (int i = -1; i <= 1; i++)
            DrawRectangle(cx + i * (sz / 3) - sz / 10, cy - sz / 4, sz / 5, sz / 2,
                (Color) {
            100, 200, 30, 180
        });
        break;
    case 4: // Hutte : murs + toit
        DrawRectangle(cx - sz / 3, cy, sz * 2 / 3, sz / 3, (Color) { 160, 90, 30, 180 });
        DrawTriangle(
            (Vector2) {
            cx, cy - sz / 6
        },
            (Vector2) {
            cx - sz / 2 + 4, cy + sz / 8
        },
            (Vector2) {
            cx + sz / 2 - 4, cy + sz / 8
        },
            (Color) {
            120, 60, 20, 180
        }
        );
        break;
    case 5: // Caverne : arche sombre
        DrawCircle(cx, cy + sz / 4, (float)(sz / 3), (Color) { 15, 15, 15, 200 });
        DrawRectangle(cx - sz / 3, cy + sz / 4, sz * 2 / 3, sz / 4, (Color) { 35, 35, 35, 160 });
        break;
    }
}

// -------------------------------------------------------------
//  TOPBAR
// -------------------------------------------------------------
void dessiner_topbar(Jeu* jeu, int joueur) {
    int w = GetScreenWidth();
    int h = TOPBAR_H;

    DrawRectangle(0, 0, w, h, COL_BG_DARK);
    DrawLine(0, h, w, h, COL_BORDER_LIT);

    // Titre
    int ftaille = PE(3, GetScreenHeight());
    DrawText("COLERE DE LA MONTAGNE DE FEU",
        PE(2, w), (h - ftaille) / 2, ftaille, COL_ACCENT);

    // Chips joueurs (droite)
    int r = PE(2, GetScreenHeight());
    int chip_x = PE(80, w);
    int chip_cy = h / 2;
    for (int i = 0; i < 4; i++) {
        Color c = COULEUR_JOUEUR[i];
        if (i != joueur) c.a = 70;
        int cx = chip_x + i * (r * 2 + PE(1, w));
        DrawCircle(cx, chip_cy, (float)r, c);
        if (i == joueur)
            DrawCircleLines((float)cx, (float)chip_cy, (float)(r + 2), COL_TEXT_BRIGHT);
        char lbl[3];
        snprintf(lbl, sizeof(lbl), "J%d", i + 1);
        int lw = MeasureText(lbl, PE(2, GetScreenHeight()));
        DrawText(lbl, cx - lw / 2, chip_cy - PE(1, GetScreenHeight()),
            PE(2, GetScreenHeight()),
            (Color) {
            255, 255, 255, i == joueur ? 220 : 100
        });
    }
}

// -------------------------------------------------------------
//  PLATEAU 4×4
// -------------------------------------------------------------
void afficherplateau(Jeu* jeu) {
    int bx = PLATEAU_X;
    int by = PLATEAU_Y;
    int cs = CASE;
    int gap = CASE_GAP;

    // Fond du plateau
    int total = 4 * cs + 3 * gap;
    DrawRectangleRounded((Rectangle) { bx - 8, by - 8, total + 16, total + 16 },
        0.04f, 4, (Color) { 12, 8, 3, 255 });
    DrawRectangleRoundedLines((Rectangle) { bx - 8, by - 8, total + 16, total + 16 },
        0.04f, 4, COL_BORDER);

    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            int type = jeu->plateau[row][col].TypeCase;
            int cx = bx + col * (cs + gap);
            int cy = by + row * (cs + gap);

            Color fond = (type >= 0 && type < 6) ? COULEUR_CASE[type] : COL_BG_MID;
            Color bordure = (type >= 0 && type < 6) ? COULEUR_CASE_BORDER[type] : COL_BORDER;
            Color txtcol = (type >= 0 && type < 6) ? COULEUR_CASE_LABEL[type] : COL_TEXT_DIM;

            // Halo pour le volcan
            if (type == 0)
                DrawRectangleRounded((Rectangle) { cx - 3, cy - 3, cs + 6, cs + 6 },
                    0.1f, 4, (Color) { 255, 60, 0, 40 });

            DrawRectangleRounded((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, fond);
            DrawRectangleRoundedLines((Rectangle) { cx, cy, cs, cs }, 0.1f, 8, bordure);

            // Icône au centre
            icone_case(type, cx + cs / 2, cy + cs / 2 - PE(1, GetScreenHeight()),
                cs / 3);

            // Nom de la case
            int ftaille = PE(1, GetScreenHeight());
            texte_centre(NOM_CASE[type], cx, cy + cs - ftaille - PE(1, GetScreenHeight()),
                cs, ftaille, txtcol);

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
                    pion(startx + drawn * spacing + pr,
                        cy + cs - pr - PE(1, GetScreenHeight()),
                        pr, pc);
                    drawn++;
                }
            }
        }
    }
}

// -------------------------------------------------------------
//  PANNEAU DÉS
// -------------------------------------------------------------
void dessiner_panel_des(Jeu* jeu, int joueur) {
    int px = PANEL_X;
    int pw = PANEL_W;
    int sh = GetScreenHeight();

    // Fond
    DrawRectangle(px, TOPBAR_H, pw, sh - TOPBAR_H, COL_BG_DARK);
    DrawLine(px, TOPBAR_H, px, sh, COL_BORDER_LIT);

    int y = TOPBAR_H + PE(2, sh);

    // --- Titre ---
    int ftitle = PE(2, sh);
    DrawText("DES", px + PE(2, pw), y, ftitle, COL_ACCENT);
    y += ftitle + PE(1, sh);
    DrawLine(px + PE(2, pw), y, px + pw - PE(2, pw), y, COL_BORDER);
    y += PE(1, sh);

    // --- Dés ---
    int nb_des = 5 + possedetitanosaure(jeu, joueur);
    int face_sz = PE(5, sh);
    int row_h = DIE_H;
    int fsmall = PE(2, sh);

    for (int i = 0; i < nb_des; i++) {
        int action = jeu->Listede[i].action;
        bool lock = jeu->Listede[i].bloque;
        bool sel = jeu->Listede[i].selectionne;

        int rx = TABLEAU_X;
        int ry = TABLEAU_Y + i * (row_h + PE(1, sh));

        // Fond ligne sélectionnée
        if (sel)
            carte(rx - 4, ry - 3, pw - PE(3, pw), row_h + 6,
                (Color) {
            13, 26, 13, 255
        }, (Color) { 42, 106, 42, 255 });

        // Dé bloqué : fond rouge, sinon couleur de la face
        Color fond_de = lock ? (Color) { 100, 20, 10, 255 }
        : COULEUR_DE[action];
        DrawRectangleRounded((Rectangle) { rx, ry, face_sz, face_sz },
            0.15f, 6, fond_de);
        DrawRectangleRoundedLines((Rectangle) { rx, ry, face_sz, face_sz },
            0.15f, 6, lock ? RED : BLACK);

        // Sigle court
        const char* sigles[] = { "VOL","CAV","HUT","EMP","OEU","DEP" };
        int sw = MeasureText(sigles[action], fsmall - 2);
        DrawText(sigles[action],
            rx + (face_sz - sw) / 2,
            ry + (face_sz - (fsmall - 2)) / 2,
            fsmall - 2,
            (Color) {
            255, 255, 255, 200
        });

        // Nom + état
        int tx = rx + face_sz + PE(1, pw);
        DrawText(NOM_FACE[action], tx, ry + PE(1, sh), fsmall, COL_TEXT);

        if (lock)
            DrawText("BLOQUE", tx, ry + PE(1, sh) + fsmall + 2,
                fsmall - 2, (Color) { 220, 60, 30, 255 });
        else if (sel)
            DrawText("selectionne", tx, ry + PE(1, sh) + fsmall + 2,
                fsmall - 2, (Color) { 42, 180, 42, 255 });
    }

    // --- Scores ---
    int sy = TOPBAR_H + PE(58, sh);
    DrawLine(px + PE(2, pw), sy, px + pw - PE(2, pw), sy, COL_BORDER);
    sy += PE(1, sh);
    DrawText("SCORES", px + PE(2, pw), sy, PE(2, sh), COL_ACCENT);
    sy += PE(2, sh) + PE(1, sh);

    for (int i = 0; i < 4; i++) {
        bool actif = (i == joueur);
        int ry = sy + i * (PE(5, sh) + PE(1, sh));
        Color fond_s = actif ? (Color) { 22, 15, 6, 255 } : COL_BG_MID;
        Color bord_s = actif ? COL_BORDER_LIT : COL_BORDER;
        carte(px + PE(2, pw), ry, pw - PE(4, pw), PE(5, sh), fond_s, bord_s);

        pion(px + PE(4, pw) + PE(2, sh),
            ry + PE(2, sh) + 2,
            PE(2, sh), COULEUR_JOUEUR[i]);

        char nom[12];
        snprintf(nom, sizeof(nom), "Joueur %d", i + 1);
        DrawText(nom,
            px + PE(4, pw) + PE(5, sh),
            ry + (PE(5, sh) - PE(2, sh)) / 2,
            PE(2, sh),
            actif ? COL_TEXT_BRIGHT : COL_TEXT_DIM);

        char pts[8];
        snprintf(pts, sizeof(pts), "%d", jeu->Joueurs[i][0].points);
        int ptw = MeasureText(pts, PE(3, sh));
        DrawText(pts,
            px + pw - PE(4, pw) - ptw,
            ry + (PE(5, sh) - PE(3, sh)) / 2,
            PE(3, sh),
            actif ? COL_ACCENT : COL_TEXT_DIM);
    }
}

// -------------------------------------------------------------
//  INFO STRIP (bas gauche)
// -------------------------------------------------------------
void dessiner_info_strip(Jeu* jeu, int joueur, int etape, int nblancer) {
    const char* phases[] = {
        "Lancer des des", "Actions des des", "Seismes", "Fin de tour"
    };

    int bx = PLATEAU_X;
    int y = INFO_Y;
    int h = INFO_H;
    int cw = INFO_CARD_W;
    int gap = PE(1, GetScreenWidth());
    int flab = PE(2, GetScreenHeight());
    int fval = PE(4, GetScreenHeight());

    // Carte 1 : Phase
    carte(bx, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("PHASE", bx + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    DrawText(phases[etape], bx + PE(2, cw), y + PE(5, h) + flab + PE(2, h), flab, COL_TEXT_BRIGHT);
    char tour[24];
    snprintf(tour, sizeof(tour), "Tour du joueur %d", joueur + 1);
    DrawText(tour, bx + PE(2, cw), y + h - flab - PE(5, h), flab - 2, COL_TEXT_DIM);

    // Carte 2 : Lancers restants
    int bx2 = bx + cw + gap;
    carte(bx2, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("LANCERS", bx2 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char lstr[4];
    snprintf(lstr, sizeof(lstr), "%d", nblancer);
    int lw = MeasureText(lstr, fval);
    DrawText(lstr, bx2 + (cw - lw) / 2, y + h / 2 - fval / 2, fval, COL_ACCENT);
    DrawText("/ 3", bx2 + (cw + lw) / 2 + 4, y + h / 2 - flab / 2, flab, COL_TEXT_DIM);

    // Carte 3 : Oeufs
    int bx3 = bx + 2 * (cw + gap);
    int oeufs = jeu->Joueurs[joueur][0].dino;
    carte(bx3, y, cw, h, COL_BG_DARK, COL_BORDER);
    DrawText("OEUFS", bx3 + PE(2, cw), y + PE(5, h), flab - 2, COL_ACCENT_DIM);
    char ostr[4];
    snprintf(ostr, sizeof(ostr), "%d", oeufs);
    int ow = MeasureText(ostr, fval);
    DrawText(ostr, bx3 + (cw - ow) / 2, y + h / 2 - fval / 2, fval, COL_TEXT_BRIGHT);
    // Petits cercles oeufs
    for (int i = 0; i < oeufs && i < 5; i++)
        DrawCircle(bx3 + PE(5, cw) + i * PE(5, cw),
            y + h - PE(8, h),
            PE(3, h),
            (Color) {
        200, 200, 255, 180
    });
}

// -------------------------------------------------------------
//  STATUS BAR
// -------------------------------------------------------------
void dessiner_statusbar(int etape, bool achoisides) {
    int sy = STATUS_Y;
    int sw = GetScreenWidth();
    int sh = GetScreenHeight() - sy;

    DrawRectangle(0, sy, sw, sh, COL_BG_DARK);
    DrawLine(0, sy, sw, sy, COL_BORDER_LIT);

    int fy = sy + (sh - PE(2, GetScreenHeight())) / 2;
    int ftaille = PE(2, GetScreenHeight());

    // Phase courante
    const char* phases[] = {
        "Lancer des des", "Actions", "Seismes", "Fin de tour"
    };
    DrawText(phases[etape], PE(2, sw), fy, ftaille, COL_ACCENT);
    DrawLine(PE(14, sw), sy + PE(15, sh), PE(14, sw), sy + PE(85, sh), COL_BORDER);

    // Message contextuel
    const char* msg = "";
    if (etape == 0)
        msg = achoisides ? "ESPACE pour lancer les des"
        : "Cliquer les des a garder (clic droit = bloquer)  |  ENTREE pour valider";
    else if (etape == 1)
        msg = "Choisir une action dans le panneau  |  T pour terminer le tour";

    DrawText(msg, PE(15, sw), fy, ftaille, COL_TEXT_DIM);

    // Raccourcis (droite)
    const char* keys[] = { "ESPACE", "CLIC", "ENTREE" };
    int kx = PE(78, sw);
    for (int i = 0; i < 3; i++) {
        int kw = MeasureText(keys[i], ftaille - 2) + PE(1, sw);
        DrawRectangle(kx, fy - 2, kw, ftaille + 4, COL_BG_MID);
        DrawRectangleLines(kx, fy - 2, kw, ftaille + 4, COL_BORDER);
        DrawText(keys[i], kx + PE(1, sw) / 2, fy, ftaille - 2, COL_ACCENT_DIM);
        kx += kw + PE(1, sw);
    }
}

// -------------------------------------------------------------
//  RENDU COMPLET  –  appel unique depuis main
// -------------------------------------------------------------
void affichage_jeu(Jeu* jeu, int joueur, int etape, int nblancer, bool achoisides) {
    ClearBackground(COL_BG);

    dessiner_topbar(jeu, joueur);
    afficherplateau(jeu);
    dessiner_panel_des(jeu, joueur);
    dessiner_info_strip(jeu, joueur, etape, nblancer);
    dessiner_statusbar(etape, achoisides);
}

// -------------------------------------------------------------
//  SELECTION DÉS À LA SOURIS
// -------------------------------------------------------------
bool selectiondes(Jeu* jeu, int joueur, Vector2 pos_souris, bool blocage) {
    for (int i = 0; i < 5 + possedetitanosaure(jeu, joueur); i++) {
        int x = TABLEAU_X;
        int y = TABLEAU_Y + i * (DIE_H + PE(1, GetScreenHeight()));
        Rectangle rect = { (float)x, (float)y, (float)DIE_H, (float)DIE_H };

        if (CheckCollisionPointRec(pos_souris, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (blocage)
                jeu->Listede[i].bloque = !jeu->Listede[i].bloque;
            else
                jeu->Listede[i].selectionne = !jeu->Listede[i].selectionne;
            return true;
        }
    }
    return false;
}

// -------------------------------------------------------------
//  FENETRE
// -------------------------------------------------------------
void initialiser_fenetre(void) {
    InitWindow(1, 1, "");

    int ecran = GetCurrentMonitor();
    int larg = GetMonitorWidth(ecran);
    int haut = GetMonitorHeight(ecran);

    int larg_voulu = (larg > 1920) ? 2560 : 1280;
    int haut_voulu = (haut > 1080) ? 1440 : 720;

    SetWindowSize(larg_voulu, haut_voulu);
    SetWindowTitle("Colere de la Montagne de Feu");
    SetTargetFPS(60);
}