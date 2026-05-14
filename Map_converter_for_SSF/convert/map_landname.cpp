#include "map_landname.h"

#include <fstream>
#include <print>

namespace convert {

void map_landname(const std::filesystem::path& map_folder,
                  std::string_view data)
{
	std::ofstream f(map_folder / "landname", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write landname");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
