#pragma once

#include <vector>
#include <fstream>

class RhombsParser
{
#pragma region consts
	static const std::vector<uint16_t> g_summer_tiles;
	static const std::vector<uint8_t> g_summer_B14;
	static const std::vector<uint8_t> g_summer_B00;
	static const std::vector<uint8_t> g_summer_420;

	static const std::vector<uint16_t> g_winter_tiles;
	static const std::vector<uint8_t> g_winter_B14;
	static const std::vector<uint8_t> g_winter_B00;
	static const std::vector<uint8_t> g_winter_420;

	static const std::vector<uint16_t> g_beach_tiles;
	static const std::vector<uint8_t> g_beach_B14;
	static const std::vector<uint8_t> g_beach_B00;
	static const std::vector<uint8_t> g_beach_420;

	static const std::vector<uint16_t> g_desert_tiles;
	static const std::vector<uint8_t> g_desert_B14;
	static const std::vector<uint8_t> g_desert_B00;
	static const std::vector<uint8_t> g_desert_420;
#pragma endregion consts

	static uint32_t get_rand();

	static void parse_scheme(const std::string_view& map_rhombs, std::ofstream& outputFile, const std::vector<uint16_t>& scheme_tiles, const std::vector<uint8_t>& v_B14, const std::vector<uint8_t>& v_B00, const std::vector<uint8_t>& v_420);

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