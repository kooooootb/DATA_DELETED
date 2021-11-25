#ifndef LAB4_PARAMETERS_H
#define LAB4_PARAMETERS_H

/*!
* \file
* \brief Заголовочный файл с глобальными параметрами
*/

#include "AllClasses.h"

const int WEIGHTLARGE = 3, WEIGHTMEDIUM = 2, WEIGHTSMALL = 1;//bullets' weight
const int AMOUNTLARGE = 1, AMOUNTMEDIUM = 5, AMOUNTSMALL = 4;//bullets shot at 1 function's call

//map parameters for graphic window
const int CELLSIZE = 30;//even
const int WINDOWWIDTH_AMOUNT = 61;//odd
const int WINDOWHEIGHT_AMOUNT = 31;//odd
//const int CELLSIZE = 2;//even
//const int WINDOWWIDTH_AMOUNT = 181;//odd
//const int WINDOWHEIGHT_AMOUNT = 101;//odd
//const unsigned int WINDOWWIDTH_AMOUNT = 3;//odd
//const unsigned int WINDOWHEIGHT_AMOUNT = 3;//odd
const int WINDOWWIDTH = CELLSIZE * WINDOWWIDTH_AMOUNT;
const int WINDOWHEIGHT = CELLSIZE * WINDOWHEIGHT_AMOUNT;
const int NORM = -1 * CELLSIZE / 2;
const int XOFFSET = WINDOWWIDTH_AMOUNT / 2;
const int YOFFSET = WINDOWHEIGHT_AMOUNT / 2;
const int CELLSIZEX = CELLSIZE;
const int CELLSIZEY = CELLSIZE;

//messages parameters
const unsigned int FONTSIZE = 20;
const unsigned int CHOOSEOFFSET = FONTSIZE * 5;

//config files' names
static const char * OPERS_CFG = "Opers.cfg";
static const char * SENTS_CFG = "Sents.cfg";
static const char * WILDS_CFG = "Wilds.cfg";
static const char * FORAGS_CFG = "Forags.cfg";
//static const char * CELLS_CFG = "Cells.cfg";
static const char * CELLS_CFG = "c1.cfg";

static const char * HKITS_CFG = "HKits.cfg";
static const char * ACONTS_CFG = "AConts.cfg";
static const char * GUNS_CFG = "Guns.cfg";

//Textures
#define TEXTDIR "Textures/"
static const char * CREATURES_TEXTURES[CREATURES_COUNT] = { TEXTDIR"OperativeTexture.png" , TEXTDIR"SentientTexture.png" , TEXTDIR"WildTexture.png" , TEXTDIR"ForagerTexture.png" };

static const char * ITEMS_TEXTURES[ITEMS_COUNT] = { TEXTDIR"HealthKitTexture.png" , TEXTDIR"AContTexture.png" , TEXTDIR"GunTexture.png" };

static const char * CELLS_TEXTURES[CELLSTYPE_COUNT] = { TEXTDIR"FloorTexture.png" , TEXTDIR"WallTexture.png" , TEXTDIR"GlassTexture.png" , TEXTDIR"PartitionTexture.png" , TEXTDIR"StorageTexture.png" };

#endif //LAB4_PARAMETERS_H
