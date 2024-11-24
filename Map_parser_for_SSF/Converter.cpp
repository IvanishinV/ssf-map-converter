
#include "stdafx.h"

#include "Helper.h"
#include "Converter.h"
#include "Parser.h"
#include "General.h"
#include "Displayinfo.h"

//___________________________________________________________________________________________________
uint32_t convertMapFileSMM(const std::string_view& inputFile, const std::string_view& convertMapName)
{
	//MAP
	std::vector<uint8_t> map_info;
	std::vector<uint8_t> map_mini;
	std::vector<uint8_t> map_rhombs;
	std::vector<uint8_t> map_flags;
	std::vector<uint8_t> map_landname;
	std::vector<uint8_t> map_objects;
	std::vector<uint8_t> map_mines;
	//MIS
	std::vector<uint8_t> mis_desc;
	std::vector<uint8_t> mis_unitnames;
	std::vector<uint8_t> mis_woofers;
	std::vector<uint8_t> mis_scripts;
	std::vector<uint8_t> mis_zones;
	std::vector<uint8_t> mis_support;
	std::vector<uint8_t> mis_players;
	std::vector<uint8_t> mis_phrases;
	std::vector<uint8_t> mis_mapunits;
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
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	//MAP
	const auto resMapRhombs = std::async(std::launch::async, covertMapRhombs, map_rhombs, mapIdentifier);
	covertMapDesc(convertMapName);
	covertMisMult(map_info, 1);
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
	covertMapLandname(map_landname);
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
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSSM(const std::string_view& inputFile, const std::string_view& convertMapName)
{
	//MAP
	std::vector<uint8_t> map_info;
	std::vector<uint8_t> map_mini;
	std::vector<uint8_t> map_rhombs;
	std::vector<uint8_t> map_flags;
	std::vector<uint8_t> map_landname;
	std::vector<uint8_t> map_objects;
	std::vector<uint8_t> map_mines;
	//MIS
	std::vector<uint8_t> mis_desc;
	std::vector<uint8_t> mis_unitnames;
	std::vector<uint8_t> mis_groups;
	std::vector<uint8_t> mis_mapunits;
	std::vector<uint8_t> mis_players;
	std::vector<uint8_t> mis_woofers;
	std::vector<uint8_t> mis_zones;
	std::vector<uint8_t> mis_scripts;
	std::vector<uint8_t> mis_support;
	std::vector<uint8_t> mis_phrases;
	std::vector<uint8_t> mis_objects;
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
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	//MAP
	const auto resMapRhombs = std::async(std::launch::async, covertMapRhombs, map_rhombs, mapIdentifier);
	covertMapDesc(convertMapName);
	covertMisMult(map_info, 0);
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
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
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSSC_map(const std::string_view& inputFile, const std::string_view& convertMapName)
{
	std::vector<uint8_t> map_info;
	std::vector<uint8_t> map_landname;
	std::vector<uint8_t> map_mini;
	std::vector<uint8_t> map_mini2;
	std::vector<uint8_t> map_rhombs;
	std::vector<uint8_t> map_flags;
	std::vector<uint8_t> map_objects;
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
	
	//------------------------------------------------------------------------------
	std::filesystem::create_directories("map.000/mis.000");
	//------------------------------------------------------------------------------
	const auto resMapRhombs = std::async(std::launch::async, covertMapRhombs, map_rhombs, mapIdentifier);
	covertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	covertMapMini(map_mini);
	covertMapLandname(map_landname);
	covertMapCflags(map_flags);
	covertMapObjects(map_objects);
	covertMapDesc(convertMapName);
	//------------------------------------------------------------------------------
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t convertMapFileSCC_mission(const std::string_view& inputFile)
{
	std::vector<uint8_t> mis_info;
	std::vector<uint8_t> map_mines;
	std::vector<uint8_t> mis_desc;
	std::vector<uint8_t> mis_unitnames;
	std::vector<uint8_t> mis_groups;
	std::vector<uint8_t> mis_mapunits;
	std::vector<uint8_t> mis_players;
	std::vector<uint8_t> mis_maunits;
	std::vector<uint8_t> mis_woofers;
	std::vector<uint8_t> mis_zones;
	std::vector<uint8_t> mis_scripts;
	std::vector<uint8_t> mis_support;
	std::vector<uint8_t> mis_phrases;
	std::vector<uint8_t> mis_objects;
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