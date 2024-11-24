#include <iostream> 
#include <fstream>
#include <string>
#include "Converter.h"
#include "Parser.h"
#include "General.h"
#include <sstream>
#include <filesystem>
#include "Displayinfo.h"

uint32_t position(std::stringstream& inputFile, std::stringstream& outputFile, const uint32_t startPosition, const uint32_t size)
{
	inputFile.seekg(startPosition);
	char* buffer = new char[size];
	inputFile.read(buffer, size);
	outputFile.write(buffer, size);
	delete[] buffer;
	//outputFile.close();
	uint32_t position = startPosition + size;
	return position;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSMM(std::stringstream& inputFile, const std::string& convertMapName)
{
	//MAP
	std::stringstream map_info;
	std::stringstream map_mini;
	std::stringstream map_rhombs;
	std::stringstream map_flags;
	std::stringstream map_landname;
	std::stringstream map_objects;
	std::stringstream map_mines;
	//MIS
	std::stringstream mis_desc;
	std::stringstream mis_unitnames;
	std::stringstream mis_woofers;
	std::stringstream mis_scripts;
	std::stringstream mis_zones;
	std::stringstream mis_support;
	std::stringstream mis_players;
	std::stringstream mis_phrases;
	std::stringstream mis_mapunits;
	//------------------------------------------------------------------------------
	// Определите начальную позицию и количество байт для извлечения
	uint32_t mapIdentifier = readFileUint32(inputFile, 136);
	uint32_t mapSizeU = readFileUint32(inputFile, 140);
	uint32_t mapSizeV = readFileUint32(inputFile, 144);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionMisScripts = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSMM);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMisDesc = position(inputFile, mis_scripts, startPositionMisScripts, accumulator + 4);
	uint32_t BriefingSize = readFileUint32(inputFile, startPositionMisDesc);
	uint32_t startPositionMapMini = position(inputFile, mis_desc, startPositionMisDesc + 4, BriefingSize);
	uint32_t MapMiniSize = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs = position(inputFile, map_mini, startPositionMapMini, MapMiniSize);
	uint32_t startPositionFlags = position(inputFile, map_rhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionLandnames = position(inputFile, map_flags, startPositionFlags, flagSize);
	uint32_t startPositionMapObjects = position(inputFile, map_landname, startPositionLandnames, LANDNAMESSIZE);
	uint32_t mapSizeMapObjects = readFileUint32(inputFile, startPositionMapObjects);
	uint32_t startPositionMisUnitNames = position(inputFile, map_objects, startPositionMapObjects + 4, mapSizeMapObjects * 8);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisWoofers = position(inputFile, mis_unitnames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, mis_woofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMapMines = position(inputFile, mis_zones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t startPositionMisSupport = position(inputFile, map_mines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPlayers = position(inputFile, mis_support, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisPhrases = position(inputFile, mis_players, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisMapUnits = position(inputFile, mis_phrases, startPositionMisPhrases + 4, sizeMisPhrases);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t mapEndPosition = position(inputFile, mis_mapunits, startPositionMisMapUnits + 4, sizeMisMapUnits);
	//------------------------------------------------------------------------------
	inputFile.str("");
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	//MAP
	covertMapDesc(convertMapName);
	covertMisMult(map_info, 1);
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
	covertMapLandname(map_landname);
	covertMapRhombs(map_rhombs, mapIdentifier);
	covertMapCflags(map_flags);
	covertMapObjects(map_objects);
	//MIS
	covertMisDesc();
	covertMisInfo(mapSizeU, mapSizeV);
	covertMisScripts(mis_scripts);
	covertMisTree(mis_desc);
	covertMisWoofers(mis_woofers);
	convertMisMines(map_mines, mapSizeU);
	covertMisZones(mis_zones, mapSizeU);
	convertMisPlayers(mis_players);
	covertMisPhrases(mis_phrases, sizeMisPhrases);
	covertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSSM(std::stringstream& inputFile, const std::string& convertMapName)
{
	//MAP
	std::stringstream map_info;
	std::stringstream map_mini;
	std::stringstream map_rhombs;
	std::stringstream map_flags;
	std::stringstream map_landname;
	std::stringstream map_objects;
	std::stringstream map_mines;
	//MIS
	std::stringstream mis_desc;
	std::stringstream mis_unitnames;
	std::stringstream mis_groups;
	std::stringstream mis_mapunits;
	std::stringstream mis_players;
	std::stringstream mis_woofers;
	std::stringstream mis_zones;
	std::stringstream mis_scripts;
	std::stringstream mis_support;
	std::stringstream mis_phrases;
	std::stringstream mis_objects;
	//------------------------------------------------------------------------------
	// Определите начальную позицию и количество байт для извлечения
	uint32_t mapIdentifier = readFileUint32(inputFile, 40);
	uint32_t mapSizeU = readFileUint32(inputFile, 44);
	uint32_t mapSizeV = readFileUint32(inputFile, 48);
	uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionMisDesc = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSSM);
	uint32_t startPositionMapMini = position(inputFile, mis_desc, startPositionMisDesc + 4, maskByte);
	uint32_t MapMiniSize = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs = position(inputFile, map_mini, startPositionMapMini, MapMiniSize);
	uint32_t startPositionFlags = position(inputFile, map_rhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionLandname = position(inputFile, map_flags, startPositionFlags, flagSize);
	uint32_t startPositionObjects = position(inputFile, map_landname, startPositionLandname, LANDNAMESSIZE);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t startPositionMisUnitNames = position(inputFile, map_objects, startPositionObjects + 4, mapSizeObjects * 8);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisGroups = position(inputFile, mis_unitnames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits = position(inputFile, mis_groups, startPositionMisGroups, MISGROUPSSIZE);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t startPositionMisPlayers = position(inputFile, mis_mapunits, startPositionMisMapUnits + 4, sizeMisMapUnits);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisWoofers = position(inputFile, mis_players, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, mis_woofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts = position(inputFile, mis_zones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, mis_scripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, map_mines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, mis_support, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, mis_phrases, startPositionMisPhrases + 4, sizeMisPhrases);
	uint32_t mapEndPosition = position(inputFile, mis_objects, startPositionMisObjects, MISOBJECTS);
	inputFile.str("");
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	//MAP
	covertMapDesc(convertMapName);
	covertMisMult(map_info, 0);
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
	covertMapRhombs(map_rhombs, mapIdentifier);
	covertMapObjects(map_objects);
	covertMapLandname(map_landname);
	covertMapCflags(map_flags);
	//MIS
	covertMisDesc();
	covertMisInfo(mapSizeU, mapSizeV);
	covertMisTree(mis_desc);
	covertMisZones(mis_zones, mapSizeU);
	convertMisMines(map_mines, mapSizeU);
	covertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	covertMisScripts(mis_scripts);
	covertMisWoofers(mis_woofers);
	covertMisPhrases(mis_phrases, sizeMisPhrases);
	covertMisObjects(mis_objects);
	convertMisGroups(mis_groups);
	convertMisPlayers(mis_players);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSSC_map(std::stringstream& inputFile, const std::string& convertMapName)
{
	std::stringstream map_info;
	std::stringstream map_landname;
	std::stringstream map_mini;
	std::stringstream map_mini2;
	std::stringstream map_rhombs;
	std::stringstream map_flags;
	std::stringstream map_objects;
	//------------------------------------------------------------------------------
	// Определите начальную позицию и количество байт для извлечения
	// Начальная позиция нужных байтов
	uint32_t mapIdentifier = readFileUint32(inputFile, 8);
	uint32_t mapSizeU = readFileUint32(inputFile, 12);
	uint32_t mapSizeV = readFileUint32(inputFile, 16);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionLandname = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSSC_map);
	uint32_t startPositionRhombs = position(inputFile, map_landname, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags = position(inputFile, map_rhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionMapMini = position(inputFile, map_flags, startPositionFlags, flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, map_mini, startPositionMapMini, rhombsSize);
	uint32_t startPositionObjects = position(inputFile, map_mini2, startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t mapEndPosition = position(inputFile, map_objects, startPositionObjects + 4, mapSizeObjects * 8);
	inputFile.str("");
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
	covertMapLandname(map_landname);
	covertMapCflags(map_flags);
	covertMapRhombs(map_rhombs, mapIdentifier);
	covertMapObjects(map_objects);
	covertMapDesc(convertMapName);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSCC_mission(std::stringstream& inputFile)
{
	std::stringstream mis_info;
	std::stringstream map_mines;
	std::stringstream mis_desc;
	std::stringstream mis_unitnames;
	std::stringstream mis_groups;
	std::stringstream mis_mapunits;
	std::stringstream mis_players;
	std::stringstream mis_maunits;
	std::stringstream mis_woofers;
	std::stringstream mis_zones;
	std::stringstream mis_scripts;
	std::stringstream mis_support;
	std::stringstream mis_phrases;
	std::stringstream mis_objects;
	//------------------------------------------------------------------------------
	uint32_t mapSizeU = readFileUint32(inputFile, 16);
	uint32_t mapSizeV = readFileUint32(inputFile, 20);
	//------------------------------------------------------------------------------
	uint32_t startPositionMisDesc = position(inputFile, mis_info, FILE_TYPE_OFFSET, MapHeaderSSC_mission);
	uint32_t sizeMisDesc = readFileUint32(inputFile, MapHeaderSSC_mission);
	uint32_t startPositionMisUnitNames = position(inputFile, mis_desc, startPositionMisDesc + 4, sizeMisDesc);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisGroups = position(inputFile, mis_unitnames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits = position(inputFile, mis_groups, startPositionMisGroups, MISGROUPSSIZE);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t startPositionMisPlayers = position(inputFile, mis_mapunits, startPositionMisMapUnits + 4, sizeMisMapUnits);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisWoofers = position(inputFile, mis_players, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, mis_woofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts = position(inputFile, mis_zones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, mis_scripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, map_mines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, mis_support, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, mis_phrases, startPositionMisPhrases + 4, sizeMisPhrases);
	uint32_t mapEndPosition = position(inputFile, mis_objects, startPositionMisObjects, MISOBJECTS);
	inputFile.str("");
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	covertMisMult(mis_info, 2);
	covertMisInfo(mapSizeU, mapSizeV);
	covertMisZones(mis_zones, mapSizeU);
	covertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	convertMisGroups(mis_groups);
	covertMisScripts(mis_scripts);
	covertMisWoofers(mis_woofers);
	convertMisMines(map_mines, mapSizeU);
	covertMisDesc();
	covertMisTree(mis_desc);
	covertMisPhrases(mis_phrases, sizeMisPhrases);
	covertMisObjects(mis_objects);
	convertMisPlayers(mis_players);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, 40, mapEndPosition);
	return 0;
}