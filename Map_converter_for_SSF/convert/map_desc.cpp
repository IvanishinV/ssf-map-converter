#include "map_desc.h"
#include "../Displayinfo.h"
#include "../wire/headers.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void map_desc(const std::filesystem::path& map_folder,
              uint32_t map_type,
              std::string_view stem)
{
	std::ofstream f(map_folder / "desc", std::ios::binary);
	if (!f)
	{
		errorWriteFile("desc");
		return;
	}

	if (map_type == HEADER_SINGLE || map_type == HEADER_MULTI)
		f.write(stem.data(), static_cast<std::streamsize>(stem.size()));
}

} // namespace convert
