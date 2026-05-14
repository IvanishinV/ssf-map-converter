#include "mis_tree.h"

#include <fstream>
#include <print>

namespace convert {

void mis_tree(const std::filesystem::path& mis_folder,
              std::string_view data)
{
	std::ofstream f(mis_folder / "misdesc", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write misdesc");
		return;
	}

	f.write(data.data(), static_cast<std::streamsize>(data.size()));
}

} // namespace convert
