
#include "stdafx.h"

#include "Helper.h"
#include "Parser.h"
#include "General.h"
#include "Displayinfo.h"

uint32_t minimapsize(uint32_t mapSizeU, uint32_t mapSizeV)
{
	if (mapSizeU <= 128 && mapSizeV <= 128 && mapSizeU == mapSizeV)
	{
		uint32_t MiniSize = mapSizeU * mapSizeV * 2;
		return MiniSize;
	}
	else
	{
		uint32_t MiniSize = mapSizeU * mapSizeV / 2;
		return MiniSize;
	}
}
//___________________________________________________________________________________________________
uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number)
{
	uint32_t result;
	result = mapSizeU * mapSizeV * number;
	return result;
}
//___________________________________________________________________________________________________
uint32_t misScripts(const std::string_view& inputFile, uint32_t scripts_number, const uint32_t scripts_position)
{
	if (scripts_number <= 0)
	{
		return 0;
	}
	else
	{
		uint32_t accumulator = 0;
		while (scripts_number--)
		{
			uint32_t COAF;
			COAF = readFileUint32(inputFile, scripts_position + 4 + accumulator);
			accumulator += COAF + 4;
		}
		return accumulator;
	}
}
//___________________________________________________________________________________________________
uint32_t parserMapFileSMM(const std::string_view& inputFile)
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories("parser_map");
	std::ofstream outputMapInfo("parser_map/map_info", std::ios::binary);
	std::ofstream outputMisScripts("parser_map/mis_scripts", std::ios::binary);
	std::ofstream outputMisDesc("parser_map/mis_desc", std::ios::binary);
	std::ofstream outputMiniMap("parser_map/map_mini", std::ios::binary);
	std::ofstream outputMapRhombs("parser_map/map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags("parser_map/map_flags", std::ios::binary);
	std::ofstream outputMapLandnames("parser_map/map_landnames", std::ios::binary);
	std::ofstream outputMapObjects("parser_map/map_objects", std::ios::binary);
	std::ofstream outputMisUnitNames("parser_map/mis_unitnames", std::ios::binary);
	std::ofstream outputMisWoofers("parser_map/mis_woofers", std::ios::binary);
	std::ofstream outputMisZones("parser_map/mis_zones", std::ios::binary);
	std::ofstream outputMapMines("parser_map/map_mines", std::ios::binary);
	std::ofstream outputMisSupport("parser_map/mis_support", std::ios::binary);
	std::ofstream outputMisPlayers("parser_map/mis_players", std::ios::binary);
	std::ofstream outputMisPhrases("parser_map/mis_phrases", std::ios::binary);
	std::ofstream outputMisMapUnits("parser_map/mis_mapunits", std::ios::binary);
	//------------------------------------------------------------------------------
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	uint32_t mapIdentifier = readFileUint32(inputFile, 136);
	uint32_t mapSizeU = readFileUint32(inputFile, 140);
	uint32_t mapSizeV = readFileUint32(inputFile, 144);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionMapInfo = 0;
	uint32_t startPositionMisScripts = position(inputFile, outputMapInfo, startPositionMapInfo, MapHeaderSMM);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMisDesc = position(inputFile, outputMisScripts, startPositionMisScripts, accumulator + 4);
	uint32_t BriefingSize = readFileUint32(inputFile, startPositionMisDesc);
	uint32_t startPositionMapMini = position(inputFile, outputMisDesc, startPositionMisDesc + 4, BriefingSize);
	uint32_t MapMiniSize = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs = position(inputFile, outputMiniMap, startPositionMapMini, MapMiniSize);
	uint32_t startPositionFlags = position(inputFile, outputMapRhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionLandnames = position(inputFile, outputMapFlags, startPositionFlags, flagSize);
	uint32_t startPositionMapObjects = position(inputFile, outputMapLandnames, startPositionLandnames, LANDNAMESSIZE);
	uint32_t mapSizeMapObjects = readFileUint32(inputFile, startPositionMapObjects);
	uint32_t startPositionMisUnitNames = position(inputFile, outputMapObjects, startPositionMapObjects + 4, mapSizeMapObjects * 8);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisWoofers = position(inputFile, outputMisUnitNames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, outputMisWoofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMapMines = position(inputFile, outputMisZones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t startPositionMisSupport = position(inputFile, outputMapMines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPlayers = position(inputFile, outputMisSupport, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisPhrases = position(inputFile, outputMisPlayers, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisMapUnits = position(inputFile, outputMisPhrases, startPositionMisPhrases, sizeMisPhrases + 4);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t mapEndPosition = position(inputFile, outputMisMapUnits, startPositionMisMapUnits + 4, sizeMisMapUnits);

	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t parserMapFileSSM(const std::string_view& inputFile)
{
	std::filesystem::create_directories("parser_map");
	std::ofstream outputMapInfo("parser_map/map_info", std::ios::binary);
	std::ofstream outputMisDesc("parser_map/mis_desc", std::ios::binary);
	std::ofstream outputMiniMap("parser_map/map_mini", std::ios::binary);
	std::ofstream outputMapRhombs("parser_map/map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags("parser_map/map_flags", std::ios::binary);
	std::ofstream outputMapLandnames("parser_map/map_landnames", std::ios::binary);
	std::ofstream outputMapObjects("parser_map/map_objects", std::ios::binary);
	std::ofstream outputMisUnitNames("parser_map/mis_unitnames", std::ios::binary);
	std::ofstream outputMisGroups("parser_map/mis_groups", std::ios::binary);
	std::ofstream outputMisMapUnits("parser_map/mis_mapunits", std::ios::binary);
	std::ofstream outputMisPlayers("parser_map/mis_players", std::ios::binary);
	std::ofstream outputMisWoofers("parser_map/mis_woofers", std::ios::binary);
	std::ofstream outputMisZones("parser_map/mis_zones", std::ios::binary);
	std::ofstream outputMisScripts("parser_map/mis_scripts", std::ios::binary);
	std::ofstream outputMapMines("parser_map/map_mines", std::ios::binary);
	std::ofstream outputMisSupport("parser_map/mis_support", std::ios::binary);
	std::ofstream outputMisPhrases("parser_map/mis_phrases", std::ios::binary);
	std::ofstream outputMisObjects("parser_map/mis_objects", std::ios::binary);
	//------------------------------------------------------------------------------
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	uint32_t mapIdentifier = readFileUint32(inputFile, 40);
	uint32_t mapSizeU = readFileUint32(inputFile, 44);
	uint32_t mapSizeV = readFileUint32(inputFile, 48);
	uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionMapInfo = 0;
	uint32_t startPositionMisDesc = position(inputFile, outputMapInfo, startPositionMapInfo, MapHeaderSSM);
	uint32_t startPositionMapMini = position(inputFile, outputMisDesc, startPositionMisDesc + 4, maskByte);
	uint32_t MapMiniSize = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs = position(inputFile, outputMiniMap, startPositionMapMini, MapMiniSize);
	uint32_t startPositionFlags = position(inputFile, outputMapRhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionLandname = position(inputFile, outputMapFlags, startPositionFlags, flagSize);
	uint32_t startPositionObjects = position(inputFile, outputMapLandnames, startPositionLandname, LANDNAMESSIZE);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t startPositionMisUnitNames = position(inputFile, outputMapObjects, startPositionObjects + 4, mapSizeObjects * 8);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisGroups = position(inputFile, outputMisUnitNames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits = position(inputFile, outputMisGroups, startPositionMisGroups, MISGROUPSSIZE);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t startPositionMisPlayers = position(inputFile, outputMisMapUnits, startPositionMisMapUnits + 4, sizeMisMapUnits);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisWoofers = position(inputFile, outputMisPlayers, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, outputMisWoofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts = position(inputFile, outputMisZones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, outputMisScripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, outputMapMines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, outputMisSupport, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, outputMisPhrases, startPositionMisPhrases, sizeMisPhrases + 4);
	uint32_t mapEndPosition = position(inputFile, outputMisObjects, startPositionMisObjects, MISOBJECTS);

	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t parserMapFileSSC_map(const std::string_view& inputFile)
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories("parser_map");
	std::ofstream outputMapInfo("parser_map/map_info", std::ios::binary);
	std::ofstream outputMapLandnames("parser_map/map_landnames", std::ios::binary);
	std::ofstream outputMapRhombs("parser_map/map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags("parser_map/map_flags", std::ios::binary);
	std::ofstream outputMiniMap("parser_map/map_mini1", std::ios::binary);
	std::ofstream outputMiniMap2("parser_map/map_mini2", std::ios::binary);
	std::ofstream outputMapObjects("parser_map/map_objects", std::ios::binary);
	//------------------------------------------------------------------------------
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	// Ќачальна€ позици€ нужных байтов
	uint32_t mapIdentifier = readFileUint32(inputFile, 8);
	uint32_t mapSizeU = readFileUint32(inputFile, 12);
	uint32_t mapSizeV = readFileUint32(inputFile, 16);
	//------------------------------------------------------------------------------
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	//------------------------------------------------------------------------------
	uint32_t startPositionMapInfo = 0;
	uint32_t startPositionLandname = position(inputFile, outputMapInfo, startPositionMapInfo, MapHeaderSSC_map);
	uint32_t startPositionRhombs = position(inputFile, outputMapLandnames, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags = position(inputFile, outputMapRhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionMapMini = position(inputFile, outputMapFlags, startPositionFlags, flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, outputMiniMap, startPositionMapMini, rhombsSize);
	uint32_t startPositionObjects = position(inputFile, outputMiniMap2, startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t mapEndPosition = position(inputFile, outputMapObjects, startPositionObjects + 4, mapSizeObjects * 8);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
//___________________________________________________________________________________________________
uint32_t parserMapFileSCC_mission(const std::string_view& inputFile)
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories("parser_map");
	std::ofstream outputMisDesc("parser_map/mis_desc", std::ios::binary);
	std::ofstream outputMisUnitNames("parser_map/mis_unitnames", std::ios::binary);
	std::ofstream outputMisGroups("parser_map/mis_groups", std::ios::binary);
	std::ofstream outputMisMapUnits("parser_map/mis_mapunits", std::ios::binary);
	std::ofstream outputMisPlayers("parser_map/mis_players", std::ios::binary);
	std::ofstream outputMisWoofers("parser_map/mis_woofers", std::ios::binary);
	std::ofstream outputMisZones("parser_map/mis_zones", std::ios::binary);
	std::ofstream outputMisScripts("parser_map/mis_scripts", std::ios::binary);
	std::ofstream outputMapMines("parser_map/map_mines", std::ios::binary);
	std::ofstream outputMisSupport("parser_map/mis_support", std::ios::binary);
	std::ofstream outputMisPhrases("parser_map/mis_phrases", std::ios::binary);
	std::ofstream outputMisObjects("parser_map/mis_objects", std::ios::binary);
	//------------------------------------------------------------------------------
	uint32_t mapSizeU = readFileUint32(inputFile, 16);
	uint32_t mapSizeV = readFileUint32(inputFile, 20);
	//------------------------------------------------------------------------------
	uint32_t startPositionMisDesc = MapHeaderSSC_mission + 4;
	uint32_t sizeMisDesc = readFileUint32(inputFile, MapHeaderSSC_mission);
	uint32_t startPositionMisUnitNames = position(inputFile, outputMisDesc, startPositionMisDesc, sizeMisDesc);
	uint32_t sizeMisUnitNames = readFileUint32(inputFile, startPositionMisUnitNames);
	uint32_t startPositionMisGroups = position(inputFile, outputMisUnitNames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits = position(inputFile, outputMisGroups, startPositionMisGroups, MISGROUPSSIZE);
	uint32_t sizeMisMapUnits = readFileUint32(inputFile, startPositionMisMapUnits);
	uint32_t startPositionMisPlayers = position(inputFile, outputMisMapUnits, startPositionMisMapUnits + 4, sizeMisMapUnits);
	uint32_t sizeMisPlayers = readFileUint32(inputFile, startPositionMisPlayers);
	uint32_t startPositionMisWoofers = position(inputFile, outputMisPlayers, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers = readFileUint32(inputFile, startPositionMisWoofers);
	uint32_t startPositionMisZones = position(inputFile, outputMisWoofers, startPositionMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts = position(inputFile, outputMisZones, startPositionMisZones, mapSizeU * mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, outputMisScripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, outputMapMines, startPositionMapMines, mapSizeU * mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, outputMisSupport, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, outputMisPhrases, startPositionMisPhrases, sizeMisPhrases + 4);
	uint32_t mapEndPosition = position(inputFile, outputMisObjects, startPositionMisObjects, MISOBJECTS);
	//------------------------------------------------------------------------------
	displayinfo(mapSizeU, mapSizeV, 40, mapEndPosition);
	return 0;
}