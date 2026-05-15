#include "parse_ssc_map.h"
#include "../Helper.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../util.h"

#include <fstream>
#include <span>

namespace parse {

void parse_ssc_map(const std::filesystem::path& map_folder, std::string_view data)
{
	WireReader r{std::as_bytes(std::span{data})};
	std::filesystem::create_directories(map_folder);
	std::ofstream outputMapInfo(map_folder / "map_info", std::ios::binary);
	std::ofstream outputMapLandnames(map_folder / "map_landnames", std::ios::binary);
	std::ofstream outputMapRhombs(map_folder / "map_rhombs", std::ios::binary);
	std::ofstream outputMapFlags(map_folder / "map_flags", std::ios::binary);
	std::ofstream outputMiniMap(map_folder / "map_mini1", std::ios::binary);
	std::ofstream outputMiniMap2(map_folder / "map_mini2", std::ios::binary);
	std::ofstream outputMapObjects(map_folder / "map_objects", std::ios::binary);

	uint32_t mapIdentifier = r.peek_at<uint32_t>(8);
	uint32_t mapSizeU      = r.peek_at<uint32_t>(12);
	uint32_t mapSizeV      = r.peek_at<uint32_t>(16);

	uint32_t rhombsSize = tileArray(mapSizeU, mapSizeV, 2);
	uint32_t flagSize   = tileArray(mapSizeU, mapSizeV, 4);

	uint32_t startPositionLandname = position(data, outputMapInfo,      FILE_TYPE_OFFSET,      MapHeaderSSC_map);
	uint32_t startPositionRhombs   = position(data, outputMapLandnames, startPositionLandname, LANDNAMESSIZE);
	uint32_t startPositionFlags    = position(data, outputMapRhombs,    startPositionRhombs,   rhombsSize);
	uint32_t startPositionMapMini  = position(data, outputMapFlags,     startPositionFlags,    flagSize);
	uint32_t startPositionMapMini2 = position(data, outputMiniMap,      startPositionMapMini,  rhombsSize);
	uint32_t startPositionObjects  = position(data, outputMiniMap2,     startPositionMapMini2, rhombsSize / 4);
	uint32_t mapSizeObjects        = r.peek_at<uint32_t>(startPositionObjects);
	uint32_t mapEndPosition        = position(data, outputMapObjects,   startPositionObjects + 4, mapSizeObjects * 8);

	displayinfo(mapSizeU, mapSizeV, mapIdentifier, mapEndPosition);
}

} // namespace parse
