#pragma once

#include <filesystem>
#include <string>

#include "util.h"

class Converter
{
public:
	void convertMap(const std::filesystem::path& filepath);

private:
	struct MapCtx
	{
		std::filesystem::path mapFolder;
		std::filesystem::path misFolder;
		std::string stemFileName;
		uint32_t mapType;
	};

	static uint32_t convertMapFileSMM(const std::string_view& inputData, const MapCtx& ctx);
	static uint32_t convertMapFileSSM(const std::string_view& inputData, const MapCtx& ctx);
	static uint32_t convertMapFileSSC_map(const std::string_view& inputData, const MapCtx& ctx);
	static uint32_t convertMapFileSCC_mission(const std::string_view& inputData, const MapCtx& ctx);
};
