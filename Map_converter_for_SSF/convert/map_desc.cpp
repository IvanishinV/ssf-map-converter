#include "map_desc.h"
#include "../wire/headers.h"

#include <fstream>
#include <print>

namespace convert {

void map_desc(const std::filesystem::path& map_folder,
              uint32_t map_type,
              std::string_view stem)
{
	std::ofstream f(map_folder / "desc", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write desc");
		return;
	}

	if (map_type == HEADER_SINGLE || map_type == HEADER_MULTI)
		f.write(stem.data(), static_cast<std::streamsize>(stem.size()));
}

} // namespace convert
