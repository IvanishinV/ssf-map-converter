
#include <gzip/decompress.hpp>
#ifdef _MSC_VER
#pragma comment(lib, "zlibstatic.lib")
#endif

#include "Converter.h"
#include "Displayinfo.h"
#include "Helper.h"
#include "Lang.h"
#include "io/wire_reader.h"
#include "util.h"

#include <algorithm>
#include <fstream>
#include <future>
#include <print>
#include <span>
#include <string>
#include <vector>
#include "convert/map_desc.h"
#include "convert/map_landname.h"
#include "convert/map_cflags.h"
#include "convert/map_info.h"
#include "convert/map_mini.h"
#include "convert/map_objects.h"
#include "convert/map_rhombs.h"
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
	WireReader r_zip{std::as_bytes(std::span{zipData})};
	const std::string rawData = (r_zip.peek_at<uint16_t>(0) == 0x8b1f) ?
		gzip::decompress(zipData.data(), zipData.size()) :
		std::string(zipData.cbegin(), zipData.cend());

	// trying to bypass germans umlauts in map file names (and other non-ascii symbols)
	std::wstring wfilepath = filepath.filename().wstring();
	wfilepath.erase(std::remove_if(wfilepath.begin(), wfilepath.end(), [](wchar_t c) {
		const auto u = static_cast<uint32_t>(c);
		return !((u < 128) || (u >= 0x400 && u < 0x500));
	}), wfilepath.cend());
	const std::filesystem::path filepath_ex{ wfilepath };

	const std::filesystem::path fileFolder = filepath.parent_path();

	MapCtx ctx;
	ctx.stemFileName = filepath_ex.string();
	ctx.mapType = WireReader{std::as_bytes(std::span{rawData})}.peek_at<uint32_t>(0);
	switch (ctx.mapType)
	{
	case HEADER_SINGLE:
	case HEADER_MULTI:
	{
		ctx.mapFolder = fileFolder / ("map." + ctx.stemFileName);
		ctx.misFolder = ctx.mapFolder / "mis.000";
		break;
	}
	case HEADER_CAMP_MAP:
	{
		if (ctx.stemFileName.length() >= 3)
			ctx.mapFolder = fileFolder / ("map." + ctx.stemFileName.substr(0, 3));
		break;
	}
	case HEADER_CAMP_MIS:
	{
		if (ctx.stemFileName.length() >= 3)
		{
			ctx.mapFolder = fileFolder / ("map." + ctx.stemFileName.substr(0, 3));
			ctx.misFolder = ctx.mapFolder / ("mis." + ctx.stemFileName.substr(3, 3));
		}
		break;
	}
	}

	try
	{
		switch (ctx.mapType)
		{
		case HEADER_SINGLE:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_SINGLE), ctx.stemFileName);
			convertMapFileSSM(rawData, ctx);

			break;
		}
		case HEADER_MULTI:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_MULTI), ctx.stemFileName);
			convertMapFileSMM(rawData, ctx);

			break;
		}
		case HEADER_CAMP_MAP:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MAP), ctx.stemFileName);
			convertMapFileSSC_map(rawData, ctx);

			break;
		}
		case HEADER_CAMP_MIS:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MIS), ctx.stemFileName);
			convertMapFileSCC_mission(rawData, ctx);

			break;
		}
		default:
		{
			std::println("");
			own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
			return;
		}
		}

		own::printlnSuccess(Dictionary::getValue(STRINGS::SUCCESS_CONVERTED), filepath_ex.string(), ctx.mapFolder.string());
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

uint32_t Converter::convertMapFileSMM(const std::string_view& inputFile, const MapCtx& ctx)
{
	WireReader r{std::as_bytes(std::span{inputFile})};
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

	const uint32_t mapIdentifier = r.peek_at<uint32_t>(136);
	const uint32_t mapSizeU = r.peek_at<uint32_t>(140);
	const uint32_t mapSizeV = r.peek_at<uint32_t>(144);

	const uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	const uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

	const uint32_t offsetMisObjects  = position(inputFile, map_info,      FILE_TYPE_OFFSET,          MapHeaderSMM);
	const uint32_t offsetMisScripts  = position(inputFile, mis_objects,   offsetMisObjects,           MISOBJECTS);
	const uint32_t sizeMisScripts    = r.peek_at<uint32_t>(offsetMisScripts);
	const uint32_t accumulator       = misScripts(inputFile, sizeMisScripts, offsetMisScripts);
	const uint32_t offsetMisDesc     = position(inputFile, mis_scripts,   offsetMisScripts,           accumulator + 4);
	const uint32_t BriefingSize      = r.peek_at<uint32_t>(offsetMisDesc);
	const uint32_t offsetMapMini     = position(inputFile, mis_desc,      offsetMisDesc + 4,          BriefingSize);
	const uint32_t MapMiniSize       = minimapsize(mapSizeU, mapSizeV);
	const uint32_t offsetRhombs      = position(inputFile, map_mini,      offsetMapMini,              MapMiniSize);
	const uint32_t offsetFlags       = position(inputFile, map_rhombs,    offsetRhombs,               rhombsSize);
	const uint32_t offsetLandnames   = position(inputFile, map_flags,     offsetFlags,                flagSize);
	const uint32_t offsetMapObjects  = position(inputFile, map_landname,  offsetLandnames,            LANDNAMESSIZE);
	const uint32_t mapSizeMapObjects = r.peek_at<uint32_t>(offsetMapObjects);
	const uint32_t offsetMisUnitNames = position(inputFile, map_objects,  offsetMapObjects + 4,       mapSizeMapObjects * 8);
	const uint32_t sizeMisUnitNames  = r.peek_at<uint32_t>(offsetMisUnitNames);
	const uint32_t offsetMisWoofers  = position(inputFile, mis_unitnames, offsetMisUnitNames + 4,     sizeMisUnitNames * 16);
	const uint32_t sizeMisWoofers    = r.peek_at<uint32_t>(offsetMisWoofers);
	const uint32_t offsetMisZones    = position(inputFile, mis_woofers,   offsetMisWoofers,           sizeMisWoofers * MISMUSICSIZE + 4);
	const uint32_t offsetMapMines    = position(inputFile, mis_zones,     offsetMisZones,             mapSizeU * mapSizeV);
	const uint32_t offsetMisSupport  = position(inputFile, map_mines,     offsetMapMines,             mapSizeU * mapSizeV);
	const uint32_t sizeMisSupport    = r.peek_at<uint32_t>(offsetMisSupport);
	const uint32_t offsetMisPlayers  = position(inputFile, mis_support,   offsetMisSupport,           sizeMisSupport + 4);
	const uint32_t sizeMisPlayers    = r.peek_at<uint32_t>(offsetMisPlayers);
	const uint32_t offsetMisPhrases  = position(inputFile, mis_players,   offsetMisPlayers + 4,       MISPLAYERSSIZE * sizeMisPlayers);
	const uint32_t sizeMisPhrases    = r.peek_at<uint32_t>(offsetMisPhrases);
	const uint32_t offsetMisMapUnits = position(inputFile, mis_phrases,   offsetMisPhrases + 4,       sizeMisPhrases);
	const uint32_t sizeMisMapUnits   = r.peek_at<uint32_t>(offsetMisMapUnits);
	const uint32_t mapEndPosition    = position(inputFile, mis_mapunits,  offsetMisMapUnits + 4,      sizeMisMapUnits);

	std::filesystem::create_directories(ctx.misFolder);

	//MAP
	const auto resMapRhombs = std::async(std::launch::async, convert::map_rhombs, ctx.mapFolder, map_rhombs, mapIdentifier);
	convert::map_desc(ctx.mapFolder, ctx.mapType, ctx.stemFileName);
	convert::map_info(ctx.mapFolder, mapIdentifier, mapSizeU, mapSizeV);
	convert::map_mini(ctx.mapFolder, ctx.mapType, mapSizeU, mapSizeV, map_mini);
	convert::map_landname(ctx.mapFolder, map_landname);
	convert::map_cflags(ctx.mapFolder, map_flags);
	convert::map_objects(ctx.mapFolder, map_objects);
	//MIS
	convert::mis_objects(ctx.misFolder, mis_objects);
	convert::mis_desc(ctx.misFolder);
	convert::mis_mult(ctx.misFolder, map_info, 1);
	convert::mis_info(ctx.misFolder, mapSizeU, mapSizeV);
	convert::mis_scripts(ctx.misFolder, ctx.stemFileName, mis_scripts);
	convert::mis_tree(ctx.misFolder, mis_desc);
	convert::mis_woofers(ctx.misFolder, mis_woofers);
	convert::mis_mines(ctx.misFolder, map_mines, mapSizeU);
	convert::mis_zones(ctx.misFolder, mis_zones, mapSizeU);
	convert::mis_players(ctx.misFolder, mis_players);
	convert::mis_phrases(ctx.misFolder, mis_phrases, sizeMisPhrases);
	convert::mis_units(ctx.misFolder, mis_unitnames, mis_mapunits, mis_support);

	{
		std::ofstream outputFileMisGroups(ctx.misFolder / "groups", std::ios::binary);
		if (!outputFileMisGroups)
		{
			errorWriteFile("groups");
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
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSSM(const std::string_view& inputFile, const MapCtx& ctx)
{
	WireReader r{std::as_bytes(std::span{inputFile})};
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

	const uint32_t mapIdentifier = r.peek_at<uint32_t>(40);
	const uint32_t mapSizeU = r.peek_at<uint32_t>(44);
	const uint32_t mapSizeV = r.peek_at<uint32_t>(48);
	uint32_t maskByte = r.peek_at<uint32_t>(MapHeaderSSM);

	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

	uint32_t startPositionMisDesc      = position(inputFile, map_info,      FILE_TYPE_OFFSET,              MapHeaderSSM);
	uint32_t startPositionMapMini      = position(inputFile, mis_desc,      startPositionMisDesc + 4,      maskByte);
	uint32_t MapMiniSize               = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs       = position(inputFile, map_mini,      startPositionMapMini,          MapMiniSize);
	uint32_t startPositionFlags        = position(inputFile, map_rhombs,    startPositionRhombs,           rhombsSize);
	uint32_t startPositionLandname     = position(inputFile, map_flags,     startPositionFlags,            flagSize);
	uint32_t startPositionObjects      = position(inputFile, map_landname,  startPositionLandname,         LANDNAMESSIZE);
	uint32_t mapSizeObjects            = r.peek_at<uint32_t>(startPositionObjects);
	uint32_t startPositionMisUnitNames = position(inputFile, map_objects,   startPositionObjects + 4,      mapSizeObjects * 8);
	uint32_t sizeMisUnitNames          = r.peek_at<uint32_t>(startPositionMisUnitNames);
	uint32_t startPositionMisGroups    = position(inputFile, mis_unitnames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits  = position(inputFile, mis_groups,    startPositionMisGroups,        MISGROUPSSIZE);
	uint32_t sizeMisMapUnits           = r.peek_at<uint32_t>(startPositionMisMapUnits);
	uint32_t startPositionMisPlayers   = position(inputFile, mis_mapunits,  startPositionMisMapUnits + 4,  sizeMisMapUnits);
	uint32_t sizeMisPlayers            = r.peek_at<uint32_t>(startPositionMisPlayers);
	uint32_t startPositionMisWoofers   = position(inputFile, mis_players,   startPositionMisPlayers + 4,   MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers            = r.peek_at<uint32_t>(startPositionMisWoofers);
	uint32_t startPositionMisZones     = position(inputFile, mis_woofers,   startPositionMisWoofers,       sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts   = position(inputFile, mis_zones,     startPositionMisZones,         mapSizeU * mapSizeV);
	uint32_t sizeMisScripts            = r.peek_at<uint32_t>(startPositionMisScripts);
	uint32_t accumulator               = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines     = position(inputFile, mis_scripts,   startPositionMisScripts,       accumulator + 4);
	uint32_t startPositionMisSupport   = position(inputFile, map_mines,     startPositionMapMines,         mapSizeU * mapSizeV);
	uint32_t sizeMisSupport            = r.peek_at<uint32_t>(startPositionMisSupport);
	uint32_t startPositionMisPhrases   = position(inputFile, mis_support,   startPositionMisSupport,       sizeMisSupport + 4);
	uint32_t sizeMisPhrases            = r.peek_at<uint32_t>(startPositionMisPhrases);
	uint32_t startPositionMisObjects   = position(inputFile, mis_phrases,   startPositionMisPhrases + 4,   sizeMisPhrases);
	uint32_t mapEndPosition            = position(inputFile, mis_objects,   startPositionMisObjects,       MISOBJECTS);

	std::filesystem::create_directories(ctx.misFolder);

	//MAP
	const auto resMapRhombs = std::async(std::launch::async, convert::map_rhombs, ctx.mapFolder, map_rhombs, mapIdentifier);
	convert::map_desc(ctx.mapFolder, ctx.mapType, ctx.stemFileName);
	convert::map_info(ctx.mapFolder, mapIdentifier, mapSizeU, mapSizeV);
	convert::map_mini(ctx.mapFolder, ctx.mapType, mapSizeU, mapSizeV, map_mini);
	convert::map_objects(ctx.mapFolder, map_objects);
	convert::map_landname(ctx.mapFolder, map_landname);
	convert::map_cflags(ctx.mapFolder, map_flags);
	//MIS
	convert::mis_desc(ctx.misFolder);
	convert::mis_mult(ctx.misFolder, map_info, 0);
	convert::mis_info(ctx.misFolder, mapSizeU, mapSizeV);
	convert::mis_tree(ctx.misFolder, mis_desc);
	convert::mis_zones(ctx.misFolder, mis_zones, mapSizeU);
	convert::mis_mines(ctx.misFolder, map_mines, mapSizeU);
	convert::mis_units(ctx.misFolder, mis_unitnames, mis_mapunits, mis_support);
	convert::mis_scripts(ctx.misFolder, ctx.stemFileName, mis_scripts);
	convert::mis_woofers(ctx.misFolder, mis_woofers);
	convert::mis_phrases(ctx.misFolder, mis_phrases, sizeMisPhrases);
	convert::mis_objects(ctx.misFolder, mis_objects);
	convert::mis_groups(ctx.misFolder, mis_groups);
	convert::mis_players(ctx.misFolder, mis_players);

	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSSC_map(const std::string_view& inputFile, const MapCtx& ctx)
{
	WireReader r{std::as_bytes(std::span{inputFile})};
	std::string_view map_info;
	std::string_view map_landname;
	std::string_view map_mini;
	std::string_view map_mini2;
	std::string_view map_rhombs;
	std::string_view map_flags;
	std::string_view map_objects;

	const uint32_t mapIdentifier = r.peek_at<uint32_t>(8);
	const uint32_t mapSizeU = r.peek_at<uint32_t>(12);
	const uint32_t mapSizeV = r.peek_at<uint32_t>(16);

	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

	uint32_t startPositionLandname = position(inputFile, map_info,     FILE_TYPE_OFFSET,      MapHeaderSSC_map);
	uint32_t startPositionRhombs   = position(inputFile, map_landname, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags    = position(inputFile, map_rhombs,   startPositionRhombs,   rhombsSize);
	uint32_t startPositionMapMini  = position(inputFile, map_flags,    startPositionFlags,    flagSize);
	uint32_t startPositionMapMini2 = position(inputFile, map_mini,     startPositionMapMini,  rhombsSize);
	uint32_t startPositionObjects  = position(inputFile, map_mini2,    startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects        = r.peek_at<uint32_t>(startPositionObjects);
	uint32_t mapEndPosition        = position(inputFile, map_objects,  startPositionObjects + 4, mapSizeObjects * 8);

	std::filesystem::create_directories(ctx.mapFolder);

	const auto resMapRhombs = std::async(std::launch::async, convert::map_rhombs, ctx.mapFolder, map_rhombs, mapIdentifier);
	convert::map_info(ctx.mapFolder, mapIdentifier, mapSizeU, mapSizeV);
	convert::map_mini(ctx.mapFolder, ctx.mapType, mapSizeU, mapSizeV, map_mini);
	convert::map_landname(ctx.mapFolder, map_landname);
	convert::map_cflags(ctx.mapFolder, map_flags);
	convert::map_objects(ctx.mapFolder, map_objects);
	convert::map_desc(ctx.mapFolder, ctx.mapType, ctx.stemFileName);

	resMapRhombs.wait();
	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}

uint32_t Converter::convertMapFileSCC_mission(const std::string_view& inputFile, const MapCtx& ctx)
{
	WireReader r{std::as_bytes(std::span{inputFile})};
	std::string_view mis_info;
	std::string_view map_mines;
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

	const uint32_t mapIdentifier = 40;
	const uint32_t mapSizeU = r.peek_at<uint32_t>(16);
	const uint32_t mapSizeV = r.peek_at<uint32_t>(20);

	uint32_t startPositionMisDesc      = position(inputFile, mis_info,      FILE_TYPE_OFFSET,              MapHeaderSSC_mission);
	uint32_t sizeMisDesc               = r.peek_at<uint32_t>(MapHeaderSSC_mission);
	uint32_t startPositionMisUnitNames = position(inputFile, mis_desc,      startPositionMisDesc + 4,      sizeMisDesc);
	uint32_t sizeMisUnitNames          = r.peek_at<uint32_t>(startPositionMisUnitNames);
	uint32_t startPositionMisGroups    = position(inputFile, mis_unitnames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits  = position(inputFile, mis_groups,    startPositionMisGroups,        MISGROUPSSIZE);
	uint32_t sizeMisMapUnits           = r.peek_at<uint32_t>(startPositionMisMapUnits);
	uint32_t startPositionMisPlayers   = position(inputFile, mis_mapunits,  startPositionMisMapUnits + 4,  sizeMisMapUnits);
	uint32_t sizeMisPlayers            = r.peek_at<uint32_t>(startPositionMisPlayers);
	uint32_t startPositionMisWoofers   = position(inputFile, mis_players,   startPositionMisPlayers + 4,   MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers            = r.peek_at<uint32_t>(startPositionMisWoofers);
	uint32_t startPositionMisZones     = position(inputFile, mis_woofers,   startPositionMisWoofers,       sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts   = position(inputFile, mis_zones,     startPositionMisZones,         mapSizeU * mapSizeV);
	uint32_t sizeMisScripts            = r.peek_at<uint32_t>(startPositionMisScripts);
	uint32_t accumulator               = misScripts(inputFile, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines     = position(inputFile, mis_scripts,   startPositionMisScripts,       accumulator + 4);
	uint32_t startPositionMisSupport   = position(inputFile, map_mines,     startPositionMapMines,         mapSizeU * mapSizeV);
	uint32_t sizeMisSupport            = r.peek_at<uint32_t>(startPositionMisSupport);
	uint32_t startPositionMisPhrases   = position(inputFile, mis_support,   startPositionMisSupport,       sizeMisSupport + 4);
	uint32_t sizeMisPhrases            = r.peek_at<uint32_t>(startPositionMisPhrases);
	uint32_t startPositionMisObjects   = position(inputFile, mis_phrases,   startPositionMisPhrases + 4,   sizeMisPhrases);
	uint32_t mapEndPosition            = position(inputFile, mis_objects,   startPositionMisObjects,       MISOBJECTS);

	std::filesystem::create_directories(ctx.misFolder);
	convert::mis_desc(ctx.misFolder);
	convert::mis_mult(ctx.misFolder, mis_info, 2);
	convert::mis_info(ctx.misFolder, mapSizeU, mapSizeV);
	convert::mis_zones(ctx.misFolder, mis_zones, mapSizeU);
	convert::mis_units(ctx.misFolder, mis_unitnames, mis_mapunits, mis_support);
	convert::mis_groups(ctx.misFolder, mis_groups);
	convert::mis_scripts(ctx.misFolder, ctx.stemFileName, mis_scripts);
	convert::mis_woofers(ctx.misFolder, mis_woofers);
	convert::mis_mines(ctx.misFolder, map_mines, mapSizeU);
	convert::mis_tree(ctx.misFolder, mis_desc);
	convert::mis_phrases(ctx.misFolder, mis_phrases, sizeMisPhrases);
	convert::mis_objects(ctx.misFolder, mis_objects);
	convert::mis_players(ctx.misFolder, mis_players);

	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
	return 0;
}
