#include "map_cflags.h"
#include "../util.h"

#include <fstream>
#include <print>
#include <vector>

namespace convert {

void map_cflags(const std::filesystem::path& map_folder,
                std::string_view data)
{
	std::ofstream f(map_folder / "cflags", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write cflags");
		return;
	}

	const size_t length = data.size();
	std::vector<uint8_t> mapFlags(length / 2, 0);

	for (size_t offset = 0, vOffset = 0; offset < length; offset += 4, vOffset += 2)
	{
		const uint8_t num1 = static_cast<uint8_t>(data[offset]);
		const uint8_t num2 = static_cast<uint8_t>(data[offset + 1]);
		const uint8_t num3 = static_cast<uint8_t>(data[offset + 2]);

		const uint8_t zoneBlocker = ((num1 & 0xF0) > 0 ? GLOBALNULL : (num1 & 0x0F));
		const uint8_t waves       = static_cast<uint8_t>(~num2 >> 1);
		const uint8_t noPantones  = ((num2 & 0x0F) == 6 ? ((num3 & 0x0F) > 0 ? 0 : 1) : 0);

		mapFlags[vOffset] = static_cast<uint8_t>(noPantones << 7 | (waves & 0x40) | (num2 & 0x30) | zoneBlocker);
	}

	f.write(reinterpret_cast<const char*>(mapFlags.data()), static_cast<std::streamsize>(mapFlags.size()));
}

} // namespace convert
