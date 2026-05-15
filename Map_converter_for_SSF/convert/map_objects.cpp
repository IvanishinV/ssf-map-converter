#include "map_objects.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void map_objects(const std::filesystem::path& map_folder,
                 std::string_view data)
{
	std::ofstream f(map_folder / "objects", std::ios::binary);
	if (!f)
	{
		errorWriteFile("objects");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
