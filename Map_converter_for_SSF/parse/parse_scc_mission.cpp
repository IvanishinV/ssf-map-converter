#include "parse_scc_mission.h"
#include "../Helper.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../util.h"

#include <fstream>
#include <span>

namespace parse {

void parse_scc_mission(const std::filesystem::path& map_folder, std::string_view data)
{
	WireReader r{std::as_bytes(std::span{data})};
	std::filesystem::create_directories(map_folder);
	std::ofstream outputMisDesc(map_folder / "mis_desc", std::ios::binary);
	std::ofstream outputMisUnitNames(map_folder / "mis_unitnames", std::ios::binary);
	std::ofstream outputMisGroups(map_folder / "mis_groups", std::ios::binary);
	std::ofstream outputMisMapUnits(map_folder / "mis_mapunits", std::ios::binary);
	std::ofstream outputMisPlayers(map_folder / "mis_players", std::ios::binary);
	std::ofstream outputMisWoofers(map_folder / "mis_woofers", std::ios::binary);
	std::ofstream outputMisZones(map_folder / "mis_zones", std::ios::binary);
	std::ofstream outputMisScripts(map_folder / "mis_scripts", std::ios::binary);
	std::ofstream outputMapMines(map_folder / "map_mines", std::ios::binary);
	std::ofstream outputMisSupport(map_folder / "mis_support", std::ios::binary);
	std::ofstream outputMisPhrases(map_folder / "mis_phrases", std::ios::binary);
	std::ofstream outputMisObjects(map_folder / "mis_objects", std::ios::binary);

	uint32_t mapSizeU = r.peek_at<uint32_t>(16);
	uint32_t mapSizeV = r.peek_at<uint32_t>(20);

	uint32_t startPositionMisDesc      = MapHeaderSSC_mission + 4;
	uint32_t sizeMisDesc               = r.peek_at<uint32_t>(MapHeaderSSC_mission);
	uint32_t startPositionMisUnitNames = position(data, outputMisDesc,      startPositionMisDesc,          sizeMisDesc);
	uint32_t sizeMisUnitNames          = r.peek_at<uint32_t>(startPositionMisUnitNames);
	uint32_t startPositionMisGroups    = position(data, outputMisUnitNames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t startPositionMisMapUnits  = position(data, outputMisGroups,    startPositionMisGroups,        MISGROUPSSIZE);
	uint32_t sizeMisMapUnits           = r.peek_at<uint32_t>(startPositionMisMapUnits);
	uint32_t startPositionMisPlayers   = position(data, outputMisMapUnits,  startPositionMisMapUnits + 4,  sizeMisMapUnits);
	uint32_t sizeMisPlayers            = r.peek_at<uint32_t>(startPositionMisPlayers);
	uint32_t startPositionMisWoofers   = position(data, outputMisPlayers,   startPositionMisPlayers + 4,   MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisWoofers            = r.peek_at<uint32_t>(startPositionMisWoofers);
	uint32_t startPositionMisZones     = position(data, outputMisWoofers,   startPositionMisWoofers,       sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMisScripts   = position(data, outputMisZones,     startPositionMisZones,         mapSizeU * mapSizeV);
	uint32_t sizeMisScripts            = r.peek_at<uint32_t>(startPositionMisScripts);
	uint32_t accumulator               = misScripts(data, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMapMines     = position(data, outputMisScripts,   startPositionMisScripts,       accumulator + 4);
	uint32_t startPositionMisSupport   = position(data, outputMapMines,     startPositionMapMines,         mapSizeU * mapSizeV);
	uint32_t sizeMisSupport            = r.peek_at<uint32_t>(startPositionMisSupport);
	uint32_t startPositionMisPhrases   = position(data, outputMisSupport,   startPositionMisSupport,       sizeMisSupport + 4);
	uint32_t sizeMisPhrases            = r.peek_at<uint32_t>(startPositionMisPhrases);
	uint32_t startPositionMisObjects   = position(data, outputMisPhrases,   startPositionMisPhrases,       sizeMisPhrases + 4);
	uint32_t mapEndPosition            = position(data, outputMisObjects,   startPositionMisObjects,       MISOBJECTS);

	displayinfo(mapSizeU, mapSizeV, 40, mapEndPosition);
}

} // namespace parse
