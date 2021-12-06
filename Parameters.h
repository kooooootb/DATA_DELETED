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
//const int CELLSIZE = 60;//even
//const int WINDOWWIDTH_AMOUNT = 31;//odd
//const int WINDOWHEIGHT_AMOUNT = 16;//odd
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
static const char * CREATURES_CFG = "Creatures.cfg";
static const char * CELLS_CFG = "Cells.cfg";
//static const char * CELLS_CFG = "c1.cfg";
static const char * ITEMS_CFG = "Items.cfg";
static const char * PRESETS_CFG = "presets.cfg";

//Textures
#define TEXTURE_DIRECTORY "Textures/"
static const char * CREATURES_TEXTURES[] = { TEXTURE_DIRECTORY"OperativeTexture.png" , TEXTURE_DIRECTORY"SentientTexture.png" , TEXTURE_DIRECTORY"WildTexture.png" , TEXTURE_DIRECTORY"ForagerTexture.png" , TEXTURE_DIRECTORY"OperativeWithGunTexture.png" , TEXTURE_DIRECTORY"SentientWithGunTexture.png"};

static const char * ITEMS_TEXTURES[ITEMS_COUNT - 1] = { TEXTURE_DIRECTORY"HealthKitTexture.png" , TEXTURE_DIRECTORY"AContTexture.png" };

static const char * GUNS_TEXTURES[AMMUNITION_COUNT] = { TEXTURE_DIRECTORY"LargeCaliberTexture.png" , TEXTURE_DIRECTORY"MediumCaliberTexture.png" , TEXTURE_DIRECTORY"SmallCaliberTexture.png" };

static const char * CELLS_TEXTURES[CELLSTYPE_COUNT] = { TEXTURE_DIRECTORY"FloorTexture.png" , TEXTURE_DIRECTORY"WallTexture.png" , TEXTURE_DIRECTORY"GlassTexture.png" , TEXTURE_DIRECTORY"PartitionTexture.png" , TEXTURE_DIRECTORY"StorageTexture.png" };

#endif //LAB4_PARAMETERS_H
