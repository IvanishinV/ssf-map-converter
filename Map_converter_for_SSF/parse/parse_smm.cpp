#include "parse_smm.h"
#include "../Helper.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../util.h"

#include <fstream>
#include <span>

namespace parse {

void parse_smm(const std::filesystem::path& map_folder, std::string_view data)
{
	WireReader r{std::as_bytes(std::span{data})};
	std::filesystem::create_directories(map_folder);
	std::ofstream outputMapInfo(map_folder / "map_info", std::ios::binary);
	std::ofstream outputMisScripts(map_folder / "mis_scripts", std::ios::binary);
	std::ofstream outputMisDesc(map_folder / "mis_desc", std::ios::binary);
	std::ofstream outputMiniMap(map_folder / "map_mini", std::ios::binary);
	std::ofstream outputMapRhombs(map_folder / "map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags(map_folder / "map_flags", std::ios::binary);
	std::ofstream outputMapLandnames(map_folder / "map_landnames", std::ios::binary);
	std::ofstream outputMapObjects(map_folder / "map_objects", std::ios::binary);
	std::ofstream outputMisUnitNames(map_folder / "mis_unitnames", std::ios::binary);
	std::ofstream outputMisWoofers(map_folder / "mis_woofers", std::ios::binary);
	std::ofstream outputMisZones(map_folder / "mis_zones", std::ios::binary);
	std::ofstream outputMapMines(map_folder / "map_mines", std::ios::binary);
	std::ofstream outputMisSupport(map_folder / "mis_support", std::ios::binary);
	std::ofstream outputMisPlayers(map_folder / "mis_players", std::ios::binary);
	std::ofstream outputMisPhrases(map_folder / "mis_phrases", std::ios::binary);
	std::ofstream outputMisMapUnits(map_folder / "mis_mapunits", std::ios::binary);
	std::ofstream outputMisObjects(map_folder / "mis_objects", std::ios::binary);

	uint32_t mapIdentifier = r.peek_at<uint32_t>(136);
	uint32_t mapSizeU      = r.peek_at<uint32_t>(140);
	uint32_t mapSizeV      = r.peek_at<uint32_t>(144);

	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize   = tileArray(mapSizeU, mapSizeV, 4);

	uint32_t startPositionMisObjects  = position(data, outputMapInfo,    FILE_TYPE_OFFSET,           MapHeaderSMM);
	uint32_t startPositionMisScripts  = position(data, outputMisObjects, startPositionMisObjects,     MISOBJECTS);
	uint32_t sizeMisScripts           = r.peek_at<uint32_t>(startPositionMisScripts);
	uint32_t accumulator              = misScripts(data, sizeMisScripts, startPositionMisScripts);
	uint32_t startPositionMisDesc     = position(data, outputMisScripts, startPositionMisScripts,     accumulator + 4);
	uint32_t BriefingSize             = r.peek_at<uint32_t>(startPositionMisDesc);
	uint32_t startPositionMapMini     = position(data, outputMisDesc,    startPositionMisDesc + 4,    BriefingSize);
	uint32_t MapMiniSize              = minimapsize(mapSizeU, mapSizeV);
	uint32_t startPositionRhombs      = position(data, outputMiniMap,    startPositionMapMini,        MapMiniSize);
	uint32_t startPositionFlags       = position(data, outputMapRhombs,  startPositionRhombs,         rhombsSize);
	uint32_t startPositionLandnames   = position(data, outputMapFlags,   startPositionFlags,          flagSize);
	uint32_t startPositionMapObjects  = position(data, outputMapLandnames, startPositionLandnames,    LANDNAMESSIZE);
	uint32_t mapSizeMapObjects        = r.peek_at<uint32_t>(startPositionMapObjects);
	uint32_t startPositionMisUnitNames = position(data, outputMapObjects, startPositionMapObjects + 4, mapSizeMapObjects * 8);
	uint32_t sizeMisUnitNames         = r.peek_at<uint32_t>(startPositionMisUnitNames);
	uint32_t startPositionMisWoofers  = position(data, outputMisUnitNames, startPositionMisUnitNames + 4, sizeMisUnitNames * 16);
	uint32_t sizeMisWoofers           = r.peek_at<uint32_t>(startPositionMisWoofers);
	uint32_t startPositionMisZones    = position(data, outputMisWoofers, startPositionMisWoofers,     sizeMisWoofers * MISMUSICSIZE + 4);
	uint32_t startPositionMapMines    = position(data, outputMisZones,   startPositionMisZones,       mapSizeU * mapSizeV);
	uint32_t startPositionMisSupport  = position(data, outputMapMines,   startPositionMapMines,       mapSizeU * mapSizeV);
	uint32_t sizeMisSupport           = r.peek_at<uint32_t>(startPositionMisSupport);
	uint32_t startPositionMisPlayers  = position(data, outputMisSupport, startPositionMisSupport,     sizeMisSupport + 4);
	uint32_t sizeMisPlayers           = r.peek_at<uint32_t>(startPositionMisPlayers);
	uint32_t startPositionMisPhrases  = position(data, outputMisPlayers, startPositionMisPlayers + 4, MISPLAYERSSIZE * sizeMisPlayers);
	uint32_t sizeMisPhrases           = r.peek_at<uint32_t>(startPositionMisPhrases);
	uint32_t startPositionMisMapUnits = position(data, outputMisPhrases, startPositionMisPhrases,     sizeMisPhrases + 4);
	uint32_t sizeMisMapUnits          = r.peek_at<uint32_t>(startPositionMisMapUnits);
	uint32_t mapEndPosition           = position(data, outputMisMapUnits, startPositionMisMapUnits + 4, sizeMisMapUnits);

	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
}

} // namespace parse
