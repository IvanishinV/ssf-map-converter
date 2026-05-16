#pragma once

#include <cstdint>
#include <fstream>
#include <span>
#include <string_view>

class RhombsParser
{
	static void parse_scheme(const std::string_view& map_rhombs, std::ofstream& outputFile,
	                         std::span<const uint16_t> scheme_tiles,
	                         std::span<const uint8_t> v_B00,
	                         std::span<const uint8_t> v_420);

public:
	enum class SchemeType
	{
		Summer,
		Winter,
		Beach,
		Desert
	};

	static void parse_rhombs(const std::string_view& map_rhombs, std::ofstream& outputFile, const SchemeType);
};
