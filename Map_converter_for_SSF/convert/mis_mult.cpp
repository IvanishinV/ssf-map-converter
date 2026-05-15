#include "mis_mult.h"
#include "../Displayinfo.h"
#include "../util.h"
#include "../Displayinfo.h"

#include <fstream>

namespace convert {

void mis_mult(const std::filesystem::path& mis_folder,
              std::string_view map_info,
              uint32_t maptypeheader)
{
	std::ofstream f(mis_folder / "mismult", std::ios::binary);
	if (!f)
	{
		errorWriteFile("mismult");
		return;
	}

	if (maptypeheader == 0 || maptypeheader == 1)
	{
		f.write(map_info.data() + 24, 16);
	}
	else
	{
		for (uint32_t n = 0; n < 16; ++n)
			f << GLOBALNULL;
	}
}

} // namespace convert
