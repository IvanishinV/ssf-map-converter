#include "mis_phrases.h"
#include "../io/wire_reader.h"

#include <algorithm>
#include <fstream>
#include <print>
#include <span>
#include <vector>

namespace convert {

void mis_phrases(const std::filesystem::path& mis_folder,
                 std::string_view data,
                 uint32_t size_mis_phrases)
{
	std::ofstream f(mis_folder / "phrases", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write phrases");
		return;
	}

	WireReader r{std::as_bytes(std::span{data})};
	std::vector<uint8_t> phrases(32000, 0);
	size_t phrasesOffset = 0;
	uint32_t accumulator = 0;
	uint32_t sizeline;
	while (accumulator < size_mis_phrases && phrasesOffset < 32000)
	{
		for (uint32_t field = 1; field <= 5; ++field)
		{
			sizeline = r.peek_at<int8_t>(field + accumulator);
			std::copy_n(data.data() + field + 1 + accumulator, sizeline,
			            phrases.data() + phrasesOffset);
			phrasesOffset += 64;
			accumulator += sizeline;
		}
		accumulator += 6;
	}

	f.write(reinterpret_cast<const char*>(phrases.data()),
	        static_cast<std::streamsize>(phrases.size()));
}

} // namespace convert
