#include "map_objects.h"

#include <fstream>
#include <print>

namespace convert {

void map_objects(const std::filesystem::path& map_folder,
                 std::string_view data)
{
	std::ofstream f(map_folder / "objects", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write objects");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
