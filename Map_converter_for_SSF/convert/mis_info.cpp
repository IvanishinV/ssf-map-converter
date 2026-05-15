#include "mis_info.h"
#include "../Displayinfo.h"
#include "../util.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

namespace {

#pragma pack(push, 1)
struct MisInfoHeader
{
	MisInfoHeader(uint32_t sizeU, uint32_t sizeV)
		: unknown_1(GLOBALNULL)
		, unknown_2(GLOBALNULL)
		, mapSizeU(sizeU)
		, mapSizeV(sizeV)
	{}

	uint32_t unknown_1;
	uint32_t unknown_2;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
};
#pragma pack(pop)

static_assert(sizeof(MisInfoHeader) == 16);

} // namespace

void mis_info(const std::filesystem::path& mis_folder,
              uint32_t map_size_u,
              uint32_t map_size_v)
{
	std::ofstream f(mis_folder / "info", std::ios::binary);
	if (!f)
	{
		errorWriteFile("mis/info");
		return;
	}

	const MisInfoHeader header(map_size_u, map_size_v);
	f.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

} // namespace convert
