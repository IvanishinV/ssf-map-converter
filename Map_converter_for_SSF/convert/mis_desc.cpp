#include "mis_desc.h"

#include <fstream>
#include <print>

namespace convert {

void mis_desc(const std::filesystem::path& mis_folder)
{
	std::ofstream f(mis_folder / "desc", std::ios::binary);
	if (!f)
		std::println(stderr, "\033[31m[Error]\033[0m cannot write mis/desc");
}

} // namespace convert
