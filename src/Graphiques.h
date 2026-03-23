#define BG_RGB(r, g, b) "\x1b[48;2;" #r ";" #g ";" #b "m"
#define FG_RGB(r, g, b) "\x1b[38;2;" #r ";" #g ";" #b "m"
#define RESET           "\x1b[0m"

void positionner_curseur(int ligne, int colonne);
void plein_ecran();
void initialiser_rendu();