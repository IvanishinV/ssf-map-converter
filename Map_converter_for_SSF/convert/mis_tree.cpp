#include "mis_tree.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void mis_tree(const std::filesystem::path& mis_folder,
              std::string_view data)
{
	std::ofstream f(mis_folder / "misdesc", std::ios::binary);
	if (!f)
	{
		errorWriteFile("misdesc");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
