#include "map_landname.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void map_landname(const std::filesystem::path& map_folder,
                  std::string_view data)
{
	std::ofstream f(map_folder / "landname", std::ios::binary);
	if (!f)
	{
		errorWriteFile("landname");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
