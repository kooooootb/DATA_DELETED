#ifndef LAB4_PARAMETERS_H
#define LAB4_PARAMETERS_H

const int WEIGHTLARGE = 10, WEIGHTMEDIUM = 5, WEIGHTSMALL = 2;//bullets' weight
const int AMOUNTLARGE = 1, AMOUNTMEDIUM = 5, AMOUNTSMALL = 4;//bullets shot at 1 function's call

//map parameters for graphic window
const int CELLSIZE = 20;//even
const int WINDOWWIDTH_AMOUNT = 93;//odd, crashes if WINDOWWIDTH > 1920
const int WINDOWHEIGHT_AMOUNT = 45;//odd, crashes if WINDOWHEIGHT > 1080
//const unsigned int WINDOWWIDTH_AMOUNT = 3;//odd, crashes if WINDOWWIDTH > 1920
//const unsigned int WINDOWHEIGHT_AMOUNT = 3;//odd, crashes if WINDOWHEIGHT > 1080
const int WINDOWWIDTH = CELLSIZE * WINDOWWIDTH_AMOUNT;
const int WINDOWHEIGHT = CELLSIZE * WINDOWHEIGHT_AMOUNT;
const int NORM = -1 * CELLSIZE / 2;
const int XOFFSET = WINDOWWIDTH_AMOUNT / 2;
const int YOFFSET = WINDOWHEIGHT_AMOUNT / 2;

//messages parameters
const unsigned int FONTSIZE = 3 * CELLSIZE / 4;
const unsigned int CHOOSEOFFSET = FONTSIZE * 5;

//config files' names
static const char * OPERS_CFG = "Opers.cfg";
static const char * SENTS_CFG = "Sents.cfg";
static const char * WILDS_CFG = "Wilds.cfg";
static const char * FORAGS_CFG = "Forags.cfg";
static const char * CELLS_CFG = "Cells.cfg";

static const char * HKITS_CFG = "HKits.cfg";
static const char * ACONTS_CFG = "AConts.cfg";
static const char * GUNS_CFG = "Guns.cfg";

#endif //LAB4_PARAMETERS_H
