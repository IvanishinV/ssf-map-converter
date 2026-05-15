#include "mis_desc.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void mis_desc(const std::filesystem::path& mis_folder)
{
	std::ofstream f(mis_folder / "desc", std::ios::binary);
	if (!f)
		errorWriteFile("mis/desc");
}

} // namespace convert
