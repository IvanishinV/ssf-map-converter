#include "mis_mines.h"
#include "../util.h"

#include <algorithm>
#include <fstream>
#include <print>
#include <vector>

namespace convert {

void mis_mines(const std::filesystem::path& mis_folder,
               std::string_view data,
               uint32_t map_size_u)
{
	std::ofstream f(mis_folder / "mines", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write mines");
		return;
	}

	std::vector<uint8_t> mapMines(LOCATIONSSIZE, 0);

	const size_t maxTries = data.size() / map_size_u;
	for (size_t i = 0; i < maxTries; ++i)
		std::copy(data.cbegin() + i * map_size_u,
		          data.cbegin() + i * map_size_u + map_size_u,
		          mapMines.begin() + 512 * i);

	f.write(reinterpret_cast<const char*>(mapMines.data()),
	        static_cast<std::streamsize>(mapMines.size()));
}

} // namespace convert
