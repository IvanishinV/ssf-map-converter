#pragma once

#include "util.h"

struct coordinatesFlag
{
	int u;
	int v;
};

struct flag
{
	coordinatesFlag		redFlag;
	coordinatesFlag		blueFlag;
	uint32_t			valueRedFlag;
};

struct unitBase //Entity, Passenger
{
	uint8_t grp;
	uint8_t hp;
	uint8_t ammo;
	uint8_t expa;
	uint8_t lives;
	uint8_t ID;
};

struct unitBaseIn //Vehicles, support
{
	unitBase unit;
	uint8_t in;
};

struct coordinates
{
	uint16_t u;
	uint16_t v;
};

#pragma pack(push, 1)
/**
 * @param unit - parameters entity or vehicles
 * @param pos - unit position on the map.
 * @param dir - unit rotation, clockwise from 0 to 31, 32 positions in total.
 * @param owner - player=0, enemy=1, ally=2, neutral=3.
 */
struct unitsMap
{
	unitBaseIn unit;
	coordinates pos;
	uint8_t dir; //todo возможно относится к структуре coordinates
	uint8_t owner;
};
#pragma pack (pop)

struct aviation
{
	char name[32];
	uint32_t number;
	uint32_t bombs;
	uint32_t reloads;
};

#pragma pack(push, 1)
struct descent
{
	uint8_t group;
	uint8_t expa;
	char ID0[16];
	char ID1[16];
	char ID2[16];
	char ID3[16];
	uint8_t number0;
	uint8_t number1;
	uint8_t number2;
	uint8_t number3;
};
#pragma pack (pop)

#pragma pack(push, 1)
struct players
{
	char name[32];
	uint8_t team;
	pixel	color;
	uint8_t nation;
	uint8_t planesdir;

	aviation airReinforcementBomb;
	aviation airReinforcementSpy;
	aviation airReinforcementTransport;
	aviation airReinforcementBoxer;

	descent group1;
	descent group2;
};
#pragma pack (pop)

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

	void convertMapInfo() const;
	void convertMapMini(const std::string_view& map_mini) const;
	void convertMapLandname(const std::string_view& map_landname) const;
	void convertMapCflags(const std::string_view& map_flags) const;
	void convertMapObjects(const std::string_view& map_objects) const;
	void convertMapDesc() const;
	void convertMapRhombs(const std::string_view& map_rhombs) const;
	void convertMisDesc() const;
	void convertMisMult(const std::string_view& map_info, const uint32_t maptypeheader) const;
	void convertMisInfo() const;
	void convertMisZones(const std::string_view& mis_zones) const;
	void convertMisUnits(const std::string_view& mis_unitnames, const std::string_view& mis_mapunits, const std::string_view& mis_support) const;
	void convertMisGroups(const std::string_view& mis_groups) const;
	void convertMisScripts(const std::string_view& mis_scripts) const;
	void convertMisWoofers(const std::string_view& mis_woofers) const;
	void convertMisMines(const std::string_view& map_mines) const;
	void convertMisTree(const std::string_view& mis_desc) const;
	void convertMisPhrases(const std::string_view& mis_phrases, const uint32_t sizeMisPhrases) const;
	void convertMisObjects(const std::string_view& mis_objects) const;
	void convertMisPlayers(const std::string_view& mis_players) const;
};