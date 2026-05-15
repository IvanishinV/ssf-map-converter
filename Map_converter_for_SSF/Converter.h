#pragma once

#include <filesystem>
#include <string>

#include "util.h"

class Converter
{
public:
	void convertMap(const std::filesystem::path& filepath);

private:
	std::filesystem::path m_mapFolder;
	std::filesystem::path m_misFolder;
	std::string m_stemFileName;
	uint32_t m_mapType;
	uint32_t m_mapIdentifier;
	uint32_t m_mapSizeU;
	uint32_t m_mapSizeV;

	uint32_t convertMapFileSMM(const std::string_view& inputData);
	uint32_t convertMapFileSSM(const std::string_view& inputData);
	uint32_t convertMapFileSSC_map(const std::string_view& inputData);
	uint32_t convertMapFileSCC_mission(const std::string_view& inputData);
};
