
#include "stdafx.h"

#include "Helper.h"
#include "Parser.h"
#include "util.h"
#include "Displayinfo.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

void Parser::parseMap(const std::filesystem::path& filepath)
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

    // trying to bypass germans umlauts in map file names
    std::wstring wfilepath = filepath.wstring();
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
        m_mapFolder = fileFolder / ("parser_map." + m_stemFileName);
        m_misFolder = m_mapFolder / "mis.000";
        break;
    }
    case HEADER_CAMP_MAP:
    {
        if (m_stemFileName.length() >= 3)
            m_mapFolder = fileFolder / ("parser_map." + m_stemFileName.substr(0, 3));
        break;
    }
    case HEADER_CAMP_MIS:
    {
        if (m_stemFileName.length() >= 3)
            m_mapFolder = fileFolder / ("parser_map." + m_stemFileName.substr(0, 3));
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
            parseMapFileSSM(rawData);

            break;
        }
        case HEADER_MULTI:
        {
            own::print(Dictionary::getValue(STRINGS::MAP_MULTI), m_stemFileName);
            parseMapFileSMM(rawData);

            break;
        }
        case HEADER_CAMP_MAP:
        {
            own::print(Dictionary::getValue(STRINGS::CAMP_MAP), m_stemFileName);
            parseMapFileSSC_map(rawData);

            break;
        }
        case HEADER_CAMP_MIS:
        {
            own::print(Dictionary::getValue(STRINGS::CAMP_MIS), m_stemFileName);
            parseMapFileSCC_mission(rawData);

            break;
        }
        default:
        {
            std::println("");
            own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
            return;
        }
        }
    }
    catch (const std::out_of_range& e)
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

uint32_t Parser::parseMapFileSMM(const std::string_view& inputFile) const
{
    // �������� ����� ��������� �������� ��� ������
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
    std::ofstream outputMisObjects(m_mapFolder / "mis_objects", std::ios::binary);

    // ���������� ��������� ������� � ���������� ���� ��� ����������
    uint32_t mapIdentifier = readFileUint32(inputFile, 136);
    uint32_t mapSizeU = readFileUint32(inputFile, 140);
    uint32_t mapSizeV = readFileUint32(inputFile, 144);

    uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
    uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

    uint32_t startPositionMisObjects = position(inputFile, outputMapInfo, FILE_TYPE_OFFSET, MapHeaderSMM);
    uint32_t startPositionMisScripts = position(inputFile, outputMisObjects, startPositionMisObjects, MISOBJECTS);
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

    // ���������� ��������� ������� � ���������� ���� ��� ����������
    uint32_t mapIdentifier = readFileUint32(inputFile, 40);
    uint32_t mapSizeU = readFileUint32(inputFile, 44);
    uint32_t mapSizeV = readFileUint32(inputFile, 48);
    uint32_t maskByte = readFileUint32(inputFile, MapHeaderSSM);

    uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
    uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

    uint32_t startPositionMisDesc = position(inputFile, outputMapInfo, FILE_TYPE_OFFSET, MapHeaderSSM);
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
    // �������� ����� ��������� �������� ��� ������
    std::filesystem::create_directories(m_mapFolder);
    std::ofstream outputMapInfo(m_mapFolder / "map_info", std::ios::binary);
    std::ofstream outputMapLandnames(m_mapFolder / "map_landnames", std::ios::binary);
    std::ofstream outputMapRhombs(m_mapFolder / "map_rhombs", std::ios::binary);
    std::ofstream outputMapFlags(m_mapFolder / "map_flags", std::ios::binary);
    std::ofstream outputMiniMap(m_mapFolder / "map_mini1", std::ios::binary);
    std::ofstream outputMiniMap2(m_mapFolder / "map_mini2", std::ios::binary);
    std::ofstream outputMapObjects(m_mapFolder / "map_objects", std::ios::binary);

    // ���������� ��������� ������� � ���������� ���� ��� ����������
    // ��������� ������� ������ ������
    uint32_t mapIdentifier = readFileUint32(inputFile, 8);
    uint32_t mapSizeU = readFileUint32(inputFile, 12);
    uint32_t mapSizeV = readFileUint32(inputFile, 16);

    uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
    uint32_t flagSize = tileArray(mapSizeU, mapSizeV, 4);

    uint32_t startPositionLandname = position(inputFile, outputMapInfo, FILE_TYPE_OFFSET, MapHeaderSSC_map);
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
    // �������� ����� ��������� �������� ��� ������
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