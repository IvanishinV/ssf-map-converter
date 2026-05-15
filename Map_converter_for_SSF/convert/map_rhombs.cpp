#include "map_rhombs.h"
#include "../RhombsParser.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void map_rhombs(const std::filesystem::path& map_folder, std::string_view data, uint32_t map_identifier)
{
	std::ofstream f(map_folder / "rhombs", std::ios::binary);
	if (!f)
	{
		errorWriteFile();
		return;
	}
	RhombsParser::parse_rhombs(data, f, static_cast<RhombsParser::SchemeType>(map_identifier));
}

} // namespace convert
