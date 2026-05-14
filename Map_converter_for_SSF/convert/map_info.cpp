#include "map_info.h"
#include "../util.h"

#include <fstream>
#include <print>

namespace convert {

namespace {

#pragma pack(push, 1)
struct MapInfoHeader
{
	MapInfoHeader(uint32_t identifier, uint32_t sizeU, uint32_t sizeV)
		: mapIdentifier(identifier)
		, mapSizeU(sizeU)
		, mapSizeV(sizeV)
		, unknown_1(GLOBALNULL)
		, unknown_2(GLOBALNULL)
		, unknown_3(GLOBALNULL)
	{}

	uint32_t mapIdentifier;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
	uint8_t  unknown_1;
	uint8_t  unknown_2;
	uint8_t  unknown_3;
};
#pragma pack(pop)

static_assert(sizeof(MapInfoHeader) == 15);

} // namespace

void map_info(const std::filesystem::path& map_folder,
              uint32_t map_identifier,
              uint32_t map_size_u,
              uint32_t map_size_v)
{
	std::ofstream f(map_folder / "info", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write info");
		return;
	}

	const MapInfoHeader header(map_identifier, map_size_u, map_size_v);
	f.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

} // namespace convert
