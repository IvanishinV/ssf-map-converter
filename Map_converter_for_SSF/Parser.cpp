
#include "stdafx.h"

#include "Parser.h"
#include "util.h"
#include "Displayinfo.h"
#include "parse/parse_smm.h"
#include "parse/parse_ssm.h"
#include "parse/parse_ssc_map.h"
#include "parse/parse_scc_mission.h"

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

	if (zipData.size() < 8)
	{
		std::println("");
		own::printlnWarning(Dictionary::getValue(STRINGS::ERROR_FILE), filepath.string());
		return;
	}

	const std::string rawData = (*(uint16_t*)zipData.data() == 0x8b1f) ?
		gzip::decompress(zipData.data(), zipData.size()) :
		std::string(zipData.cbegin(), zipData.cend());

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
			parse::parse_ssm(m_mapFolder, rawData);
			break;
		}
		case HEADER_MULTI:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_MULTI), m_stemFileName);
			parse::parse_smm(m_mapFolder, rawData);
			break;
		}
		case HEADER_CAMP_MAP:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MAP), m_stemFileName);
			parse::parse_ssc_map(m_mapFolder, rawData);
			break;
		}
		case HEADER_CAMP_MIS:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MIS), m_stemFileName);
			parse::parse_scc_mission(m_mapFolder, rawData);
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
