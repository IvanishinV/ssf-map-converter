#include "mis_woofers.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../Displayinfo.h"
#include "../wire/types.h"

#include <format>
#include <fstream>
#include <span>

namespace convert {

void mis_woofers(const std::filesystem::path& mis_folder,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "sounds", std::ios::binary);
	if (!f)
	{
		errorWriteFile("sounds");
		return;
	}

	WireReader r{std::as_bytes(std::span{data})};
	const uint32_t count = r.read<uint32_t>();

	for (uint32_t i = 0; i < count; ++i)
	{
		const auto src = r.read<woofers>();
		f << std::format("Name=\"{}\"\nU={}\nV={}\nRadius={}\nWorse={}\nMinWait={}\nMaxWait={}\n\n"
			, src.name
			, src.pos.u
			, src.pos.v
			, src.radius
			, src.worse
			, src.minWait
			, src.maxWait);
	}
}

} // namespace convert
