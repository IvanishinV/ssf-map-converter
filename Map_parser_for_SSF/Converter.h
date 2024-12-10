#pragma once

#include "util.h"

class Converter
{
public:
	void convertMap(const std::string_view& inputData, const std::string_view& filename);

private:
	std::string m_mapFileName;
	std::string_view m_rawMapFileName;
	std::filesystem::path m_mapFolder;
	std::filesystem::path m_misFolder;

	uint32_t convertMapFileSMM(const std::string_view& inputData);
	uint32_t convertMapFileSSM(const std::string_view& inputData);
	uint32_t convertMapFileSSC_map(const std::string_view& inputData);
	uint32_t convertMapFileSCC_mission(const std::string_view& inputData);

	void convertMapInfo(const uint32_t mapIdentifier, const uint32_t mapSizeU, const uint32_t mapSizeV) const;
	void convertMapMini(const std::string_view& map_mini) const;
	void convertMapLandname(const std::string_view& map_landname) const;
	void convertMapCflags(const std::string_view& map_flags) const;
	void convertMapObjects(const std::string_view& map_objects) const;
	void convertMapDesc() const;
	void convertMapRhombs(const std::string_view& map_rhombs, const uint32_t mapIdentifier) const;
	void convertMisDesc() const;
	void convertMisMult(const std::string_view& map_info, const uint32_t maptypeheader) const;
	void convertMisInfo(const uint32_t mapSizeU, const uint32_t mapSizeV) const;
	void convertMisZones(const std::string_view& mis_zones, const uint32_t mapSizeU) const;
	void convertMisUnits(const std::string_view& mis_unitnames, const std::string_view& mis_mapunits, const std::string_view& mis_support) const;
	void convertMisGroups(const std::string_view& mis_groups) const;
	void convertMisScripts(const std::string_view& mis_scripts) const;
	void convertMisWoofers(const std::string_view& mis_woofers) const;
	void convertMisMines(const std::string_view& map_mines, const uint32_t mapSizeU) const;
	void convertMisTree(const std::string_view& mis_desc) const;
	void convertMisPhrases(const std::string_view& mis_phrases, const uint32_t sizeMisPhrases) const;
	void convertMisObjects(const std::string_view& mis_objects) const;
	void convertMisPlayers(const std::string_view& mis_players) const;
};