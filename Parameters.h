#ifndef LAB4_PARAMETERS_H
#define LAB4_PARAMETERS_H

/*!
* \file
* \brief Заголовочный файл с глобальными параметрами
*/

#include "AllClasses.h"

const int WEIGHTLARGE = 3, WEIGHTMEDIUM = 2, WEIGHTSMALL = 1;//bullets' weight

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
static const char * ITEMS_CFG = "Items.cfg";

//Textures
#define TEXTURE_DIRECTORY "Textures/"
static const char * CREATURES_TEXTURES[CREATURES_COUNT] = { TEXTURE_DIRECTORY"OperativeTexture.png" , TEXTURE_DIRECTORY"SentientTexture.png" , TEXTURE_DIRECTORY"WildTexture.png" , TEXTURE_DIRECTORY"ForagerTexture.png" };

static const char * ITEMS_TEXTURES[ITEMS_COUNT] = { TEXTURE_DIRECTORY"HealthKitTexture.png" , TEXTURE_DIRECTORY"AContTexture.png" , TEXTURE_DIRECTORY"GunTexture.png" };

static const char * CELLS_TEXTURES[CELLSTYPE_COUNT] = { TEXTURE_DIRECTORY"FloorTexture.png" , TEXTURE_DIRECTORY"WallTexture.png" , TEXTURE_DIRECTORY"GlassTexture.png" , TEXTURE_DIRECTORY"PartitionTexture.png" , TEXTURE_DIRECTORY"StorageTexture.png" };

#endif //LAB4_PARAMETERS_H
