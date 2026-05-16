
#include "Displayinfo.h"
#include "Lang.h"
#include "Parser.h"
#include "io/wire_reader.h"
#include "util.h"

#include <algorithm>
#include <fstream>
#include <print>
#include <span>
#include <string>
#include <vector>
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

	WireReader r_zip{std::as_bytes(std::span{zipData})};
	const std::string rawData = (r_zip.peek_at<uint16_t>(0) == 0x8b1f) ?
		gzip::decompress(zipData.data(), zipData.size()) :
		std::string(zipData.cbegin(), zipData.cend());

	std::wstring wfilepath = filepath.wstring();
	wfilepath.erase(std::remove_if(wfilepath.begin(), wfilepath.end(), [](wchar_t c) { return !((c >= 0 && c < 128) || (c >= 0x400 && c < 0x500)); }), wfilepath.cend());
	const std::filesystem::path filepath_ex{ wfilepath };
	const std::string stemFileName = filepath_ex.string();
	const std::filesystem::path fileFolder = filepath.parent_path();

	const uint32_t mapType = WireReader{std::as_bytes(std::span{rawData})}.peek_at<uint32_t>(0);
	std::filesystem::path mapFolder;
	switch (mapType)
	{
	case HEADER_SINGLE:
	case HEADER_MULTI:
		mapFolder = fileFolder / ("parser_map." + stemFileName);
		break;
	case HEADER_CAMP_MAP:
	case HEADER_CAMP_MIS:
		if (stemFileName.length() >= 3)
			mapFolder = fileFolder / ("parser_map." + stemFileName.substr(0, 3));
		break;
	}

	try
	{
		switch (mapType)
		{
		case HEADER_SINGLE:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_SINGLE), stemFileName);
			parse::parse_ssm(mapFolder, rawData);
			break;
		}
		case HEADER_MULTI:
		{
			own::print(Dictionary::getValue(STRINGS::MAP_MULTI), stemFileName);
			parse::parse_smm(mapFolder, rawData);
			break;
		}
		case HEADER_CAMP_MAP:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MAP), stemFileName);
			parse::parse_ssc_map(mapFolder, rawData);
			break;
		}
		case HEADER_CAMP_MIS:
		{
			own::print(Dictionary::getValue(STRINGS::CAMP_MIS), stemFileName);
			parse::parse_scc_mission(mapFolder, rawData);
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
