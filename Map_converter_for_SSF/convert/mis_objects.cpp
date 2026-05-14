#include "mis_objects.h"
#include "../wire/types.h"

#include <fstream>
#include <print>
#include <vector>

namespace convert {

void mis_objects(const std::filesystem::path& mis_folder,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "objs", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write objs");
		return;
	}

	const coordinates16* src = reinterpret_cast<const coordinates16*>(data.data());
	const size_t count = data.size() / sizeof(coordinates16);

	std::vector<coordinates32> dst(count);
	for (size_t i = 0; i < count; ++i)
	{
		dst[i].u = src[i].u;
		dst[i].v = src[i].v;
	}

	f.write(reinterpret_cast<const char*>(dst.data()),
	        static_cast<std::streamsize>(count * sizeof(coordinates32)));
}

} // namespace convert
