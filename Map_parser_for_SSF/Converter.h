#pragma once
#include "Map_info.h"
#include "Map_mini.h"
#include "Map_rhombs.h"
#include "Map_objects.h"
#include "Map_landname.h"
#include "Map_cflags.h"
#include "Map_desc.h"

#include "Mis_mult.h"
#include "Mis_info.h"
#include "Mis_desc.h"
#include "Mis_zones.h"
#include "Mis_mapunits.h"
#include "Mis_scripts.h"
#include "Mis_woofers.h"
#include "Mis_phrases.h"
#include "Mis_objects.h"
#include "Mis_tree.h"
#include "Mis_mines.h"
#include "Mis_groups.h"
#include "Mis_players.h"

uint32_t convertMapFileSMM(std::stringstream& inputFile,const std::string& convertMapName);
uint32_t convertMapFileSSM(std::stringstream& inputFile, const std::string& convertMapName);
uint32_t convertMapFileSSC_map(std::stringstream& inputFile, const std::string& convertMapName);
uint32_t convertMapFileSCC_mission(std::stringstream& inputFile);
