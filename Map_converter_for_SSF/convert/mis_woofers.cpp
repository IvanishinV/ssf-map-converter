#include "mis_woofers.h"
#include "../Helper.h"
#include "../wire/types.h"

#include <format>
#include <fstream>
#include <print>

namespace convert {

void mis_woofers(const std::filesystem::path& mis_folder,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "sounds", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write sounds");
		return;
	}

	const uint32_t count = readFileUint32(data, 0);
	const woofers* src = reinterpret_cast<const woofers*>(data.data() + sizeof(count));

	for (uint32_t i = 0; i < count; ++i, ++src)
	{
		f << std::format("Name=\"{}\"\nU={}\nV={}\nRadius={}\nWorse={}\nMinWait={}\nMaxWait={}\n\n"
			, src->name
			, src->pos.u
			, src->pos.v
			, src->radius
			, src->worse
			, src->minWait
			, src->maxWait);
	}
}

} // namespace convert
