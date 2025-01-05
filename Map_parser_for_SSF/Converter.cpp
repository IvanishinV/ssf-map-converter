
#include "stdafx.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

#include "Helper.h"
#include "Converter.h"
#include "Displayinfo.h"
#include "RhombsParser.h"
#include "util.h"

#pragma region stack_operations

std::stack< std::vector<uint8_t> > one_stack_end_operations(std::vector<uint8_t>& bufferScripts, std::vector<uint8_t>& operand1, std::stack< std::vector<uint8_t> >& stack_for_RPN)
{
	//Если в конце получаем стак с 1 операндом
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	bufferScripts.insert(bufferScripts.end(), operand1.begin(), operand1.end());
	std::format_to(std::back_inserter(bufferScripts), "$END\n!\n");
	operand1.clear();
	return stack_for_RPN;
}

std::stack< std::vector<uint8_t> > two_stack_end_operations(std::vector<uint8_t>& bufferScripts, std::vector<uint8_t>& operand1, std::vector<uint8_t>& operand2, std::stack< std::vector<uint8_t> >& stack_for_RPN)
{
	//Если в конце получаем стак с 2мя операндами
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	bufferScripts.insert(bufferScripts.end(), operand1.begin(), operand1.end());
	bufferScripts.insert(bufferScripts.end(), operand2.begin(), operand2.end());
	std::format_to(std::back_inserter(bufferScripts), "$END\n!\n");
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

std::stack< std::vector<uint8_t> > stack_operations(std::vector<uint8_t>& operand1, std::vector<uint8_t>& operand2, std::stack< std::vector<uint8_t> >& stack_for_RPN, const std::string& script_name)
{
	//если в памяти стака 2 операнда
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1.insert(operand1.end(), script_name.begin(), script_name.end());
	operand1.insert(operand1.end(), operand2.begin(), operand2.end());
	stack_for_RPN.push(operand1);
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

std::stack< std::vector<uint8_t> > stack_operations_NF(std::vector<uint8_t>& operand2, std::stack< std::vector<uint8_t> >& stack_for_RPN, const std::string& script_name)
{
	//если в памяти стака 2 операнда но есть логическая операция "Наверно что" производится запись в начало операнда2 логики и назад в стак, 
	//после идут 2 стандартных пути если есть логическая операция И ИЛИ то 2 стак превращаются в один с записью между ними логики И ИЛИ либо
	//если логики более нет то дело идет к завершению где происходит проверка количества стаков в памяти и в зависимости от этого происходит свой "$END\n!\n"
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand2.insert(operand2.begin(), script_name.begin(), script_name.end());
	stack_for_RPN.push(operand2);
	operand2.clear();
	return stack_for_RPN;
}

std::stack< std::vector<uint8_t> > stack_operations_bracket(std::vector<uint8_t>& operand1, std::vector<uint8_t>& operand2, std::stack< std::vector<uint8_t> >& stack_for_RPN,
	const std::string& script_name, const std::string& bracket_open, const std::string& bracket_close)
{
	//если в памяти стака более двух операндов то происходит операция со скобками
	operand2 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1 = stack_for_RPN.top(); stack_for_RPN.pop();
	operand1.insert(operand1.begin(), bracket_open.begin(), bracket_open.end());
	operand1.insert(operand1.end(), script_name.begin(), script_name.end());
	operand1.insert(operand1.end(), operand2.begin(), operand2.end());
	operand1.insert(operand1.end(), bracket_close.begin(), bracket_close.end());
	stack_for_RPN.push(operand1);
	operand1.clear();
	operand2.clear();
	return stack_for_RPN;
}

#pragma endregion stack_operations

void convertOPN(std::vector<uint8_t>& bufferScripts, std::vector<uint8_t>& operand1, std::vector<uint8_t>& operand2,
	std::stack< std::vector<uint8_t> >& stack_for_RPN, const uint32_t logicOperator, const uint32_t scriptNumber)
{

	switch (scriptNumber)
	{
	//Если operand один но есть вероятность что перед ним есть условие "Наверно что"
	case 1:
	{
		switch (logicOperator)
		{
		case 3:
		{
			// Унарный оператор 
			std::format_to(std::back_inserter(bufferScripts), "$nF\n");
			break;
		}
		case 4:
		{
			// конец скрипта в первм окне qed
			one_stack_end_operations(bufferScripts, operand1, stack_for_RPN);
			break;
		}
		}

		break;
	}
	//Если operand более одного
	default:
	{
		switch (logicOperator)
		{
		case 1:
		{
			// И
			if (stack_for_RPN.size() > 2)
			{
				stack_operations_bracket(operand1, operand2, stack_for_RPN, "$&\n", "$(\n", "$)\n");
			}
			else
			{
				if (stack_for_RPN.size() == 1)
					throw std::logic_error("There is only one element in stack_for_RPN");
				stack_operations(operand1, operand2, stack_for_RPN, "$&\n");
			}

			break;
		}
		case 2:
		{
			// ИЛИ
			if (stack_for_RPN.size() > 2)
			{
				stack_operations_bracket(operand1, operand2, stack_for_RPN, "$|\n", "$(\n", "$)\n");
			}
			else
			{
				if (stack_for_RPN.size() == 1)
					throw std::logic_error("There is only one element in stack_for_RPN");
				stack_operations(operand1, operand2, stack_for_RPN, "$|\n");
			}

			break;
		}
		case 3:
		{
			// Наверно что
			stack_operations_NF(operand2, stack_for_RPN, "$nF\n");
			break;
		}
		case 4:
		{
			// конец скрипта в первм окне qed
			if (stack_for_RPN.size() > 1)
			{

				two_stack_end_operations(bufferScripts, operand1, operand2, stack_for_RPN);
			}
			else
			{
				one_stack_end_operations(bufferScripts, operand1, stack_for_RPN);
			}
			break;
		}
		}
	}
	}
}

void getScriptSize(std::vector<uint8_t>& bufferScripts, uint32_t num_scripts)
{
	//Функция определения размера скрипта
	std::vector<uint8_t> cap_scripts;
	std::format_to(std::back_inserter(cap_scripts), "script \"{}\" size {}\n", num_scripts, bufferScripts.size());
	bufferScripts.insert(bufferScripts.begin(), cap_scripts.begin(), cap_scripts.end());
	cap_scripts.clear();
	//В конце каждого скрипта идет эта каретка, в размер скрипта не входит
	std::format_to(std::back_inserter(bufferScripts), "\r\n");
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

	std::vector<std::string_view> nameunit;
	nameunit.reserve(mis_unitnames.size() / 16);
	int numOfNames = 0;
	while (numOfNames * 16 < mis_unitnames.size())
	{
		nameunit.push_back(mis_unitnames.data() + numOfNames * 16);
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

	scripts1 struct_scripts;
	scripts2 scripts;

	// uint32_t numberOfScripts = *(uint32_t*)mis_scripts.data();
	// std::println("Number of scripts: {}", numberOfScripts);

	std::vector<uint8_t> bufferScripts;
	std::vector<uint8_t> outputFileBuffer;
	std::vector<uint8_t> operand1, operand2;

	std::stack< std::vector<uint8_t> > stack_for_RPN;

	uint32_t accumulator_num_scripts = 1;

	std::array <std::string, 200> numberscripts;

	for (size_t curOffset = 4; curOffset + sizeof(scripts1) < mis_scripts.size(); curOffset)
	{
		std::memcpy(&struct_scripts, mis_scripts.data() + curOffset, sizeof(scripts1));

		uint32_t n = 0;
		uint32_t key = 0;
		uint32_t script_num_for_OPN = 0;
		curOffset += sizeof(scripts1);

		for (int i = 0; (struct_scripts.size_of_script - sizeof(scripts2)) / sizeof(scripts2) > i; i++)
		{
			uint8_t logicOperator = 0;
			std::memcpy(&scripts, mis_scripts.data() + curOffset, sizeof(scripts2));
			curOffset += sizeof(scripts2);
			{
				//Переменные значения
				switch (scripts.num4)
				{
				case 255:
				{
					// Операция с координатой ячеек памяти num1 == 0 - 99 num2 == 255 && num3 == 255 && num4 == 255 (для qed num4 == 255 - 128)
					numberscripts[n] = reverse_num(((0x7F) << 24) | ((0xFF) << 16) | ((0xFF) << 8) | (scripts.num1 & 0xFF));
					n++;
				}
				break;
				case START + 1:
				{
					// num4 здесь приравнивается к 0 т.к. значение 128 фактически равно 0 и не хранит информацию об Ai командах все что более 128 уже имет какое либо значение комнды Ai
					numberscripts[n] = reverse_num(((0x00) << 24) | ((scripts.num3 & 0xFF) << 16) | ((scripts.num2 & 0xFF) << 8) | (scripts.num1 & 0xFF));
					n++;
				}
				break;
				case START + 2:
				{

					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 3:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 4:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 5:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 6:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 7:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START + 8:
				{
					numberscripts[n] = reverse_num(readUint32(scripts));
					n++;
				}
				break;
				case START:
				{
					//END - конец скрипта
					std::format_to(std::back_inserter(bufferScripts), "$END\n");
				}
				break;
				case bufferNONE:
					switch (scripts.num1)
					{
						//Логические операторы
					case bufferAND: logicOperator = 1; break;
					case bufferOR: logicOperator = 2; break;
					case bufferNF: logicOperator = 3; break;

						//Скрипты из первого окна редактора карт

					case bufferUG: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ug\n$UMSK\n#{}\n$ug1\n$Grp\n#{}\n$sp_8\n$CABE\n#{}\n$sp_9\n$N1\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 3, 0, 0, 0 #{} плейсхолдеры

					case bufferUP: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$up\n$UMSK\n#{}\n$up1\n$Plr\n#{}\n$sp_b\n$CABE\n#{}\n$sp_c\n$N1\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 4, 0, 0, 0

					case bufferUGL: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ugl\n$CABE\n#{}\n$N1\n#{}\n$umx0\n$Grp\n#{}\n$ugl1\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 5, 0, 0, 0

					case bufferUPL: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$upl\n$CABE\n#{}\n$N1\n#{}\n$umx2\n$Plr\n#{}\n$upl1\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 6, 0, 0, 0

					case bufferPKP: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$pkp\n$Plr\n#{}\n$pkp1\n$CABE\n#{}\n$sp_d\n$N1\n#{}\n$sp_e\n$PT\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 9, 0, 0, 0

					case bufferPKF: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$pkf\n$Plr\n#{}\n$pkf1\n$CABE\n#{}\n$sp_f\n$N1\n#{}\n$pkf2\n$PT\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 10, 0, 0, 0

					case bufferTE: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$te\n$Tr\n#{}\n"
						, numberscripts[key]);
						key++; script_num_for_OPN += 1; break; // 27, 0, 0, 0

					case bufferCD: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$cd\n$CABE\n#{}\n$sp_5\n$T\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 28, 0, 0, 0

					case bufferTMS: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$tms\n$CAB\n#{}\n$sp_6\n$T\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 29, 0, 0, 0

					case bufferUGLper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ugl%\n$CABE\n#{}\n$N1\n#{}\n$umx1\n$Grp\n#{}\n$ugl1\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 30, 0, 0, 0

					case bufferUPLper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$upl%\n$CABE\n#{}\n$N1\n#{}\n$umx3\n$Plr\n#{}\n$upl1\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; script_num_for_OPN += 1; break; // 31, 0, 0, 0

					case bufferUGper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$ug%u\n$UMSK\n#{}\n$umx4\n$Grp\n#{}\n$sp_7\n$CABE\n#{}\n$sp_k\n$N1\n#{}\n$ugut1\n$UMSK\n#{}\n$ug%u1\n$Grp\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3], numberscripts[key + 4], numberscripts[key + 5]);
						key += 6; script_num_for_OPN += 1; break; // 32, 0, 0, 0

					case bufferUPper: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$up%\n$UMSK\n#{}\n$up%u1\n$Plr\n#{}\n$sp_a\n$CABE\n#{}\n$sp_k\n$N1\n#{}\n$ugut1\n$UMSK\n#{}\n$up%u2\n$Plr\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3], numberscripts[key + 4], numberscripts[key + 5]);
						key += 6; script_num_for_OPN += 1; break; // 33, 0, 0, 0

					case bufferAIGB: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aigb\n$Grp\n#{}\n$aigb1\n$AI\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 35, 0, 0, 0

					case bufferAIZ1: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aiz1\n$Grp\n#{}\n$aiz_1\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 35, 0, 0, 0

					case bufferAIZ2: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aiz2\n$Grp\n#{}\n$aiz_2\n$L\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 36, 0, 0, 0

					case bufferAIG1: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aig1\n$Grp\n#{}\n$aig_1\n$Grp\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 37, 0, 0, 0

					case bufferAIG2: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$aig2\n$Grp\n#{}\n$aig_2\n$Grp\n#{}\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; script_num_for_OPN += 1; break; // 38, 0, 0, 0

					case bufferOID: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$oid\n$Obj\n#{}\n$oid1\n"
						, numberscripts[key]);
						key++; script_num_for_OPN += 1; break; // 41, 0, 0, 0

					case bufferGWATA: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$gwata\n$Grp\n#{}\n$gwata1\n$CAB\n#{}\n$T\n#{}\n$gwata2\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; script_num_for_OPN += 1; break; // 49, 0, 0, 0

					case bufferVIC: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$vic\n$Lvar\n#{}\n$CABE\n#{}\n$Rvar\n#{}\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; script_num_for_OPN += 1; break; // 50, 0, 0, 0

					case bufferMIST: std::format_to(std::back_inserter(stack_for_RPN.emplace()), "$mist\n"); script_num_for_OPN += 1; break;

						//END - разделитель между двумя окнами скриптов в редакторе карт QED
					case bufferEND: logicOperator = 4; break;
						//std::format_to(std::back_inserter(bufferScripts), "$END\n!\n");
						//break; // 7, 0, 0, 0

						//Скрипты из второго окна редактора карт
					case bufferSPPL: std::format_to(std::back_inserter(bufferScripts), "$sppl\n$I\n#{}\n$spc\n$PT\n#{}\n$sppl1\n$P\n#{}\n$sppl2\n$L\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; break; // 8, 0, 0, 0

					case bufferETC: std::format_to(std::back_inserter(bufferScripts), "$etc\n$\\\\n\n");
						break; // 11, 0, 0, 0

					case bufferSTRT: std::format_to(std::back_inserter(bufferScripts), "$strt\n$Tr\n#{}\n$strt1\n$T\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 12, 0, 0, 0

					case bufferSTPT: std::format_to(std::back_inserter(bufferScripts), "$stpt\n$Tr\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 13, 0, 0, 0

					case bufferMSTL: std::format_to(std::back_inserter(bufferScripts), "$mstl\n$L\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 14, 0, 0, 0

					case bufferSP: std::format_to(std::back_inserter(bufferScripts), "$sp\n$phrs\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 15, 0, 0, 0

					case bufferSCD: std::format_to(std::back_inserter(bufferScripts), "$scd\n$T\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 16, 0, 0, 0

					case bufferSNM: std::format_to(std::back_inserter(bufferScripts), "$snm\n$mission\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 17, 0, 0, 0

					case bufferTM: std::format_to(std::back_inserter(bufferScripts), "$tm\n$F\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 18, 0, 0, 0

					case bufferSGB: std::format_to(std::back_inserter(bufferScripts), "$sgb\n$G\n#{}\n$sgb1\n$AI\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 19, 0, 0, 0 

					case bufferSGL1: std::format_to(std::back_inserter(bufferScripts), "$sgl1\n$G\n#{}\n$sgl11\n$L\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 20, 0, 0, 0 

					case bufferSGL2: std::format_to(std::back_inserter(bufferScripts), "$sgl2\n$G\n#{}\n$sgl21\n$L\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 21, 0, 0, 0

					case bufferSGG1: std::format_to(std::back_inserter(bufferScripts), "$sgg1\n$G\n#{}\n$sgg11\n$G\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 22, 0, 0, 0

					case bufferSGG2: std::format_to(std::back_inserter(bufferScripts), "$sgg2\n$G\n#{}\n$sgg21\n$G\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 23, 0, 0, 0

					case bufferAPP: std::format_to(std::back_inserter(bufferScripts), "$app\n$P\n#{}\n$spc\n$I\n#{}\n$spc\n$PT\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 24, 0, 0, 0

					case bufferAFP: std::format_to(std::back_inserter(bufferScripts), "$afp\n$P\n#{}\n$spc\n$I\n#{}\n$afp1\n$PT\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 25, 0, 0, 0

					case bufferRU: std::format_to(std::back_inserter(bufferScripts), "$ru\n$G\n#{}\n$ru1\n$flg\n#{}\n$ru2\n$L\n#{}\n$ru3\n$T\n#{}\n$ru4\n$I\n#{}\n$ru5\n$I\n#{}\n$ru5\n$I\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3], numberscripts[key + 4], numberscripts[key + 5], numberscripts[key + 6]);
						key += 7; break; // 39, 0, 0, 0

					case bufferSRFS: std::format_to(std::back_inserter(bufferScripts), "$srfs\n$P\n#{}\n$srfs0\n$resv\n#{}\n$srfs1\n$flg\n#{}\n$srfs2\n$L\n#{}\n$srfs3\n$T\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3], numberscripts[key + 4]);
						key += 5; break; // 40, 0, 0, 0

					case bufferSPTO: std::format_to(std::back_inserter(bufferScripts), "$spto\n$I\n#{}\n$spc\n$PT\n#{}\n$spto1\n$P\n#{}\n$spto2\n$Obj\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; break; // 42, 0, 0, 0

					case bufferSAT: std::format_to(std::back_inserter(bufferScripts), "$sat\n$\\\\n\n");
						break; // 43, 0, 0, 0

					case bufferARPO: std::format_to(std::back_inserter(bufferScripts), "$arpo\n$Obj\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 44, 0, 0, 0

					case bufferARPO2: std::format_to(std::back_inserter(bufferScripts), "$arpo\n$Obj\n#{}\n$\\\\n\n"
						, numberscripts[key]);
						key++; break; // 45, 0, 0, 0

					case bufferSPPA: std::format_to(std::back_inserter(bufferScripts), "$sppa\n$I\n#{}\n$spc\n$PT\n#{}\n$sppa1\n$P\n#{}\n$sppa2\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 46, 0, 0, 0

					case bufferLCCV: std::format_to(std::back_inserter(bufferScripts), "$lccv\n$Lvar\n#{}\n$lccv1\n$Rvar\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 47, 0, 0, 0 //$Rvar не расшифрован

					case bufferMO: std::format_to(std::back_inserter(bufferScripts), "$mo\n$Lvar\n#{}\n$spc\n$Math\n#{}\n$spc\n$Rvar\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 48, 0, 0, 0 //$Math не расшифрован

					case bufferSRES: std::format_to(std::back_inserter(bufferScripts), "$sres\n$resv\n#{}\n$sres1\n$resv\n#{}\n$sres2\n$resv\n#{}\n$sres3\n$T\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; break; // 52, 0, 0, 0

					case bufferFRES: std::format_to(std::back_inserter(bufferScripts), "$fres\n$Fmask\n#{}\n$fres0\n$Fmask\n#{}\n$fres1\n$Fmask\n#{}\n$fres2\n$T\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2], numberscripts[key + 3]);
						key += 4; break; // 53, 0, 0, 0

					case bufferPFF: std::format_to(std::back_inserter(bufferScripts), "$pff\n$Fmask\n#{}\n$spc\n$I\n#{}\n$spc\n$PT\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 54, 0, 0, 0

					case bufferFPFF: std::format_to(std::back_inserter(bufferScripts), "$fpff\n$Fmask\n#{}\n$spc\n$I\n#{}\n$fpff1\n$PT\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1], numberscripts[key + 2]);
						key += 3; break; // 55, 0, 0, 0

					case bufferMFF: std::format_to(std::back_inserter(bufferScripts), "$mff\n$Fmask\n#{}\n$spc\n$phrs\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 56, 0, 0, 0

					case bufferSPPO: std::format_to(std::back_inserter(bufferScripts), "$sppo\n$phrs\n#{}\n$sppo1\n$Obj\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 57, 0, 0, 0

					case bufferDGP: std::format_to(std::back_inserter(bufferScripts), "$dgp\n$G\n#{}\n$dgp1\n$P\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break; // 58, 0, 0, 0

					case bufferFVKGZ: std::format_to(std::back_inserter(bufferScripts), "$fvkgz\n$G\n#{}\n$fvkgz1\n$L\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break;

					case bufferFVKGO: std::format_to(std::back_inserter(bufferScripts), "$fvkgo\n$G\n#{}\n$fvkgo1\n$Obj\n#{}\n$\\\\n\n"
						, numberscripts[key], numberscripts[key + 1]);
						key += 2; break;

						//Неизвестные скрипты
					default:
					{
						std::format_to(std::back_inserter(bufferScripts), "unknown scripts {} {} {} {}\n",
							static_cast<uint16_t>(scripts.num1),
							static_cast<uint16_t>(scripts.num2),
							static_cast<uint16_t>(scripts.num3),
							static_cast<uint16_t>(scripts.num4));
						std::println("\033[31m[Error]\033[0m Found unknown script in {}: {} {} {} {}.", m_stemFileName, scripts.num1, scripts.num2, scripts.num3, scripts.num4);
						break;
					}
					}
					break;
				}
				try
				{
					convertOPN(bufferScripts, operand1, operand2, stack_for_RPN, logicOperator, script_num_for_OPN);
				}
				catch (std::logic_error e)
				{
					std::println("\033[31m[Error]\033[0m Got exception for {:x} {:x} {:x} {:x}: {}", scripts.num1, scripts.num2, scripts.num3, scripts.num4, e.what());
				}
			}
		}

		getScriptSize(bufferScripts, accumulator_num_scripts);
		outputFile.write((char*)bufferScripts.data(), bufferScripts.size());
		bufferScripts.clear(); // чистка вектора
		accumulator_num_scripts++;
	}

	outputFile.close();
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

		{
			const uint16_t RGB_Color = *(uint16_t*)(buffer + 33);
			const uint16_t red = (RGB_Color >> 11) & 0x1F;
			const uint16_t green = (RGB_Color >> 5) & 0x3F;
			const uint16_t blue = RGB_Color & 0x1F;
			outputMisPlayers << std::format("Player {}\n name=\"{}\"\n team={}\n nation={}\n color={} {} {}\n"
				, curTry					// Player
				, (const char*)buffer		// name
				, *(uint8_t*)(buffer + 32)	// team
				, *(uint8_t*)(buffer + 35)	// nation
				, red * 8
				, green * 4
				, blue * 8);
		}

		const uint32_t Numberbomb = *(uint32_t*)(buffer + 69);
		const uint32_t Bombsbomb = *(uint32_t*)(buffer + 73);
		const uint32_t Reloadbomb = *(uint32_t*)(buffer + 77);
		outputMisPlayers << std::format(" bomb\n  ID={}\n  Number={}\n  Bombs={}\n  Reload={}\n"
			, (const char*)buffer + 37
			, Numberbomb
			, Bombsbomb
			, Reloadbomb);

		const uint32_t Numberspy = *(uint32_t*)(buffer + 113);
		const uint32_t Bombsspy = *(uint32_t*)(buffer + 117);
		const uint32_t Reloadspy = *(uint32_t*)(buffer + 121);
		outputMisPlayers << std::format(" spy\n  ID={}\n  Number={}\n  Bombs={}\n  Reload={}\n"
			, (const char*)buffer + 81
			, Numberspy
			, Bombsspy
			, Reloadspy);

		const uint32_t Numbertransport = *(uint32_t*)(buffer + 157);
		const uint32_t Bombstransport = *(uint32_t*)(buffer + 161);
		const uint32_t Reloadtransport = *(uint32_t*)(buffer + 165);
		outputMisPlayers << std::format(" transport\n  ID={}\n  Number={}\n  Bombs={}\n  Reload={}\n"
			, (const char*)buffer + 125
			, Numbertransport
			, Bombstransport
			, Reloadtransport);

		const uint32_t Numberboxer = *(uint32_t*)(buffer + 201);
		const uint32_t Bombsboxer = *(uint32_t*)(buffer + 205);
		const uint32_t Reloadboxer = *(uint32_t*)(buffer + 209);
		outputMisPlayers << std::format(" boxer\n  ID={}\n  Number={}\n  Bombs={}\n  Reload={}\n"
			, (const char*)buffer + 169
			, Numberboxer
			, Bombsboxer
			, Reloadboxer);

		outputMisPlayers << std::format(" descent 0\n  group={}\n  expa={}\n"
			, *(uint8_t*)(buffer + 213)
			, *(uint8_t*)(buffer + 214));

		outputMisPlayers << std::format("  ID 0={}\n  number 0={}\n  ID 1={}\n  number 1={}\n  ID 2={}\n  number 2={}\n  ID 3={}\n  number 3={}\n"
			, (const char*)buffer + 215
			, *(uint8_t*)(buffer + 279)
			, (const char*)buffer + 231
			, *(uint8_t*)(buffer + 280)
			, (const char*)buffer + 247
			, *(uint8_t*)(buffer + 281)
			, (const char*)buffer + 263
			, *(uint8_t*)(buffer + 282));

		outputMisPlayers << std::format(" descent 1\n  group={}\n  expa={}\n"
			, *(uint8_t*)(buffer + 283)
			, *(uint8_t*)(buffer + 284));

		outputMisPlayers << std::format("  ID 0={}\n  number 0={}\n  ID 1={}\n  number 1={}\n  ID 2={}\n  number 2={}\n  ID 3={}\n  number 3={}\n"
			, (const char*)buffer + 285
			, *(uint8_t*)(buffer + 349)
			, (const char*)buffer + 301
			, *(uint8_t*)(buffer + 350)
			, (const char*)buffer + 317
			, *(uint8_t*)(buffer + 351)
			, (const char*)buffer + 333
			, *(uint8_t*)(buffer + 352));

		outputMisPlayers << std::format(" planesdir={}\n", *(uint8_t*)(buffer + 36));	// planesdir
	}
	outputMisPlayers.close();
}