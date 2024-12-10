
#include "stdafx.h"

#include "Helper.h"
#include "Parser.h"
#include "util.h"
#include "Displayinfo.h"

void Parser::parseMap(const std::string_view& inputData, const std::string_view& filename)
{
	m_mapFileName = filename;
	m_rawMapFileName = filename.substr(0, filename.find_last_of("."));
	m_mapFolder = "parser_map.000";
	m_misFolder = "parser_map.000/mis.000";

	const uint32_t mapType = *(uint32_t*)inputData.data();
	switch (mapType)
	{
	case (HEADER_SINGLE):
	{
		own::println(Dictionary::getValue(STRINGS::MAP_SINGLE));
		parseMapFileSMM(inputData);
		break;
	}
	case (HEADER_MULTI):
	{
		own::println(Dictionary::getValue(STRINGS::MAP_MULTI));
		parseMapFileSMM(inputData);
		break;
	}
	case (HEADER_CAMP_MAP):
	{
		m_mapFolder = "parser_map." + m_mapFileName.substr(0, 3);
		std::println("mapFolder: {}", m_mapFolder.string());

		own::println(Dictionary::getValue(STRINGS::CAMP_MAP));
		parseMapFileSSC_map(inputData);
		break;
	}
	case (HEADER_CAMP_MIS):
	{
		m_mapFolder = "parser_map." + m_mapFileName.substr(0, 3);;
		m_misFolder = m_mapFolder / ("mis." + m_mapFileName.substr(3, 3));
		std::println("mapFolder: {}\nmisFolder: {}", m_mapFolder.string(), m_misFolder.string());

		own::println(Dictionary::getValue(STRINGS::CAMP_MIS));
		parseMapFileSCC_mission(inputData);
		break;
	}
	default:
	{
		own::println(Dictionary::getValue(STRINGS::ERROR_FILE));
	}
	};
}

uint32_t Parser::parseMapFileSMM(const std::string_view& inputFile) const
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories(m_mapFolder);
	std::ofstream outputMapInfo(m_mapFolder / "map_info", std::ios::binary);
	std::ofstream outputMisScripts(m_mapFolder / "mis_scripts", std::ios::binary);
	std::ofstream outputMisDesc(m_mapFolder / "mis_desc", std::ios::binary);
	std::ofstream outputMiniMap(m_mapFolder / "map_mini", std::ios::binary);
	std::ofstream outputMapRhombs(m_mapFolder / "map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags(m_mapFolder / "map_flags", std::ios::binary);
	std::ofstream outputMapLandnames(m_mapFolder / "map_landnames", std::ios::binary);
	std::ofstream outputMapObjects(m_mapFolder / "map_objects", std::ios::binary);
	std::ofstream outputMisUnitNames(m_mapFolder / "mis_unitnames", std::ios::binary);
	std::ofstream outputMisWoofers(m_mapFolder / "mis_woofers", std::ios::binary);
	std::ofstream outputMisZones(m_mapFolder / "mis_zones", std::ios::binary);
	std::ofstream outputMapMines(m_mapFolder / "map_mines", std::ios::binary);
	std::ofstream outputMisSupport(m_mapFolder / "mis_support", std::ios::binary);
	std::ofstream outputMisPlayers(m_mapFolder / "mis_players", std::ios::binary);
	std::ofstream outputMisPhrases(m_mapFolder / "mis_phrases", std::ios::binary);
	std::ofstream outputMisMapUnits(m_mapFolder / "mis_mapunits", std::ios::binary);
	
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	uint32_t mapIdentifier = readFileUint32(inputFile, 136);
	uint32_t mapSizeU = readFileUint32(inputFile, 140);
	uint32_t mapSizeV = readFileUint32(inputFile, 144);
	
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
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

uint32_t Parser::parseMapFileSSM(const std::string_view& inputFile) const
{
	std::filesystem::create_directories(m_mapFolder);
	std::ofstream outputMapInfo(m_mapFolder / "map_info", std::ios::binary);
	std::ofstream outputMisDesc(m_mapFolder / "mis_desc", std::ios::binary);
	std::ofstream outputMiniMap(m_mapFolder / "map_mini", std::ios::binary);
	std::ofstream outputMapRhombs(m_mapFolder / "map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags(m_mapFolder / "map_flags", std::ios::binary);
	std::ofstream outputMapLandnames(m_mapFolder / "map_landnames", std::ios::binary);
	std::ofstream outputMapObjects(m_mapFolder / "map_objects", std::ios::binary);
	std::ofstream outputMisUnitNames(m_mapFolder / "mis_unitnames", std::ios::binary);
	std::ofstream outputMisGroups(m_mapFolder / "mis_groups", std::ios::binary);
	std::ofstream outputMisMapUnits(m_mapFolder / "mis_mapunits", std::ios::binary);
	std::ofstream outputMisPlayers(m_mapFolder / "mis_players", std::ios::binary);
	std::ofstream outputMisWoofers(m_mapFolder / "mis_woofers", std::ios::binary);
	std::ofstream outputMisZones(m_mapFolder / "mis_zones", std::ios::binary);
	std::ofstream outputMisScripts(m_mapFolder / "mis_scripts", std::ios::binary);
	std::ofstream outputMapMines(m_mapFolder / "map_mines", std::ios::binary);
	std::ofstream outputMisSupport(m_mapFolder / "mis_support", std::ios::binary);
	std::ofstream outputMisPhrases(m_mapFolder / "mis_phrases", std::ios::binary);
	std::ofstream outputMisObjects(m_mapFolder / "mis_objects", std::ios::binary);
	
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	uint32_t mapIdentifier = readFileUint32(inputFile, 40);
	uint32_t mapSizeU = readFileUint32(inputFile, 44);
	uint32_t mapSizeV = readFileUint32(inputFile, 48);
	uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);
	
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
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

uint32_t Parser::parseMapFileSSC_map(const std::string_view& inputFile) const
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories(m_mapFolder);
	std::ofstream outputMapInfo(m_mapFolder / "map_info", std::ios::binary);
	std::ofstream outputMapLandnames(m_mapFolder / "map_landnames", std::ios::binary);
	std::ofstream outputMapRhombs(m_mapFolder / "map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags(m_mapFolder / "map_flags", std::ios::binary);
	std::ofstream outputMiniMap(m_mapFolder / "map_mini1", std::ios::binary);
	std::ofstream outputMiniMap2(m_mapFolder / "map_mini2", std::ios::binary);
	std::ofstream outputMapObjects(m_mapFolder / "map_objects", std::ios::binary);
	
	// ќпределите начальную позицию и количество байт дл€ извлечени€
	// Ќачальна€ позици€ нужных байтов
	uint32_t mapIdentifier = readFileUint32(inputFile, 8);
	uint32_t mapSizeU = readFileUint32(inputFile, 12);
	uint32_t mapSizeV = readFileUint32(inputFile, 16);
	
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
	uint32_t startPositionMapInfo = 0;
	uint32_t startPositionLandname = position(inputFile, outputMapInfo, startPositionMapInfo, MapHeaderSSC_map);
	uint32_t startPositionRhombs = position(inputFile, outputMapLandnames, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags = position(inputFile, outputMapRhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionMapMini = position(inputFile, outputMapFlags, startPositionFlags, flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, outputMiniMap, startPositionMapMini, rhombsSize);
	uint32_t startPositionObjects = position(inputFile, outputMiniMap2, startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t mapEndPosition = position(inputFile, outputMapObjects, startPositionObjects + 4, mapSizeObjects * 8);
	
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Parser::parseMapFileSCC_mission(const std::string_view& inputFile) const
{
	// ќтрываем новый текстовый документ дл€ записи
	std::filesystem::create_directories(m_mapFolder);
	std::ofstream outputMisDesc(m_mapFolder / "mis_desc", std::ios::binary);
	std::ofstream outputMisUnitNames(m_mapFolder / "mis_unitnames", std::ios::binary);
	std::ofstream outputMisGroups(m_mapFolder / "mis_groups", std::ios::binary);
	std::ofstream outputMisMapUnits(m_mapFolder / "mis_mapunits", std::ios::binary);
	std::ofstream outputMisPlayers(m_mapFolder / "mis_players", std::ios::binary);
	std::ofstream outputMisWoofers(m_mapFolder / "mis_woofers", std::ios::binary);
	std::ofstream outputMisZones(m_mapFolder / "mis_zones", std::ios::binary);
	std::ofstream outputMisScripts(m_mapFolder / "mis_scripts", std::ios::binary);
	std::ofstream outputMapMines(m_mapFolder / "map_mines", std::ios::binary);
	std::ofstream outputMisSupport(m_mapFolder / "mis_support", std::ios::binary);
	std::ofstream outputMisPhrases(m_mapFolder / "mis_phrases", std::ios::binary);
	std::ofstream outputMisObjects(m_mapFolder / "mis_objects", std::ios::binary);
	
	uint32_t mapSizeU = readFileUint32(inputFile, 16);
	uint32_t mapSizeV = readFileUint32(inputFile, 20);
	
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
	
	displayinfo(mapSizeU, mapSizeV, 40, mapEndPosition);
	return 0;
}