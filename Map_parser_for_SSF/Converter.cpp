
#include "stdafx.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

#include "Helper.h"
#include "Converter.h"
#include "Displayinfo.h"
#include "RhombsParser.h"
#include "util.h"

void convertOPN(std::stringstream& bufferScripts_OPN1, const uint32_t logicOperator, const uint32_t scriptNumber, std::stringstream& bufferScriptsOPN, std::stringstream& bufferScripts)
{
	//Функция выполняющая обратную польскую нотацию
	if (scriptNumber == 1)
	{
		bufferScripts << bufferScripts_OPN1.str();
		bufferScripts_OPN1.str("");
	}
	else
	{
		if (logicOperator == 0)
		{
			bufferScriptsOPN << bufferScripts_OPN1.str();
		}
		else
			if (logicOperator == 1)
			{
				bufferScripts << "$&" << '\n' << bufferScriptsOPN.str();
				bufferScriptsOPN.str("");
			}
			else
				if (logicOperator == 2)
				{
					bufferScripts << "$|" << '\n' << bufferScriptsOPN.str();
					bufferScriptsOPN.str("");
				}
	}
	bufferScripts_OPN1.str("");
	return;
}

void aiFlagsConvert(std::ofstream& outputFileMisGroups, const uint8_t aitype1, const uint8_t aitype2, const uint8_t aitype3, const uint8_t aitype4)
{
	uint32_t AIflags = ((aitype4 & 0xFF) << 24) | ((aitype3 & 0xFF) << 16) | ((aitype2 & 0xFF) << 8) | (aitype1 & 0xFF);
	uint8_t AItype1;
	uint8_t AItype2;
	uint8_t AItype3;
	uint8_t AItype4;

	AItype1 = (uint8_t)(AIflags & 0xF0) >> 4;
	if ((AItype1 & 0x1) == 0x1)
		outputFileMisGroups << " aif_rndmove";
	if ((AItype1 & 0x2) == 0x2)
		outputFileMisGroups << " aif_zonehaveown";
	if ((AItype1 & 0x4) == 0x4)
		outputFileMisGroups << " aif_zonenoemeny";
	if ((AItype1 & 0x8) == 0x8)
		outputFileMisGroups << " aif_useobjects";

	AItype2 = (uint8_t)(AIflags >> 8) & 0xFF;
	if ((AItype2 & 0x1) == 0x1)
		outputFileMisGroups << " aif_rndtarget";
	if ((AItype2 & 0x2) == 0x2)
		outputFileMisGroups << " aif_grestrict";
	if ((AItype2 & 0x4) == 0x4)
		outputFileMisGroups << " aif_zrestrict";
	if ((AItype2 & 0x8) == 0x8)
		outputFileMisGroups << " aif_atgnouse";
	if ((AItype2 & 0x10) == 0x10)
		outputFileMisGroups << " aif_atgzonehaveown";
	if ((AItype2 & 0x20) == 0x20)
		outputFileMisGroups << " aif_atgzonenoenemy";
	if ((AItype2 & 0x40) == 0x40)
		outputFileMisGroups << " aif_atgnomove";
	if ((AItype2 & 0x80) == 0x80)
		outputFileMisGroups << " aif_atgnotow";

	AItype3 = (uint8_t)(AIflags >> 16) & 0xFF;
	if ((AItype3 & 0x1) == 0x1)
		outputFileMisGroups << " aif_atgnodrop";
	if ((AItype3 & 0x2) == 0x2)
		outputFileMisGroups << " aif_gaubnouse";
	if ((AItype3 & 0x4) == 0x4)
		outputFileMisGroups << " aif_gaubzonehaveown";
	if ((AItype3 & 0x8) == 0x8)
		outputFileMisGroups << " aif_gaubzonenoenemy";
	if ((AItype3 & 0x10) == 0x10)
		outputFileMisGroups << " aif_gaubnomove";
	if ((AItype3 & 0x20) == 0x20)
		outputFileMisGroups << " aif_gaubnotow";
	if ((AItype3 & 0x40) == 0x40)
		outputFileMisGroups << " aif_gaubnodrop";
	if ((AItype3 & 0x80) == 0x80)
		outputFileMisGroups << " aif_domnouse";

	AItype4 = (uint8_t)(AIflags >> 24) & 0x0F;
	if ((AItype4 & 0x1) == 0x1)
		outputFileMisGroups << " aif_domambush";
	if ((AItype4 & 0x2) == 0x2)
		outputFileMisGroups << " aif_domhide";
	if ((AItype4 & 0x4) == 0x4)
		outputFileMisGroups << " aif_holdfire";
}

void getScriptSize(std::stringstream& outputfilebuffer, std::stringstream& bufferScripts, uint32_t accumulatorNumberScripts)
{
	//для получения размера данных есть метод .size() надо изучить
	// переписать функцию на size_t getScriptSize(const char* begin)
	bufferScripts.seekg(0, std::ios::end);
	size_t size = bufferScripts.tellg();
	outputfilebuffer << "script \"" << accumulatorNumberScripts << "\" size " << size - 2 << '\n';
	outputfilebuffer << bufferScripts.str();
	/*cout << "Размер скрипта: " << size << '\n' << endl;*/
	return;
}


void Converter::convertMap(const std::filesystem::path& filepath)
{
	if (!std::filesystem::exists(filepath))
	{
		errorExistsFile(filepath.string());
		return;
	}

	std::ifstream inputFile(filepath, std::ios::ate | std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		errorOpenFile(filepath.string());
		return;
	}

	std::vector<char> zipData(inputFile.tellg());

	inputFile.seekg(std::ios::beg);
	inputFile.read(zipData.data(), zipData.size());
	inputFile.close();

	// check for size
	if (zipData.size() < 8)
	{
		std::println("");
		own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
		return;
	}
	// check for gzip archive
	const std::string rawData = (*(uint16_t*)zipData.data() == 0x8b1f) ?
		gzip::decompress(zipData.data(), zipData.size()) :
		std::string(zipData.cbegin(), zipData.cend());

	// trying to bypass germans umlauts in map file names
	std::wstring wfilepath = filepath.wstring();
	wfilepath.erase(std::remove_if(wfilepath.begin(), wfilepath.end(), [](wchar_t c) { return !((c >= 0 && c < 128) || (c >= 0x400 && c < 0x500)); }), wfilepath.cend());
	const std::filesystem::path filepath_ex{ wfilepath };
	
	const std::string fileName = filepath_ex.filename().string();

	const std::filesystem::path fileFolder = filepath.parent_path();
	m_mapFolder = fileFolder / "map.000";		// todo: add check for already converted files to not to 
	m_misFolder = m_mapFolder / "mis.000";

	m_mapType = *(uint32_t*)rawData.data();
	switch (m_mapType)
	{
	case (HEADER_SINGLE):
	{
		own::println(Dictionary::getValue(STRINGS::MAP_SINGLE), fileName);
		convertMapFileSSM(rawData);

		//own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filename, m_misFolder.string());
		break;
	}
	case (HEADER_MULTI):
	{
		own::println(Dictionary::getValue(STRINGS::MAP_MULTI), fileName);
		convertMapFileSMM(rawData);

		//own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filename, m_misFolder.string());
		break;
	}
	case (HEADER_CAMP_MAP):
	{
		if (fileName.length() >= 3)
			m_mapFolder = fileFolder / ("map." + fileName.substr(0, 3));

		own::println(Dictionary::getValue(STRINGS::CAMP_MAP), fileName);
		convertMapFileSSC_map(rawData);

		break;
	}
	case (HEADER_CAMP_MIS):
	{
		if (fileName.length() >= 3)
			m_mapFolder = fileFolder / ("map." + fileName.substr(0, 3));
		if (fileName.length() >= 3)
			m_misFolder = m_mapFolder / ("mis." + fileName.substr(3, 3));

		own::println(Dictionary::getValue(STRINGS::CAMP_MIS), fileName);
		convertMapFileSCC_mission(rawData);

		//own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filename, m_misFolder.string());
		break;
	}
	default:
	{
		std::println("");
		own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
		return;
	}
	};

	own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filepath_ex.string(), m_mapFolder.string());
}

uint32_t Converter::convertMapFileSMM(const std::string_view& inputFile)
{
	//MAP
	std::string_view map_info;
	std::string_view map_mini;
	std::string_view map_rhombs;
	std::string_view map_flags;
	std::string_view map_landname;
	std::string_view map_objects;
	std::string_view map_mines;
	//MIS
	std::string_view mis_desc;
	std::string_view mis_unitnames;
	std::string_view mis_woofers;
	std::string_view mis_scripts;
	std::string_view mis_zones;
	std::string_view mis_support;
	std::string_view mis_players;
	std::string_view mis_phrases;
	std::string_view mis_mapunits;
	
	// Определите начальную позицию и количество байт для извлечения
	const uint32_t mapIdentifier = readFileUint32(inputFile, 136);
	const uint32_t mapSizeU = readFileUint32(inputFile, 140);
	const uint32_t mapSizeV = readFileUint32(inputFile, 144);
	
	const uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	const uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
	const uint32_t offsetMisScripts = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSMM);
	const uint32_t sizeMisScripts = readFileUint32(inputFile, offsetMisScripts);
	const uint32_t accumulator = misScripts(inputFile, sizeMisScripts, offsetMisScripts);
	const uint32_t offsetMisDesc = position(inputFile, mis_scripts, offsetMisScripts, accumulator + 4);
	const uint32_t BriefingSize = readFileUint32(inputFile, offsetMisDesc);
	const uint32_t offsetMapMini = position(inputFile, mis_desc, offsetMisDesc + 4, BriefingSize);
	const uint32_t MapMiniSize = minimapsize(mapSizeU, mapSizeV);
	const uint32_t offsetRhombs = position(inputFile, map_mini, offsetMapMini, MapMiniSize);
	const uint32_t offsetFlags = position(inputFile, map_rhombs, offsetRhombs, rhombsSize);
	const uint32_t offsetLandnames = position(inputFile, map_flags, offsetFlags, flagSize);
	const uint32_t offsetMapObjects = position(inputFile, map_landname, offsetLandnames, LANDNAMESSIZE);
	const uint32_t mapSizeMapObjects = readFileUint32(inputFile, offsetMapObjects);
	const uint32_t offsetMisUnitNames = position(inputFile, map_objects, offsetMapObjects + 4, mapSizeMapObjects * 8);
	const uint32_t sizeMisUnitNames = readFileUint32(inputFile, offsetMisUnitNames);
	const uint32_t offsetMisWoofers = position(inputFile, mis_unitnames, offsetMisUnitNames + 4, sizeMisUnitNames * 16);
	const uint32_t sizeMisWoofers = readFileUint32(inputFile, offsetMisWoofers);
	const uint32_t offsetMisZones = position(inputFile, mis_woofers, offsetMisWoofers, sizeMisWoofers * MISMUSICSIZE + 4);
	const uint32_t offsetMapMines = position(inputFile, mis_zones, offsetMisZones, mapSizeU * mapSizeV);
	const uint32_t offsetMisSupport = position(inputFile, map_mines, offsetMapMines, mapSizeU * mapSizeV);
	const uint32_t sizeMisSupport = readFileUint32(inputFile, offsetMisSupport);
	const uint32_t offsetMisPlayers = position(inputFile, mis_support, offsetMisSupport, sizeMisSupport + 4);
	const uint32_t sizeMisPlayers = readFileUint32(inputFile, offsetMisPlayers);
	const uint32_t offsetMisPhrases = position(inputFile, mis_players, offsetMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	const uint32_t sizeMisPhrases = readFileUint32(inputFile, offsetMisPhrases);
	const uint32_t offsetMisMapUnits = position(inputFile, mis_phrases, offsetMisPhrases + 4, sizeMisPhrases);
	const uint32_t sizeMisMapUnits = readFileUint32(inputFile, offsetMisMapUnits);
	const uint32_t mapEndPosition = position(inputFile, mis_mapunits, offsetMisMapUnits + 4, sizeMisMapUnits);
	
	std::filesystem::create_directories(m_misFolder);
	
	//MAP
	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs, mapIdentifier);
	convertMapDesc();
	convertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	convertMapMini(map_mini);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	convertMapObjects(map_objects);
	//MIS
	convertMisDesc();
	convertMisMult(map_info, 1);
	convertMisInfo(mapSizeU, mapSizeV);
	convertMisScripts(mis_scripts);
	convertMisTree(mis_desc);
	convertMisWoofers(mis_woofers);
	convertMisMines(map_mines, mapSizeU);
	convertMisZones(mis_zones, mapSizeU);
	convertMisPlayers(mis_players);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSSM(const std::string_view& inputFile)
{
	//MAP
	std::string_view map_info;
	std::string_view map_mini;
	std::string_view map_rhombs;
	std::string_view map_flags;
	std::string_view map_landname;
	std::string_view map_objects;
	std::string_view map_mines;
	//MIS
	std::string_view mis_desc;
	std::string_view mis_unitnames;
	std::string_view mis_groups;
	std::string_view mis_mapunits;
	std::string_view mis_players;
	std::string_view mis_woofers;
	std::string_view mis_zones;
	std::string_view mis_scripts;
	std::string_view mis_support;
	std::string_view mis_phrases;
	std::string_view mis_objects;
	
	// Определите начальную позицию и количество байт для извлечения
	uint32_t mapIdentifier = readFileUint32(inputFile, 40);
	uint32_t mapSizeU = readFileUint32(inputFile, 44);
	uint32_t mapSizeV = readFileUint32(inputFile, 48);
	uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);
	
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
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
	
	std::filesystem::create_directories(m_misFolder);
	
	//MAP
	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs, mapIdentifier);
	convertMapDesc();
	convertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	convertMapMini(map_mini);
	convertMapObjects(map_objects);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	//MIS
	convertMisDesc();
	convertMisMult(map_info, 0);
	convertMisInfo(mapSizeU, mapSizeV);
	convertMisTree(mis_desc);
	convertMisZones(mis_zones, mapSizeU);
	convertMisMines(map_mines, mapSizeU);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	convertMisScripts(mis_scripts);
	convertMisWoofers(mis_woofers);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisObjects(mis_objects);
	convertMisGroups(mis_groups);
	convertMisPlayers(mis_players);
	
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSSC_map(const std::string_view& inputFile)
{
	std::string_view map_info;
	std::string_view map_landname;
	std::string_view map_mini;
	std::string_view map_mini2;
	std::string_view map_rhombs;
	std::string_view map_flags;
	std::string_view map_objects;
	
	// Определите начальную позицию и количество байт для извлечения
	// Начальная позиция нужных байтов
	uint32_t mapIdentifier = readFileUint32(inputFile, 8);
	uint32_t mapSizeU = readFileUint32(inputFile, 12);
	uint32_t mapSizeV = readFileUint32(inputFile, 16);
	
	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);
	
	uint32_t startPositionLandname = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSSC_map);
	uint32_t startPositionRhombs = position(inputFile, map_landname, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags = position(inputFile, map_rhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionMapMini = position(inputFile, map_flags, startPositionFlags, flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, map_mini, startPositionMapMini, rhombsSize);
	uint32_t startPositionObjects = position(inputFile, map_mini2, startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t mapEndPosition = position(inputFile, map_objects, startPositionObjects + 4, mapSizeObjects * 8);
	
	
	std::filesystem::create_directories(m_mapFolder);
	
	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs, mapIdentifier);
	convertMapInfo(mapIdentifier, mapSizeU, mapSizeV);
	convertMapMini(map_mini);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	convertMapObjects(map_objects);
	convertMapDesc();
	
	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSCC_mission(const std::string_view& inputFile)
{
	std::string_view mis_info;
	std::string_view map_mines;
	std::string_view mis_desc;
	std::string_view mis_unitnames;
	std::string_view mis_groups;
	std::string_view mis_mapunits;
	std::string_view mis_players;
	std::string_view mis_maunits;
	std::string_view mis_woofers;
	std::string_view mis_zones;
	std::string_view mis_scripts;
	std::string_view mis_support;
	std::string_view mis_phrases;
	std::string_view mis_objects;

	const uint32_t mapSizeU = readFileUint32(inputFile, 16);
	const uint32_t mapSizeV = readFileUint32(inputFile, 20);

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

	std::filesystem::create_directories(m_misFolder);
	convertMisDesc();
	convertMisMult(mis_info, 2);
	convertMisInfo(mapSizeU, mapSizeV);
	convertMisZones(mis_zones, mapSizeU);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	convertMisGroups(mis_groups);
	convertMisScripts(mis_scripts);
	convertMisWoofers(mis_woofers);
	convertMisMines(map_mines, mapSizeU);
	convertMisTree(mis_desc);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisObjects(mis_objects);
	convertMisPlayers(mis_players);

	displayinfo(mapSizeU, mapSizeV, 40, mapEndPosition);
	return 0;
}

#pragma pack(push, 1)
struct MAP_INFO_HEADER
{
	MAP_INFO_HEADER(uint32_t mapIdentifier, uint32_t mapSizeU, uint32_t mapSizeV)
	{
		this->mapIdentifier = mapIdentifier;
		this->mapSizeU = mapSizeU;
		this->mapSizeV = mapSizeV;
		unknown_1 = GLOBALNULL;
		unknown_2 = GLOBALNULL;
		unknown_3 = GLOBALNULL;
	}

private:
	uint32_t mapIdentifier;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
	uint8_t unknown_1;
	uint8_t unknown_2;
	uint8_t unknown_3;
};
#pragma pack (pop)

void Converter::convertMapInfo(const uint32_t mapIdentifier, const uint32_t mapSizeU, const uint32_t mapSizeV) const
{
	std::ofstream outputMapInfo(m_mapFolder / "info", std::ios::binary);
	if (!outputMapInfo)
	{
		errorWriteFile();
		return;
	}
	MAP_INFO_HEADER part1(mapIdentifier, mapSizeU, mapSizeV);
	outputMapInfo.write(reinterpret_cast<const char*>(&part1), sizeof(part1));
	outputMapInfo.close();
}

struct BITMAPINFOHEADER_
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	long	 biXPelsPerMeter;
	long	 biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

struct RGBQUAD_
{
	uint32_t rgbBlue;
	uint32_t rgbGreen;
	uint32_t rgbRed;
	//uint32_t rgbReserved;
};

void Converter::convertMapMini(const std::string_view& map_mini) const
{
	std::ofstream outputFileMapMiniBMP(m_mapFolder / "map_mini.bmp", std::ios::binary);
	if (!outputFileMapMiniBMP)
	{
		errorWriteFile();
		return;
	}

	const size_t size = map_mini.size();
	BITMAPFILEHEADER part1{};
	BITMAPINFOHEADER part2{};
	RGBQUAD_ part3{};
	
	part1.bfType = 19778;
	part1.bfSize = static_cast<DWORD>(size) + (sizeof(part1)) + (sizeof(part2)) + (sizeof(part3));
	part1.bfReserved1 = GLOBALNULL;
	part1.bfReserved2 = GLOBALNULL;
	part1.bfOffBits = (sizeof(part1)) + (sizeof(part2)) + (sizeof(part3));
	
	part2.biSize = sizeof(part2);
	part2.biWidth = static_cast<LONG>(sqrt(size / 2));
	part2.biHeight = static_cast<LONG>(sqrt(size / 2));
	part2.biPlanes = 1;
	part2.biBitCount = 16;
	part2.biCompression = 3;
	part2.biSizeImage = GLOBALNULL;
	part2.biXPelsPerMeter = GLOBALNULL;
	part2.biYPelsPerMeter = GLOBALNULL;
	part2.biClrUsed = GLOBALNULL;
	part2.biClrImportant = GLOBALNULL;
	
	part3.rgbBlue = 63488;
	part3.rgbGreen = 2016;
	part3.rgbRed = 31;
	
	outputFileMapMiniBMP.write(reinterpret_cast<const char*>(&part1), 14);
	outputFileMapMiniBMP.write(reinterpret_cast<const char*>(&part2), 40);
	outputFileMapMiniBMP.write(reinterpret_cast<const char*>(&part3), 12);

	outputFileMapMiniBMP.write(reinterpret_cast<const char*>(map_mini.data()), size);
	outputFileMapMiniBMP.close();
}

void Converter::convertMapLandname(const std::string_view& map_landname) const
{
	std::ofstream outputMapLandname(m_mapFolder / "landname", std::ios::binary);
	if (!outputMapLandname)
	{
		errorWriteFile();
		return;
	}

	outputMapLandname.write(reinterpret_cast<const char*>(map_landname.data()), map_landname.size());
	outputMapLandname.close();
}

void Converter::convertMapCflags(const std::string_view& map_flags) const
{
	std::ofstream outputMapCflags(m_mapFolder / "cflags", std::ios::binary);
	if (!outputMapCflags)
	{
		errorWriteFile();
		return;
	}

	const size_t length = map_flags.size();
	std::vector<uint8_t> mapFlags(length / 2, 0);

	for (size_t offset = 0, vOffset = 0; offset < length; offset += 4, vOffset += 2)
	{
		const uint8_t num1 = map_flags[offset];
		const uint8_t num2 = map_flags[offset + 1];
		const uint8_t num3 = map_flags[offset + 2];
		//const uint8_t num4 = map_flags[offset + 3];	// not used

		//Тернарные операторы
		const uint8_t zoneBlocker = ((num1 & 0xF0) > 0 ? GLOBALNULL : (num1 & 0x0F));
		const uint8_t waves = ~num2 >> 1;
		const uint8_t noPantones = ((num2 & 0x0F) == 6 ? ((num3 & 0x0F) > 0 ? 0 : 1) : 0);

		//Работа с битами
		const uint8_t cflags = noPantones << 7 | (waves & 0x40) | (num2 & 0x30) | zoneBlocker;

		mapFlags[vOffset] = cflags;
		//mapFlags[vOffset + 1] = 0;	// not needed
	}
	outputMapCflags.write(reinterpret_cast<const char*>(mapFlags.data()), mapFlags.size());
	outputMapCflags.close();
}

void Converter::convertMapObjects(const std::string_view& map_objects) const
{
	std::ofstream outputMapObjects(m_mapFolder / "objects", std::ios::binary);
	if (!outputMapObjects)
	{
		errorWriteFile();
		return;
	}

	outputMapObjects.write(reinterpret_cast<const char*>(map_objects.data()), map_objects.size());
	outputMapObjects.close();
}

void Converter::convertMapDesc() const
{
	std::ofstream outputFileMapDesc(m_mapFolder / "desc", std::ios::binary);
	if (!outputFileMapDesc)
	{
		errorWriteFile();
		return;
	}

	switch (m_mapType)
	{
	case HEADER_SINGLE:
	case HEADER_MULTI:
		outputFileMapDesc.write(m_stemFileName.data(), m_stemFileName.length());
	}

	outputFileMapDesc.close();
}

void Converter::convertMapRhombs(const std::string_view& map_rhombs, const uint32_t mapIdentifier) const
{
	std::ofstream outputFile(m_mapFolder / "rhombs", std::ios::binary);
	if (!outputFile)
	{
		errorWriteFile();
		return;
	}

	RhombsParser::parse_rhombs(map_rhombs, outputFile, static_cast<RhombsParser::SchemeType>(mapIdentifier));

	outputFile.close();
}

void Converter::convertMisDesc() const
{
	std::ofstream outputFileMisDesc(m_misFolder / "desc", std::ios::binary);
	if (!outputFileMisDesc)
	{
		errorWriteFile();
		return;
	}
	outputFileMisDesc.close();
}

void Converter::convertMisMult(const std::string_view& map_info, const uint32_t maptypeheader) const
{
	std::ofstream outputMapMisMult(m_misFolder / "mismult", std::ios::binary);
	if (!outputMapMisMult)
	{
		errorWriteFile();
		return;
	}

	if (maptypeheader == 0 || maptypeheader == 1)
	{
		outputMapMisMult.write(reinterpret_cast<const char*>(map_info.data()) + 24, 16);
	}
	else
	{
		for (uint32_t n = 0; n < 16; n++)
		{
			outputMapMisMult << GLOBALNULL;
		}
	}
	outputMapMisMult.close();
}

#pragma pack(push, 1)
struct MIS_INFO_HEADER
{
private:
	uint32_t unknown_1;
	uint32_t unknown_2;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
public:
	MIS_INFO_HEADER(uint32_t mapSizeU, uint32_t mapSizeV)
	{
		unknown_1 = GLOBALNULL;
		unknown_2 = GLOBALNULL;
		this->mapSizeU = mapSizeU;
		this->mapSizeV = mapSizeV;
	}
};
#pragma pack (pop)

void Converter::convertMisInfo(const uint32_t mapSizeU, const uint32_t mapSizeV) const
{
	std::ofstream outputMisInfo(m_misFolder / "info", std::ios::binary);
	if (!outputMisInfo)
	{
		errorWriteFile();
		return;
	}

	MIS_INFO_HEADER part1(mapSizeU, mapSizeV);
	outputMisInfo.write(reinterpret_cast<const char*>(&part1), sizeof(part1));
	outputMisInfo.close();
}

void Converter::convertMisZones(const std::string_view& mis_zones, const uint32_t mapSizeU) const
{
	std::ofstream outputFileMisZones(m_misFolder / "locations", std::ios::binary);
	if (!outputFileMisZones)
	{
		errorWriteFile();
		return;
	}

	std::vector<uint8_t> misZones(LOCATIONSSIZE, 0);

	const size_t maxTries = mis_zones.size() / mapSizeU;
	for (size_t i = 0; i < maxTries; ++i)
	{
		std::copy(mis_zones.cbegin() + i * mapSizeU, mis_zones.cbegin() + i * mapSizeU + mapSizeU, misZones.begin() + 512 * i);
	}

	outputFileMisZones.write(reinterpret_cast<const char*>(misZones.data()), misZones.size());
	outputFileMisZones.close();
}

void Converter::convertMisUnits(const std::string_view& mis_unitnames, const std::string_view& mis_mapunits, const std::string_view& mis_support) const
{
	std::ofstream outputFileMapUnits(m_misFolder / "mapunits", std::ios::binary);
	std::ofstream outputFileSupport(m_misFolder / "support", std::ios::binary);
	if (!outputFileMapUnits || !outputFileSupport)
	{
		errorWriteFile();
		return;
	}
	
	std::map <int, std::string> nameunit;
	int numOfNames = 0;
	while (numOfNames * 16 < mis_unitnames.size())
	{
		nameunit.emplace(numOfNames, (const char*)(mis_unitnames.data() + numOfNames * 16));
		++numOfNames;
	}

	uint32_t numberofunit = *(uint32_t*)(mis_mapunits.data());
	
	// mapunits file will have a reversed strings comparing to the original editor mapunits file
	size_t curOffset = sizeof(numberofunit);
	while (curOffset + 13 <= mis_mapunits.size())
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_mapunits.data()) + curOffset;
		curOffset += 13;

		uint8_t GRP = *(uint8_t*)(buffer + 0);
		uint8_t HP = *(uint8_t*)(buffer + 1);
		uint8_t Ammo = *(uint8_t*)(buffer + 2);
		uint8_t Expa = *(uint8_t*)(buffer + 3);
		uint8_t Lives = *(uint8_t*)(buffer + 4);
		uint8_t ID = *(uint8_t*)(buffer + 5);
		uint8_t In = *(uint8_t*)(buffer + 6);
		uint16_t U = *(uint16_t*)(buffer + 7);
		uint16_t V = *(uint16_t*)(buffer + 9);
		uint8_t Dir = *(uint8_t*)(buffer + 11);
		uint8_t Owner = *(uint8_t*)(buffer + 12);
		{
			outputFileMapUnits << std::format("ID={} GRP={} HP={} Ammo={} Expa={} Lives={} U={} V={} Dir={} In={} Owner={}\n"
				, nameunit[ID]
				, (uint16_t)GRP
				, (uint16_t)HP
				, (uint16_t)Ammo
				, (uint16_t)Expa
				, (uint16_t)Lives
				, (uint16_t)U
				, (uint16_t)V
				, (uint16_t)Dir
				, (uint16_t)In
				, (uint16_t)Owner);
		}
		if (In > 0)
		{
			while (In--)
			{
				const uint8_t* unitBuffer = reinterpret_cast<const uint8_t*>(mis_mapunits.data()) + curOffset;
				curOffset += 6;

				uint8_t GRPpassenger = *(uint8_t*)(unitBuffer + 0);
				uint8_t HPpassenger = *(uint8_t*)(unitBuffer + 1);
				uint8_t Ammopassenger = *(uint8_t*)(unitBuffer + 2);
				uint8_t Expapassenger = *(uint8_t*)(unitBuffer + 3);
				uint8_t Livespassenger = *(uint8_t*)(unitBuffer + 4);
				uint8_t IDpassenger = *(uint8_t*)(unitBuffer + 5);
				{
					outputFileMapUnits << std::format(" ID={} GRP={} HP={} Ammo={} Expa={} Lives={}\n"
						, nameunit[IDpassenger]
						, (uint16_t)GRPpassenger
						, (uint16_t)HPpassenger
						, (uint16_t)Ammopassenger
						, (uint16_t)Expapassenger
						, (uint16_t)Livespassenger);
				}
			}
		}
	}
	
	const uint32_t supportSize = *(uint32_t*)(mis_support.data());	// not used

	uint32_t flag_num = 16;
	uint32_t script_num = 64;
	curOffset = sizeof(supportSize);
	while (flag_num--)
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_support.data()) + curOffset;
		curOffset += 20;

		int num1 = *(uint32_t*)(buffer + 0);
		int num2 = *(uint32_t*)(buffer + 4);
		int num3 = *(uint32_t*)(buffer + 8);
		int num4 = *(uint32_t*)(buffer + 12);
		int num5 = *(uint32_t*)(buffer + 16);
		//cout << "flag " << num1 << " " << num2 << " " << num5 << " " << num3 << " " << num4 << '\n';
		outputFileSupport << std::format("flag {},{},{},{},{}\n"
			, num1
			, num2
			, num5
			, num3
			, num4);
	}
	//cout << '\n';
	outputFileSupport << '\n';
	
	uint32_t accumulator = 0;
	while (script_num--)
	{
		const uint8_t NameSize = mis_support[324 + accumulator];

		if (NameSize > 0)
		{
			const uint8_t* buffername = reinterpret_cast<const uint8_t*>(mis_support.data()) + 324 + 1 + accumulator;
			//cout << "support " << "\"" << buffername << "\"" << '\n';
			outputFileSupport << "support \"";
			outputFileSupport.write((const char*)buffername, NameSize);
			outputFileSupport << "\"\n";
		}
		else
		{
			//cout << "support " << "\"" << "\"" << '\n';
			outputFileSupport << "support \"\"\n";
		}
		
		uint32_t UnitScriptSize = *(uint32_t*)(mis_support.data() + 324 + 1 + accumulator + NameSize);
		curOffset = 324 + 1 + accumulator + NameSize + sizeof(UnitScriptSize);
		accumulator += 5 + NameSize;
		if (UnitScriptSize > 0)
		{
			while (UnitScriptSize--)
			{
				const uint8_t* bufferunit = reinterpret_cast<const uint8_t*>(mis_support.data()) + curOffset;
				curOffset += 7;

				uint8_t GRP = *(uint8_t*)(bufferunit + 0);
				uint8_t HP = *(uint8_t*)(bufferunit + 1);
				uint8_t Ammo = *(uint8_t*)(bufferunit + 2);
				uint8_t Expa = *(uint8_t*)(bufferunit + 3);
				uint8_t Lives = *(uint8_t*)(bufferunit + 4);
				uint8_t Id = *(uint8_t*)(bufferunit + 5);
				uint8_t In = *(uint8_t*)(bufferunit + 6);
				accumulator += 7;
				{
					//cout << "ID=" << Id << " GRP=" << GRP << " HP=" << HP << " Ammo=" << Ammo << " Expa=" << Expa << " Lives=" << Lives << " In=" << In << '\n';
					outputFileSupport << std::format(" ID={} Grp={} HP={} Ammo={} Expa={} Lives={} In={}\n"
						, nameunit[Id]
						, (uint16_t)GRP
						, (uint16_t)HP
						, (uint16_t)Ammo
						, (uint16_t)Expa
						, (uint16_t)Lives
						, (uint16_t)In);
				}
				if (In > 0)
				{
					while (In--)
					{
						const uint8_t* bufferUnit = reinterpret_cast<const uint8_t*>(mis_support.data()) + curOffset;
						curOffset += 6;

						uint8_t GRPpasseenger = *(uint8_t*)(bufferUnit + 0);
						uint8_t HPpasseenger = *(uint8_t*)(bufferUnit + 1);
						uint8_t Ammopasseenger = *(uint8_t*)(bufferUnit + 2);
						uint8_t Expapasseenger = *(uint8_t*)(bufferUnit + 3);
						uint8_t Livespasseenger = *(uint8_t*)(bufferUnit + 4);
						uint8_t Idpasseenger = *(uint8_t*)(bufferUnit + 5);
						accumulator += 6;
						{
							//cout << "ID=" << Idpasseenger << " GRP=" << GRPpasseenger << " HP=" << HPpasseenger << " Ammo=" << Ammopasseenger << " Expa=" << Expapasseenger << " Lives=" << Livespasseenger << '\n';
							outputFileSupport << std::format("  ID={} Grp={} HP={} Ammo={} Expa={} Lives={}\n"
								, nameunit[Idpasseenger]
								, (uint16_t)GRPpasseenger
								, (uint16_t)HPpasseenger
								, (uint16_t)Ammopasseenger
								, (uint16_t)Expapasseenger
								, (uint16_t)Livespasseenger);
						}
					}
				}
			}
		}
		//cout << "end" << '\n';
		outputFileSupport << "end\n";
	}
	
	outputFileMapUnits.close();
	outputFileSupport.close();
}

void Converter::convertMisGroups(const std::string_view& mis_groups) const
{
	std::ofstream outputFileMisGroups(m_misFolder / "groups", std::ios::binary);
	if (!outputFileMisGroups)
	{
		errorWriteFile();
		return;
	}
	constexpr std::array <std::string_view, 16> AI_type_array =
	{
		"\"ai_none\"",
		"\"ai_recon\"",
		"\"ai_sold_guard\"",
		"\"ai_sold_follow\"",
		"\"ai_sold_art\"",
		"\"ai_tank_guard\"",
		"\"ai_tank_follow\"",
		"\"ai_furg_help\"",
		"\"ai_furg_move\"",
		"\"ai_gruz_reload\"",
		"\"ai_katya_move\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\""
	};
	
	const size_t maxTries = mis_groups.size() / 27;
	for (size_t accumulator = 0; accumulator < maxTries; ++accumulator)
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_groups.data()) + accumulator * 27;
		uint8_t aitype1 = *(uint8_t*)(buffer + 0);
		uint8_t aitype2 = *(uint8_t*)(buffer + 1);
		uint8_t aitype3 = *(uint8_t*)(buffer + 2);
		uint8_t aitype4 = *(uint8_t*)(buffer + 3);

		uint8_t zone1 = *(uint8_t*)(buffer + 4);
		uint8_t zone2 = *(uint8_t*)(buffer + 5);
		uint8_t group1 = *(uint8_t*)(buffer + 6);
		uint8_t group2 = *(uint8_t*)(buffer + 7);
		uint8_t flag = *(uint8_t*)(buffer + 8);
		uint8_t zone = *(uint8_t*)(buffer + 9);

		uint8_t atime1 = *(uint8_t*)(buffer + 10);
		uint8_t atime2 = *(uint8_t*)(buffer + 11);
		uint8_t atime3 = *(uint8_t*)(buffer + 12);
		uint8_t atime4 = *(uint8_t*)(buffer + 13);

		uint8_t delay1 = *(uint8_t*)(buffer + 14);
		uint8_t delay2 = *(uint8_t*)(buffer + 15);
		uint8_t delay3 = *(uint8_t*)(buffer + 16);
		uint8_t delay4 = *(uint8_t*)(buffer + 17);

		uint16_t min = *(uint16_t*)(buffer + 18);

		uint8_t force1 = *(uint8_t*)(buffer + 20);
		uint8_t force2 = *(uint8_t*)(buffer + 21);
		uint8_t force3 = *(uint8_t*)(buffer + 22);
		uint8_t force4 = *(uint8_t*)(buffer + 23);

		uint8_t hp = *(uint8_t*)(buffer + 24);
		uint8_t ammo = *(uint8_t*)(buffer + 25);
		uint8_t expa = *(uint8_t*)(buffer + 26);
		
		{
			const uint8_t AI_type = (uint8_t)(aitype1 & 0x0F);
			outputFileMisGroups << std::format("Group {}\n ai type={} group1={} group2={} zone1={} zone2={}\n  aiflags="
				, accumulator
				, AI_type_array[AI_type]
				, (uint16_t)group1
				, (uint16_t)group2
				, (uint16_t)zone1
				, (uint16_t)zone2);

			if (AI_type != 0 && AI_type != 1)
				aiFlagsConvert(outputFileMisGroups, aitype1, aitype2, aitype3, aitype4);

			outputFileMisGroups << '\n';
		}

		
		if (atime1 == 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
		{
			outputFileMisGroups << " reserv auto=0 delay=0 min=0 force=0 atime=0";
		}
		else
			if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
			{
				uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
				uint32_t convertatime = atime1234 / timeconvertnum;
				outputFileMisGroups << std::format(" reserv auto=1 delay=0 min={} force=0 atime={}", min, convertatime);
			}
			else
				if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
				{
					uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
					uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
					uint32_t convertatime = atime1234 / timeconvertnum;
					uint32_t convertdelay = delay1234 / timeconvertnum;
					outputFileMisGroups << std::format(" reserv auto=3 delay={} min={} force=0 atime={}", convertdelay, min, convertatime);
				}
				else
					if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && (min >= 1 && min <= 98) && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
					{
						uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
						uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
						uint32_t convertatime = atime1234 / timeconvertnum;
						uint32_t convertdelay = delay1234 / timeconvertnum;
						outputFileMisGroups << std::format(" reserv auto=7 delay={} min={} force=0 atime={}", convertdelay, min, convertatime);
					}
					else
						if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && min == 0 && (force3 >= 1 && force3 <= 13))
						{
							uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
							uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
							uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
							uint32_t convertatime = atime1234 / timeconvertnum;
							uint32_t convertdelay = delay1234 / timeconvertnum;
							uint32_t convertforce = force1234 / timeconvertnum;
							outputFileMisGroups << std::format(" reserv auto=11 delay={} min={} force={} atime={}", convertdelay, min, convertforce, convertatime);
						}
						else
							if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && (force3 >= 1 && force3 <= 13))
							{
								uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
								uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
								uint32_t convertatime = atime1234 / timeconvertnum;
								uint32_t convertforce = force1234 / timeconvertnum;
								outputFileMisGroups << std::format(" reserv auto=9 delay=0 min=0 force={} atime={}", convertforce, convertatime);
							}
							else
								if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && (min >= 1 && min <= 98) && (force3 >= 1 && force3 <= 13))
								{
									uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
									uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
									uint32_t convertatime = atime1234 / timeconvertnum;
									uint32_t convertforce = force1234 / timeconvertnum;
									outputFileMisGroups << std::format(" reserv auto=13 delay=0 min={} force={} atime={}", min, convertforce, convertatime);
								}
								else
								{
									uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
									uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
									uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
									uint32_t convertatime = atime1234 / timeconvertnum;
									uint32_t convertdelay = delay1234 / timeconvertnum;
									uint32_t convertforce = force1234 / timeconvertnum;
									outputFileMisGroups << std::format(" reserv auto=15 delay={} min={} force={} atime={}", convertdelay, min, convertforce, convertatime);
								}
		
		{
			outputFileMisGroups << std::format(" flag={} zone={} hp={} ammo={} expa={}\n"
				, (uint16_t)flag
				, (uint16_t)zone
				, (uint16_t)hp
				, (uint16_t)ammo
				, (uint16_t)expa);
		}
		
	}
	outputFileMisGroups.close();
}

void Converter::convertMisScripts(const std::string_view& mis_scripts) const
{
	std::ofstream outputFile(m_misFolder / "scripts2", std::ios::binary);
	if (!outputFile)
	{
		errorWriteFile();
		return;
	}
	
	uint32_t numberofscripts = *(uint32_t*)mis_scripts.data();

	//Скрипты1
	const uint8_t bufferAND[4] = { 1, 0, 0, 0 };
	const uint8_t bufferOR[4] = { 2, 0, 0, 0 };
	const uint8_t bufferUG[4] = { 3, 0, 0, 0 };
	const uint8_t bufferUP[4] = { 4, 0, 0, 0 };
	const uint8_t bufferUGL[4] = { 5, 0, 0, 0 };
	const uint8_t bufferUPL[4] = { 6, 0, 0, 0 };
	const uint8_t bufferPKP[4] = { 9, 0, 0, 0 };
	const uint8_t bufferPKF[4] = { 10, 0, 0, 0 };
	const uint8_t bufferTE[4] = { 27, 0, 0, 0 };
	const uint8_t bufferCD[4] = { 28, 0, 0, 0 };
	const uint8_t bufferTMS[4] = { 29, 0, 0, 0 };
	const uint8_t bufferUGLper[4] = { 30, 0, 0, 0 };
	const uint8_t bufferUPLper[4] = { 31, 0, 0, 0 };
	const uint8_t bufferUGper[4] = { 32, 0, 0, 0 };
	const uint8_t bufferUPper[4] = { 33, 0, 0, 0 };
	const uint8_t bufferAIGB[4] = { 34, 0, 0, 0 };
	const uint8_t bufferAIZ1[4] = { 35, 0, 0, 0 };
	const uint8_t bufferAIZ2[4] = { 36, 0, 0, 0 };
	const uint8_t bufferAIG1[4] = { 37, 0, 0, 0 };
	const uint8_t bufferAIG2[4] = { 38, 0, 0, 0 };
	const uint8_t bufferOID[4] = { 41, 0, 0, 0 };
	const uint8_t bufferGWATA[4] = { 49, 0, 0, 0 };
	const uint8_t bufferVIC[4] = { 50, 0, 0, 0 };
	const uint8_t bufferMIST[4] = { 51, 0, 0, 0 };

	//Скрипты2
	const uint8_t bufferSPPL[4] = { 8, 0, 0, 0 };
	const uint8_t bufferETC[4] = { 11, 0, 0, 0 };
	const uint8_t bufferSTRT[4] = { 12, 0, 0, 0 };
	const uint8_t bufferSTPT[4] = { 13, 0, 0, 0 };
	const uint8_t bufferMSTL[4] = { 14, 0, 0, 0 };
	const uint8_t bufferSP[4] = { 15, 0, 0, 0 };
	const uint8_t bufferSCD[4] = { 16, 0, 0, 0 };
	const uint8_t bufferSNM[4] = { 17, 0, 0, 0 };
	const uint8_t bufferTM[4] = { 18, 0, 0, 0 };
	const uint8_t bufferSGB[4] = { 19, 0, 0, 0 };
	const uint8_t bufferSGL1[4] = { 20, 0, 0, 0 };
	const uint8_t bufferSGL2[4] = { 21, 0, 0, 0 };
	const uint8_t bufferSGG1[4] = { 22, 0, 0, 0 };
	const uint8_t bufferSGG2[4] = { 23, 0, 0, 0 };
	const uint8_t bufferAPP[4] = { 24, 0, 0, 0 };
	const uint8_t bufferAFP[4] = { 25, 0, 0, 0 };
	const uint8_t bufferRU[4] = { 39, 0, 0, 0 };
	const uint8_t bufferSRFS[4] = { 40, 0, 0, 0 };
	const uint8_t bufferSPTO[4] = { 42, 0, 0, 0 };
	const uint8_t bufferSAT[4] = { 43, 0, 0, 0 };
	const uint8_t bufferARPO[4] = { 44, 0, 0, 0 };
	const uint8_t bufferARPO2[4] = { 45, 0, 0, 0 };
	const uint8_t bufferSPPA[4] = { 46, 0, 0, 0 };
	const uint8_t bufferLCCV[4] = { 47, 0, 0, 0 };
	const uint8_t bufferMO[4] = { 48, 0, 0, 0 };
	const uint8_t bufferSRES[4] = { 52, 0, 0, 0 };
	const uint8_t bufferFRES[4] = { 53, 0, 0, 0 };
	const uint8_t bufferPFF[4] = { 54, 0, 0, 0 };
	const uint8_t bufferFPFF[4] = { 55, 0, 0, 0 };
	const uint8_t bufferMFF[4] = { 56, 0, 0, 0 };
	const uint8_t bufferSPPO[4] = { 57, 0, 0, 0 };
	const uint8_t bufferDGP[4] = { 58, 0, 0, 0 };

	//Старт, конец
	const uint8_t bufferEND[4] = { 7, 0, 0, 0 };
	//uint8_t bufferstart[4] = { 67, 65, 79, 70 };
	const uint8_t bufferEND2[4] = { 255, 255, 255, 127 };
	
	std::stringstream bufferScripts;
	std::stringstream bufferScriptsOPN;
	std::stringstream bufferScripts_OPN1;
	std::stringstream outputFileBuffer;
	
	uint32_t accumulatorNumberScripts = 1;
	

	size_t curOffset{ sizeof(numberofscripts) };
	while (numberofscripts--)
	{
		const uint8_t* sizeBuffer = reinterpret_cast<const uint8_t*>(mis_scripts.data()) + curOffset;
		curOffset += 8;
		uint8_t sizescripts1 = *(uint8_t*)(sizeBuffer + 0);
		uint8_t sizescripts2 = *(uint8_t*)(sizeBuffer + 1);
		uint8_t sizescripts3 = *(uint8_t*)(sizeBuffer + 2);
		uint8_t sizescripts4 = *(uint8_t*)(sizeBuffer + 3);

		//uint8_t startscripts1 = *(uint8_t*)(buffer + 4);			// not used
		//uint8_t startscripts2 = *(uint8_t*)(buffer + 5);
		//uint8_t startscripts3 = *(uint8_t*)(buffer + 6);
		//uint8_t startscripts4 = *(uint8_t*)(buffer + 7);
		uint32_t sizescripts = ((sizescripts4 & 0xFF) << 24) | ((sizescripts3 & 0xFF) << 16) | ((sizescripts2 & 0xFF) << 8) | (sizescripts1 & 0xFF);

		uint32_t numberoflines = (sizescripts - 4) / 4; //расчет количества циклов в одном скрипте
		std::map <uint32_t, std::string> numberscripts;
		uint32_t n = 0; //присвоение ключа к map
		uint32_t key = 0; //ключ к map
		uint32_t scriptNumber = 0; //номер собранных скриптовых комманд в одном скрипте, нужно для выполнения обратной польской нотации

		while (numberoflines--)
		{
			uint8_t logicOperator = 0; //Логический оператор И ИЛИ

			const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_scripts.data()) + curOffset;
			curOffset += 4;

			uint8_t num1 = *(uint8_t*)(buffer + 0);
			uint8_t num2 = *(uint8_t*)(buffer + 1);
			uint8_t num3 = *(uint8_t*)(buffer + 2);
			uint8_t num4 = *(uint8_t*)(buffer + 3);
			{
				//Переменные значения
				if (num4 == 128)
				{
					if (num2 == 0)
					{
						// Реверс числа
						std::string str;
						reverse_num(num1, str);
						//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
						numberscripts[n] = str;
						//std::cout << key << ": " << str << std::endl;
						n++;
					}
					else
					{
						// Реверс числа
						std::string str;
						uint16_t num12 = ((num2 & 0xFF) << 8) | (num1 & 0xFF);
						//char buffertwobytes[2] = { num1, num2 };
						//SHORT num12 = *(unsigned short*)(buffertwobytes + 0);
						reverse_num(num12, str);
						//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
						numberscripts[n] = str;
						//std::cout << key << ": " << str << std::endl;
						n++;
					}

				}
				else if (num2 == 255 && num3 == 255 && num4 == 255) // Операция с координатой ячеек памяти
				{
					// Реверс числа
					std::string str;
					uint32_t num1234 = 392 + num1;
					reverse_num(num1234, str);
					//outputFile << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
					numberscripts[n] = str;
					//std::cout << key << ": " << str << std::endl;
					n++;
				}

				//Скрипты в первом окне редактора карт QED (они же подвержены обратной польской нотации)
				else if (memcmp(buffer, bufferAND, 4) == 0) // 1, 0, 0, 0
				{
					//Если оператор "И"
					logicOperator = 1;
				}
				else if (memcmp(buffer, bufferOR, 4) == 0) // 2, 0, 0, 0
				{
					//Если оператор "ИЛИ"
					logicOperator = 2;
				}
				else if (memcmp(buffer, bufferUG, 4) == 0) // 3, 0, 0, 0
				{
					bufferScripts_OPN1 << "$ug" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$ug1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_8" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sp_9" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUP, 4) == 0) // 4, 0, 0, 0
				{
					bufferScripts_OPN1 << "$up" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$up1" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_b" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sp_c" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUGL, 4) == 0) // 5, 0, 0, 0
				{
					bufferScripts_OPN1 << "$ugl" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$umx0" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$ugl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUPL, 4) == 0) // 6, 0, 0, 0
				{
					bufferScripts_OPN1 << "$upl" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$umx2" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$upl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferPKP, 4) == 0) // 9, 0, 0, 0
				{
					bufferScripts_OPN1 << "$pkp" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$pkp1" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_d" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sp_e" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferPKF, 4) == 0) // 10, 0, 0, 0
				{
					bufferScripts_OPN1 << "$pkf" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$pkf1" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_f" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$pkf2" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferTE, 4) == 0) // 27, 0, 0, 0
				{
					bufferScripts_OPN1 << "$te" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key] << '\n';
					key++;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferCD, 4) == 0) // 28, 0, 0, 0
				{
					bufferScripts_OPN1 << "$cd" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sp_5" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferTMS, 4) == 0) // 29, 0, 0, 0
				{
					bufferScripts_OPN1 << "$tms" << '\n' << "$CAB" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sp_6" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUGLper, 4) == 0) // 30, 0, 0, 0
				{
					bufferScripts_OPN1 << "$ugl%" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$umx1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$ugl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUPLper, 4) == 0) // 31, 0, 0, 0
				{
					bufferScripts_OPN1 << "$upl%" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$N1" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$umx3" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$upl1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3] << '\n';
					key += 4;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUGper, 4) == 0) // 32, 0, 0, 0
				{
					bufferScripts_OPN1 << "$ug%u" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$umx4" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_7" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sp_k" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$ugut1" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key + 4]
						<< '\n' << "$ug%u1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 5] << '\n';
					key += 6;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferUPper, 4) == 0) // 33, 0, 0, 0
				{
					bufferScripts_OPN1 << "$up%" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$up%u1" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sp_a" << '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sp_k" << '\n' << "$N1" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$ugut1" << '\n' << "$UMSK" << '\n' << "#" << numberscripts[key + 4]
						<< '\n' << "$up%u2" << '\n' << "$Plr" << '\n' << "#" << numberscripts[key + 5] << '\n';
					key += 6;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferAIGB, 4) == 0) // 34, 0, 0, 0
				{
					bufferScripts_OPN1 << "$aigb" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$aigb1" << '\n' << "$AI" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferAIZ1, 4) == 0) // 35, 0, 0, 0
				{
					bufferScripts_OPN1 << "$aiz1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$aiz_1" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferAIZ2, 4) == 0) // 36, 0, 0, 0
				{
					bufferScripts_OPN1 << "$aiz2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$aiz_2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferAIG1, 4) == 0) // 37, 0, 0, 0
				{
					bufferScripts_OPN1 << "$aig1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$aig_1" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferAIG2, 4) == 0) // 38, 0, 0, 0
				{
					bufferScripts_OPN1 << "$aig2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$aig_2" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key + 1] << '\n';
					key += 2;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferOID, 4) == 0) // 41, 0, 0, 0
				{
					bufferScripts_OPN1 << "$oid" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$oid1" << '\n';
					key++;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferGWATA, 4) == 0) // 49, 0, 0, 0
				{
					bufferScripts_OPN1 << "$gwata" << '\n' << "$Grp" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$gwata1" << '\n' << "$CAB" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$T" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$gwata2" << '\n';
					key += 3;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferVIC, 4) == 0) // 50, 0, 0, 0
				{
					bufferScripts_OPN1 << "$vic" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key] << NUMMEMORY
						<< '\n' << "$CABE" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 2] << '\n';
					key += 3;
					scriptNumber += 1;
				}
				else if (memcmp(buffer, bufferMIST, 4) == 0) // 51, 0, 0, 0
				{
					bufferScripts_OPN1 << "$mist" << '\n';
					scriptNumber += 1;
				}

				//Скрипты во втором окне редактора карт QED
				else if (memcmp(buffer, bufferSPPL, 4) == 0) // 8, 0, 0, 0
				{
					bufferScripts << "$sppl" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sppl1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sppl2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 4;
				}
				else if (memcmp(buffer, bufferETC, 4) == 0) // 11, 0, 0, 0 
				{
					bufferScripts << "$etc" << '\n' << "$" << "\\\\" << "n" << '\n';
				}
				else if (memcmp(buffer, bufferSTRT, 4) == 0) // 12, 0, 0, 0
				{
					bufferScripts << "$strt" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$strt1" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSTPT, 4) == 0) // 13, 0, 0, 0
				{
					bufferScripts << "$stpt" << '\n' << "$Tr" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferMSTL, 4) == 0) // 14, 0, 0, 0
				{
					bufferScripts << "$mstl" << '\n' << "$L" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferSP, 4) == 0) // 15, 0, 0, 0
				{
					bufferScripts << "$sp" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferSCD, 4) == 0) // 16, 0, 0, 0 
				{
					bufferScripts << "$scd" << '\n' << "$T" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferSNM, 4) == 0) // 17, 0, 0, 0
				{
					bufferScripts << "$snm" << '\n' << "$mission" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferTM, 4) == 0) // 18, 0, 0, 0
				{
					bufferScripts << "$tm" << '\n' << "$F" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferSGB, 4) == 0) // 19, 0, 0, 0
				{
					bufferScripts << "$sgb" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sgb1" << '\n' << "$AI" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSGL1, 4) == 0) // 20, 0, 0, 0	
				{
					bufferScripts << "$sgl1" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sgl11" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSGL2, 4) == 0) // 21, 0, 0, 0	
				{
					bufferScripts << "$sgl2" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sgl21" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSGG1, 4) == 0) // 22, 0, 0, 0
				{
					bufferScripts << "$sgg1" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sgg11" << '\n' << "$G" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSGG2, 4) == 0) // 23, 0, 0, 0
				{
					bufferScripts << "$sgg2" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sgg21" << '\n' << "$G" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferAPP, 4) == 0) // 24, 0, 0, 0
				{
					bufferScripts << "$app" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
				}
				else if (memcmp(buffer, bufferAFP, 4) == 0) // 25, 0, 0, 0
				{
					bufferScripts << "$afp" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$afp1" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
				}
				else if (memcmp(buffer, bufferRU, 4) == 0) // 39, 0, 0, 0
				{
					bufferScripts << "$ru" << '\n' << "$G " << '\n' << "#" << numberscripts[key]
						<< '\n' << "$ru1" << '\n' << "$flg" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$ru2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$ru3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$ru4" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 4]
						<< '\n' << "$ru5" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 5]
						<< '\n' << "$ru5" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 6]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 7;
				}
				else if (memcmp(buffer, bufferSRFS, 4) == 0) // 40, 0, 0, 0
				{
					bufferScripts << "$srfs" << '\n' << "$P" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$srfs0" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$srfs1" << '\n' << "$flg" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$srfs2" << '\n' << "$L" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$srfs3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 4]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 5;
				}
				else if (memcmp(buffer, bufferSPTO, 4) == 0) // 42, 0, 0, 0
				{
					bufferScripts << "$spto" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$spto1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$spto2" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 4;
				}
				else if (memcmp(buffer, bufferSAT, 4) == 0) // 43, 0, 0, 0
				{
					bufferScripts << "$sat" << '\n' << "$" << "\\\\" << "n" << '\n';;
				}
				else if (memcmp(buffer, bufferARPO, 4) == 0) // 44, 0, 0, 0
				{
					bufferScripts << "$arpo" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferARPO2, 4) == 0) // 45, 0, 0, 0
				{
					bufferScripts << "$arpo" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key++;
				}
				else if (memcmp(buffer, bufferSPPA, 4) == 0) // 46, 0, 0, 0
				{
					bufferScripts << "$sppa" << '\n' << "$I" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sppa1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sppa2"
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
				}
				else if (memcmp(buffer, bufferLCCV, 4) == 0) // 47, 0, 0, 0 скрипт не до конца рабочий
				{
					bufferScripts << "$lccv" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key] << NUMMEMORY
						<< '\n' << "$lccv1" << '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
					//$Rvar не расшифрован
				}
				else if (memcmp(buffer, bufferMO, 4) == 0) // 48, 0, 0, 0 скрипт не до конца рабочий
				{
					bufferScripts << "$mo" << '\n' << "$Lvar" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$Math" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$spc" << '\n' << "$Rvar" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
					//$Math не расшифрован
				}
				else if (memcmp(buffer, bufferSRES, 4) == 0) // 52, 0, 0, 0
				{
					bufferScripts << "$sres" << '\n' << "$resv" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sres1" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$sres2" << '\n' << "$resv" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$sres3" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 4;
				}
				else if (memcmp(buffer, bufferFRES, 4) == 0) // 53, 0, 0, 0
				{
					bufferScripts << "$fres" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$fres0" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$fres1" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$fres2" << '\n' << "$T" << '\n' << "#" << numberscripts[key + 3]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 4;
				}
				else if (memcmp(buffer, bufferPFF, 4) == 0) // 54, 0, 0, 0
				{
					bufferScripts << "$pff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$spc" << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
				}
				else if (memcmp(buffer, bufferFPFF, 4) == 0) // 55, 0, 0, 0
				{
					bufferScripts << "$fpff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$I" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$fpff1 " << '\n' << "$PT" << '\n' << "#" << numberscripts[key + 2]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 3;
				}
				else if (memcmp(buffer, bufferMFF, 4) == 0) // 56, 0, 0, 0
				{
					bufferScripts << "$mff" << '\n' << "$Fmask" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$spc" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferSPPO, 4) == 0) // 57, 0, 0, 0
				{
					bufferScripts << "$sppo" << '\n' << "$phrs" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$sppo1" << '\n' << "$Obj" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}
				else if (memcmp(buffer, bufferDGP, 4) == 0) // 58, 0, 0, 0
				{
					bufferScripts << "$dgp" << '\n' << "$G" << '\n' << "#" << numberscripts[key]
						<< '\n' << "$dgp1" << '\n' << "$P" << '\n' << "#" << numberscripts[key + 1]
						<< '\n' << "$" << "\\\\" << "n" << '\n';
					key += 2;
				}

				//END - разделитель между двумя окнами скриптов в редакторе карт QED
				else if (memcmp(buffer, bufferEND, 4) == 0) // 7, 0, 0, 0
				{
					bufferScripts << "$END" << '\n' << "!" << '\n';
				}
				//END - конец скрипта
				else if (memcmp(buffer, bufferEND2, 4) == 0) // 255, 255, 255, 127
				{
					bufferScripts << "$END" << '\n' << '\r' << '\n';
				}


				//Неизвестные скрипты
				else
				{
					bufferScripts << "   " << num1 << "   " << num2 << "   " << num3 << "   " << num4 << '\n';
					std::println("\033[31m[Error]\033[0m Found bad script in {}: {} {} {} {}.", m_stemFileName, num1, num2, num3, num4);
				}
				convertOPN(bufferScripts_OPN1, logicOperator, scriptNumber, bufferScriptsOPN, bufferScripts);
			}

		}

		getScriptSize(outputFileBuffer, bufferScripts, accumulatorNumberScripts);
		outputFile << outputFileBuffer.str();
		bufferScripts.str("");
		outputFileBuffer.str("");
		numberscripts.clear();
		accumulatorNumberScripts++;
	}
	outputFile.close();
	return;
}

void Converter::convertMisWoofers(const std::string_view& mis_woofers) const
{
	std::ofstream outputFileMisWoofers(m_misFolder / "sounds", std::ios::binary);
	if (!outputFileMisWoofers)
	{
		errorWriteFile();
		return;
	}
	
	const uint32_t numOfSounds = readFileUint32(mis_woofers, 0);
	for (uint32_t i = 0, curOffset = sizeof(numOfSounds); i < numOfSounds; ++i)
	{
		const uint8_t* buffername = reinterpret_cast<const uint8_t*>(mis_woofers.data()) + curOffset;
		curOffset += 64;
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_woofers.data()) + curOffset;
		curOffset += 14;
		
		uint16_t U = *(uint16_t*)(buffer + 0);
		uint16_t V = *(uint16_t*)(buffer + 2);
		uint16_t Radius = *(uint16_t*)(buffer + 4);
		float Worse = *(float*)(buffer + 6);
		uint16_t MinWait = *(uint16_t*)(buffer + 10);
		uint16_t MaxWait = *(uint16_t*)(buffer + 12);
		
		outputFileMisWoofers << "Name=\"" << reinterpret_cast<const char*>(buffername);
		outputFileMisWoofers << "\"\nU=" << U << "\nV=" << V
			<< "\nRadius=" << Radius << "\nWorse=" << Worse << "\nMinWait=" << MinWait << "\nMaxWait=" << MaxWait << "\n\n";
	}
	outputFileMisWoofers.close();
}

void Converter::convertMisMines(const std::string_view& map_mines, const uint32_t mapSizeU) const
{
	std::ofstream outputFileMisMines(m_misFolder / "mines", std::ios::binary);
	if (!outputFileMisMines)
	{
		errorWriteFile();
		return;
	}

	std::vector<uint8_t> mapMines(LOCATIONSSIZE, 0);

	const size_t maxTries = map_mines.size() / mapSizeU;
	for (size_t i = 0; i < maxTries; ++i)
		std::copy(map_mines.cbegin() + i * mapSizeU, map_mines.cbegin() + i * mapSizeU + mapSizeU, mapMines.begin() + 512 * i);

	outputFileMisMines.write(reinterpret_cast<const char*>(mapMines.data()), mapMines.size());
	outputFileMisMines.close();
}

void Converter::convertMisTree(const std::string_view& mis_desc) const
{
	std::ofstream outputFileMisTree(m_misFolder / "misdesc", std::ios::binary);
	if (!outputFileMisTree)
	{
		errorWriteFile();
		return;
	}

	outputFileMisTree.write(reinterpret_cast<const char*>(mis_desc.data()), mis_desc.size());
	outputFileMisTree.close();
}

void Converter::convertMisPhrases(const std::string_view& mis_phrases, const uint32_t sizeMisPhrases) const
{
	std::ofstream outputFileMisPharses(m_misFolder / "phrases", std::ios::binary);
	if (!outputFileMisPharses)
	{
		errorWriteFile();
		return;
	}

	std::vector<uint8_t> phrases(32000, 0);
	size_t phrasesOffset{ 0 };
	uint32_t accumulator{ 0 };
	uint32_t sizeline;
	while (accumulator < sizeMisPhrases && phrasesOffset < 32000)
	{
		//uint32_t numtext = readFileInt8(mis_phrases, 0 + accumulator);		// not used

		sizeline = readFileInt8(mis_phrases, 1 + accumulator);
		position(mis_phrases, phrases, 2 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(mis_phrases, 2 + accumulator);
		position(mis_phrases, phrases, 3 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(mis_phrases, 3 + accumulator);
		position(mis_phrases, phrases, 4 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(mis_phrases, 4 + accumulator);
		position(mis_phrases, phrases, 5 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(mis_phrases, 5 + accumulator);
		position(mis_phrases, phrases, 6 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;
		accumulator += 6;
	}

	outputFileMisPharses.write(reinterpret_cast<const char*>(phrases.data()), phrases.size());
	outputFileMisPharses.close();
}

void Converter::convertMisObjects(const std::string_view& mis_objects) const
{
	std::ofstream outputFileMisObjects(m_misFolder / "objs", std::ios::binary);
	if (!outputFileMisObjects)
	{
		errorWriteFile();
		return;
	}

	const size_t maxTries = mis_objects.size() / 2;
	for (size_t i = 0; i < maxTries; ++i)
	{
		uint8_t num1 = mis_objects[i * 2];
		uint8_t num2 = mis_objects[i * 2 + 1];
		outputFileMisObjects << num1 << num2 << GLOBALNULL << GLOBALNULL;
	}
	outputFileMisObjects.close();
}

void Converter::convertMisPlayers(const std::string_view& mis_players) const
{
	std::ofstream outputMisPlayers(m_misFolder / "players", std::ios::binary);
	if (!outputMisPlayers)
	{
		errorWriteFile();
		return;
	}
	
	const size_t maxTries = mis_players.size() / 353;
	for (size_t curTry = 0; curTry < maxTries; ++curTry)
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(mis_players.data()) + curTry * 353;
		outputMisPlayers << "Player " << curTry << '\n';
		std::string_view name{ (const char*)buffer };
		outputMisPlayers << " name=" << '\"' << name << '\"' << '\n';
		uint8_t team = *(uint8_t*)(buffer + 32);
		outputMisPlayers << " team=" << (uint16_t)team << '\n';
		uint16_t RGB_Color = *(uint16_t*)(buffer + 33);
		uint8_t nation = *(uint8_t*)(buffer + 35);
		uint8_t planesdir = *(uint8_t*)(buffer + 36);
		outputMisPlayers << " nation=" << (uint16_t)nation << '\n';
		{
			unsigned short red = (RGB_Color >> 11) & 0x1F;
			unsigned short green = (RGB_Color >> 5) & 0x3F;
			unsigned char blue = RGB_Color & 0x1F;
			outputMisPlayers << " color=" << (uint16_t)red * 8 << " " << (uint16_t)green * 4 << " " << (uint16_t)blue * 8 << '\n';
		}
		
		outputMisPlayers << " bomb" << '\n';
		std::string_view namebomb{ (const char*)buffer + 37 };
		outputMisPlayers << "  ID=" << namebomb << '\n';
		uint32_t Numberbomb = *(uint32_t*)(buffer + 69);
		uint32_t Bombsbomb = *(uint32_t*)(buffer + 73);
		uint32_t Reloadbomb = *(uint32_t*)(buffer + 77);
		outputMisPlayers << "  Number=" << Numberbomb << '\n' << "  Bombs=" << Bombsbomb << '\n' << "  Reload=" << Reloadbomb << '\n';
		
		outputMisPlayers << " spy" << '\n';
		std::string_view namespy{ (const char*)buffer + 81 };
		outputMisPlayers << "  ID=" << namespy << '\n';
		uint32_t Numberspy = *(uint32_t*)(buffer + 113);
		uint32_t Bombsspy = *(uint32_t*)(buffer + 117);
		uint32_t Reloadspy = *(uint32_t*)(buffer + 121);
		outputMisPlayers << "  Number=" << Numberspy << '\n' << "  Bombs=" << Bombsspy << '\n' << "  Reload=" << Reloadspy << '\n';
		
		outputMisPlayers << " transport" << '\n';
		std::string_view nametransport{ (const char*)buffer + 125 };
		outputMisPlayers << "  ID=" << nametransport << '\n';
		uint32_t Numbertransport = *(uint32_t*)(buffer + 157);
		uint32_t Bombstransport = *(uint32_t*)(buffer + 161);
		uint32_t Reloadtransport = *(uint32_t*)(buffer + 165);
		outputMisPlayers << "  Number=" << Numbertransport << '\n' << "  Bombs=" << Bombstransport << '\n' << "  Reload=" << Reloadtransport << '\n';
		
		outputMisPlayers << " boxer" << '\n';
		std::string_view nameboxer{ (const char*)buffer + 169 };
		outputMisPlayers << "  ID=" << nameboxer << '\n';
		uint32_t Numberboxer = *(uint32_t*)(buffer + 201);
		uint32_t Bombsboxer = *(uint32_t*)(buffer + 205);
		uint32_t Reloadboxer = *(uint32_t*)(buffer + 209);
		outputMisPlayers << "  Number=" << Numberboxer << '\n' << "  Bombs=" << Bombsboxer << '\n' << "  Reload=" << Reloadboxer << '\n';
		
		outputMisPlayers << " descent 0" << '\n';
		uint8_t group1 = *(uint8_t*)(buffer + 213);
		uint8_t expa1 = *(uint8_t*)(buffer + 214);
		outputMisPlayers << "  group=" << (uint16_t)group1 << '\n' << "  expa=" << (uint16_t)expa1 << '\n';
		std::string_view ID11{ (const char*)buffer + 215 };
		std::string_view ID12{ (const char*)buffer + 231 };
		std::string_view ID13{ (const char*)buffer + 247 };
		std::string_view ID14{ (const char*)buffer + 263 };
		uint8_t number11 = *(uint8_t*)(buffer + 279);
		uint8_t number12 = *(uint8_t*)(buffer + 280);
		uint8_t number13 = *(uint8_t*)(buffer + 281);
		uint8_t number14 = *(uint8_t*)(buffer + 282);
		outputMisPlayers << "  ID 0=" << ID11 << '\n' << "  number 0=" << (uint16_t)number11 << '\n'
			<< "  ID 1=" << ID12 << '\n' << "  number 1=" << (uint16_t)number12 << '\n'
			<< "  ID 2=" << ID13 << '\n' << "  number 2=" << (uint16_t)number13 << '\n'
			<< "  ID 3=" << ID14 << '\n' << "  number 3=" << (uint16_t)number14 << '\n';
		
		outputMisPlayers << " descent 1" << '\n';
		uint8_t group2 = *(uint8_t*)(buffer + 283);
		uint8_t expa2 = *(uint8_t*)(buffer + 284);
		outputMisPlayers << "  group=" << (uint16_t)group2 << '\n' << "  expa=" << (uint16_t)expa2 << '\n';
		std::string_view ID21{ (const char*)buffer + 285 };
		std::string_view ID22{ (const char*)buffer + 301 };
		std::string_view ID23{ (const char*)buffer + 317 };
		std::string_view ID24{ (const char*)buffer + 333 };
		uint8_t number21 = *(uint8_t*)(buffer + 349);
		uint8_t number22 = *(uint8_t*)(buffer + 350);
		uint8_t number23 = *(uint8_t*)(buffer + 351);
		uint8_t number24 = *(uint8_t*)(buffer + 352);
		outputMisPlayers << "  ID 0=" << ID21 << '\n' << "  number 0=" << (uint16_t)number21 << '\n'
			<< "  ID 1=" << ID22 << '\n' << "  number 1=" << (uint16_t)number22 << '\n'
			<< "  ID 2=" << ID23 << '\n' << "  number 2=" << (uint16_t)number23 << '\n'
			<< "  ID 3=" << ID24 << '\n' << "  number 3=" << (uint16_t)number24 << '\n';
		
		outputMisPlayers << " planesdir=" << (uint16_t)planesdir << '\n';
	}
	outputMisPlayers.close();
}