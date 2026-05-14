
#include "stdafx.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

#include "Helper.h"
#include "Converter.h"
#include "Displayinfo.h"
#include "RhombsParser.h"
#include "util.h"
#include "wire/types.h"
#include "wire/bmp.h"
#include "convert/map_desc.h"
#include "convert/map_landname.h"
#include "convert/map_cflags.h"
#include "convert/map_info.h"
#include "convert/map_mini.h"
#include "convert/map_objects.h"
#include "convert/mis_desc.h"
#include "convert/mis_mult.h"
#include "convert/mis_info.h"
#include "convert/mis_zones.h"
#include "convert/mis_units.h"
#include "convert/mis_groups.h"
#include "convert/mis_scripts.h"
#include "convert/mis_woofers.h"
#include "convert/mis_mines.h"
#include "convert/mis_tree.h"
#include "convert/mis_phrases.h"
#include "convert/mis_objects.h"
#include "convert/mis_players.h"


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

	std::vector<char> zipData(static_cast<size_t>(inputFile.tellg()));

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

	// trying to bypass germans umlauts in map file names (and other non-ascii symbols)
	std::wstring wfilepath = filepath.filename().wstring();
	wfilepath.erase(std::remove_if(wfilepath.begin(), wfilepath.end(), [](wchar_t c) { return !((c >= 0 && c < 128) || (c >= 0x400 && c < 0x500)); }), wfilepath.cend());
	const std::filesystem::path filepath_ex{ wfilepath };
	m_stemFileName = filepath_ex.string();

	const std::filesystem::path fileFolder = filepath.parent_path();

	m_mapType = *(uint32_t*)rawData.data();
	switch (m_mapType)
	{
	case HEADER_SINGLE:
	case HEADER_MULTI:
	{
		m_mapFolder = fileFolder / ("map." + m_stemFileName);
		m_misFolder = m_mapFolder / "mis.000";
		break;
	}
	case HEADER_CAMP_MAP:
	{
		if (m_stemFileName.length() >= 3)
			m_mapFolder = fileFolder / ("map." + m_stemFileName.substr(0, 3));
		break;
	}
	case HEADER_CAMP_MIS:
	{
		if (m_stemFileName.length() >= 3)
			m_mapFolder = fileFolder / ("map." + m_stemFileName.substr(0, 3));
		if (m_stemFileName.length() >= 3)
			m_misFolder = m_mapFolder / ("mis." + m_stemFileName.substr(3, 3));

		break;
	}
	}

	try
	{
		switch (m_mapType)
		{
		case HEADER_SINGLE:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_SINGLE), m_stemFileName);
			convertMapFileSSM(rawData);

			break;
		}
		case HEADER_MULTI:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_MULTI), m_stemFileName);
			convertMapFileSMM(rawData);

			break;
		}
		case HEADER_CAMP_MAP:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MAP), m_stemFileName);
			convertMapFileSSC_map(rawData);

			break;
		}
		case HEADER_CAMP_MIS:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MIS), m_stemFileName);
			convertMapFileSCC_mission(rawData);

			break;
		}
		default:
		{
			std::println("");
			own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
			return;
		}
		}

		own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filepath_ex.string(), m_mapFolder.string());
	}
	catch (const std::out_of_range&)
	{
		own::printlnError(Dictionary::getValue(STRINGS::OUT_OF_RANGE_ERROR));
	}
	catch (const std::exception& e)
	{
		own::printlnError(Dictionary::getValue(STRINGS::EXCEPTION_ERROR), e.what());
	}
	catch (...)
	{
		own::printlnError(Dictionary::getValue(STRINGS::UNKNOWN_EXCEPTION_ERROR));
	}
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
	std::string_view mis_objects;
	std::string_view mis_desc;
	std::string_view mis_unitnames;
	std::string_view mis_woofers;
	std::string_view mis_scripts;
	std::string_view mis_zones;
	std::string_view mis_support;
	std::string_view mis_players;
	std::string_view mis_phrases;
	std::string_view mis_mapunits;

	// ���������� ��������� ������� � ���������� ���� ��� ����������
	m_mapIdentifier = readFileUint32(inputFile, 136);
	m_mapSizeU = readFileUint32(inputFile, 140);
	m_mapSizeV = readFileUint32(inputFile, 144);

	const uint32_t rhombsSize = tileArray(m_mapSizeU, m_mapSizeV, 2);
	const uint32_t flagSize = tileArray(m_mapSizeU, m_mapSizeV, 4);

	const uint32_t offsetMisObjects = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSMM);
	const uint32_t offsetMisScripts = position(inputFile, mis_objects, offsetMisObjects, MISOBJECTS);
	const uint32_t sizeMisScripts = readFileUint32(inputFile, offsetMisScripts);
	const uint32_t accumulator = misScripts(inputFile, sizeMisScripts, offsetMisScripts);
	const uint32_t offsetMisDesc = position(inputFile, mis_scripts, offsetMisScripts, accumulator + 4);
	const uint32_t BriefingSize = readFileUint32(inputFile, offsetMisDesc);
	const uint32_t offsetMapMini = position(inputFile, mis_desc, offsetMisDesc + 4, BriefingSize);
	const uint32_t MapMiniSize = minimapsize(m_mapSizeU, m_mapSizeV);
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
	const uint32_t offsetMapMines = position(inputFile, mis_zones, offsetMisZones, m_mapSizeU * m_mapSizeV);
	const uint32_t offsetMisSupport = position(inputFile, map_mines, offsetMapMines, m_mapSizeU * m_mapSizeV);
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
	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs);
	convertMapDesc();
	convertMapInfo();
	convertMapMini(map_mini);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	convertMapObjects(map_objects);
	//MIS
	convertMisObjects(mis_objects);
	convertMisDesc();
	convertMisMult(map_info, 1);
	convertMisInfo();
	convertMisScripts(mis_scripts);
	convertMisTree(mis_desc);
	convertMisWoofers(mis_woofers);
	convertMisMines(map_mines);
	convertMisZones(mis_zones);
	convertMisPlayers(mis_players);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);

	{
		std::ofstream outputFileMisGroups(m_misFolder / "groups", std::ios::binary);
		if (!outputFileMisGroups)
		{
			errorWriteFile();
		}
		else
		{
			for (size_t i = 0; i < 100; ++i)
			{
				outputFileMisGroups << std::format("Group {}\n", i)
					<< " ai type=\"ai_none\" group1=0 group2=0 zone1=0 zone2=0\n"
					<< "  aiflags=\n"
					<< " reserv auto=0 delay=0 min=0 force=0 atime=0 flag=0 zone=0 hp=100 ammo=100 \n"
					<< "expa=0\n";
			}
		}
	}

	resMapRhombs.wait();
	displayinfo(m_mapSizeU, m_mapSizeV, m_mapIdentifier, mapEndPosition);
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

	// ���������� ��������� ������� � ���������� ���� ��� ����������
	m_mapIdentifier = readFileUint32(inputFile, 40);
	m_mapSizeU = readFileUint32(inputFile, 44);
	m_mapSizeV = readFileUint32(inputFile, 48);
	uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);

	uint32_t rhombsSize = tileArray(m_mapSizeU, m_mapSizeV, 2);
	uint32_t flagSize = tileArray(m_mapSizeU, m_mapSizeV, 4);

	uint32_t startPositionMisDesc = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSSM);
	uint32_t startPositionMapMini = position(inputFile, mis_desc, startPositionMisDesc + 4, maskByte);
	uint32_t MapMiniSize = minimapsize(m_mapSizeU, m_mapSizeV);
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
	uint32_t startPositionMisScripts = position(inputFile, mis_zones, startPositionMisZones, m_mapSizeU * m_mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, mis_scripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, map_mines, startPositionMapMines, m_mapSizeU * m_mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, mis_support, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, mis_phrases, startPositionMisPhrases + 4, sizeMisPhrases);
	uint32_t mapEndPosition = position(inputFile, mis_objects, startPositionMisObjects, MISOBJECTS);

	std::filesystem::create_directories(m_misFolder);

	//MAP
	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs);
	convertMapDesc();
	convertMapInfo();
	convertMapMini(map_mini);
	convertMapObjects(map_objects);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	//MIS
	convertMisDesc();
	convertMisMult(map_info, 0);
	convertMisInfo();
	convertMisTree(mis_desc);
	convertMisZones(mis_zones);
	convertMisMines(map_mines);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	convertMisScripts(mis_scripts);
	convertMisWoofers(mis_woofers);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisObjects(mis_objects);
	convertMisGroups(mis_groups);
	convertMisPlayers(mis_players);

	resMapRhombs.wait();
	displayinfo(m_mapSizeU, m_mapSizeV, m_mapIdentifier, mapEndPosition);
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

	// ���������� ��������� ������� � ���������� ���� ��� ����������
	// ��������� ������� ������ ������
	m_mapIdentifier = readFileUint32(inputFile, 8);
	m_mapSizeU = readFileUint32(inputFile, 12);
	m_mapSizeV = readFileUint32(inputFile, 16);

	uint32_t rhombsSize = tileArray(m_mapSizeU, m_mapSizeV, 2);
	uint32_t flagSize = tileArray(m_mapSizeU, m_mapSizeV, 4);

	uint32_t startPositionLandname = position(inputFile, map_info, FILE_TYPE_OFFSET, MapHeaderSSC_map);
	uint32_t startPositionRhombs = position(inputFile, map_landname, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags = position(inputFile, map_rhombs, startPositionRhombs, rhombsSize);
	uint32_t startPositionMapMini = position(inputFile, map_flags, startPositionFlags, flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, map_mini, startPositionMapMini, rhombsSize);
	uint32_t startPositionObjects = position(inputFile, map_mini2, startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects = readFileUint32(inputFile, startPositionObjects);
	uint32_t mapEndPosition = position(inputFile, map_objects, startPositionObjects + 4, mapSizeObjects * 8);


	std::filesystem::create_directories(m_mapFolder);

	const auto resMapRhombs = std::async(std::launch::async, &Converter::convertMapRhombs, this, map_rhombs);
	convertMapInfo();
	convertMapMini(map_mini);
	convertMapLandname(map_landname);
	convertMapCflags(map_flags);
	convertMapObjects(map_objects);
	convertMapDesc();

	resMapRhombs.wait();
	displayinfo(m_mapSizeU, m_mapSizeV, m_mapIdentifier, mapEndPosition);
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

	m_mapIdentifier = 40;
	m_mapSizeU = readFileUint32(inputFile, 16);
	m_mapSizeV = readFileUint32(inputFile, 20);

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
	uint32_t startPositionMisScripts = position(inputFile, mis_zones, startPositionMisZones, m_mapSizeU * m_mapSizeV);
	uint32_t sizeMisScripts = readFileUint32(inputFile, startPositionMisScripts);
	uint32_t accumulator = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines = position(inputFile, mis_scripts, startPositionMisScripts, accumulator + 4);
	uint32_t startPositionMisSupport = position(inputFile, map_mines, startPositionMapMines, m_mapSizeU * m_mapSizeV);
	uint32_t sizeMisSupport = readFileUint32(inputFile, startPositionMisSupport);
	uint32_t startPositionMisPhrases = position(inputFile, mis_support, startPositionMisSupport, sizeMisSupport + 4);
	uint32_t sizeMisPhrases = readFileUint32(inputFile, startPositionMisPhrases);
	uint32_t startPositionMisObjects = position(inputFile, mis_phrases, startPositionMisPhrases + 4, sizeMisPhrases);
	uint32_t mapEndPosition = position(inputFile, mis_objects, startPositionMisObjects, MISOBJECTS);

	std::filesystem::create_directories(m_misFolder);
	convertMisDesc();
	convertMisMult(mis_info, 2);
	convertMisInfo();
	convertMisZones(mis_zones);
	convertMisUnits(mis_unitnames, mis_mapunits, mis_support);
	convertMisGroups(mis_groups);
	convertMisScripts(mis_scripts);
	convertMisWoofers(mis_woofers);
	convertMisMines(map_mines);
	convertMisTree(mis_desc);
	convertMisPhrases(mis_phrases, sizeMisPhrases);
	convertMisObjects(mis_objects);
	convertMisPlayers(mis_players);

	displayinfo(m_mapSizeU, m_mapSizeV, m_mapIdentifier, mapEndPosition);
	return 0;
}

void Converter::convertMapInfo() const
{
	convert::map_info(m_mapFolder, m_mapIdentifier, m_mapSizeU, m_mapSizeV);
}

void Converter::convertMapMini(const std::string_view& map_mini) const
{
	convert::map_mini(m_mapFolder, m_mapType, m_mapSizeU, m_mapSizeV, map_mini);
}

void Converter::convertMapLandname(const std::string_view& map_landname) const
{
	convert::map_landname(m_mapFolder, map_landname);
}

void Converter::convertMapCflags(const std::string_view& map_flags) const
{
	convert::map_cflags(m_mapFolder, map_flags);
}

void Converter::convertMapObjects(const std::string_view& map_objects) const
{
	convert::map_objects(m_mapFolder, map_objects);
}

void Converter::convertMapDesc() const
{
	convert::map_desc(m_mapFolder, m_mapType, m_stemFileName);
}

void Converter::convertMapRhombs(const std::string_view& map_rhombs) const
{
	std::ofstream outputFile(m_mapFolder / "rhombs", std::ios::binary);
	if (!outputFile)
	{
		errorWriteFile();
		return;
	}

	RhombsParser::parse_rhombs(map_rhombs, outputFile, static_cast<RhombsParser::SchemeType>(m_mapIdentifier));

	outputFile.close();
}

void Converter::convertMisDesc() const
{
	convert::mis_desc(m_misFolder);
}

void Converter::convertMisMult(const std::string_view& map_info, const uint32_t maptypeheader) const
{
	convert::mis_mult(m_misFolder, map_info, maptypeheader);
}

void Converter::convertMisInfo() const
{
	convert::mis_info(m_misFolder, m_mapSizeU, m_mapSizeV);
}

void Converter::convertMisZones(const std::string_view& mis_zones) const
{
	convert::mis_zones(m_misFolder, mis_zones, m_mapSizeU);
}

void Converter::convertMisUnits(const std::string_view& mis_unitnames, const std::string_view& mis_mapunits, const std::string_view& mis_support) const
{
	convert::mis_units(m_misFolder, mis_unitnames, mis_mapunits, mis_support);
}

void Converter::convertMisGroups(const std::string_view& mis_groups) const
{
	convert::mis_groups(m_misFolder, mis_groups);
}

void Converter::convertMisScripts(const std::string_view& mis_scripts) const
{
	convert::mis_scripts(m_misFolder, m_stemFileName, mis_scripts);
}

void Converter::convertMisWoofers(const std::string_view& mis_woofers) const
{
	convert::mis_woofers(m_misFolder, mis_woofers);
}

void Converter::convertMisMines(const std::string_view& map_mines) const
{
	convert::mis_mines(m_misFolder, map_mines, m_mapSizeU);
}

void Converter::convertMisTree(const std::string_view& mis_desc) const
{
	convert::mis_tree(m_misFolder, mis_desc);
}

void Converter::convertMisPhrases(const std::string_view& mis_phrases, const uint32_t sizeMisPhrases) const
{
	convert::mis_phrases(m_misFolder, mis_phrases, sizeMisPhrases);
}

void Converter::convertMisObjects(const std::string_view& mis_objects) const
{
	convert::mis_objects(m_misFolder, mis_objects);
}

void Converter::convertMisPlayers(const std::string_view& mis_players) const
{
	convert::mis_players(m_misFolder, mis_players);
}
