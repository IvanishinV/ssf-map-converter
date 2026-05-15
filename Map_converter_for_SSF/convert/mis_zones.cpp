#include "mis_zones.h"
#include "../Displayinfo.h"
#include "../util.h"
#include "../Displayinfo.h"

#include <algorithm>
#include <fstream>
#include <vector>

namespace convert {

void mis_zones(const std::filesystem::path& mis_folder,
               std::string_view data,
               uint32_t map_size_u)
{
	std::ofstream f(mis_folder / "locations", std::ios::binary);
	if (!f)
	{
		errorWriteFile("locations");
		return;
	}

	std::vector<uint8_t> misZones(LOCATIONSSIZE, 0);

	const size_t maxTries = data.size() / map_size_u;
	for (size_t i = 0; i < maxTries; ++i)
	{
		std::copy(data.cbegin() + i * map_size_u,
		          data.cbegin() + i * map_size_u + map_size_u,
		          misZones.begin() + 512 * i);
	}

	f.write(reinterpret_cast<const char*>(misZones.data()),
	        static_cast<std::streamsize>(misZones.size()));
}

} // namespace convert
