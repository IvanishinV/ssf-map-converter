#include "mis_phrases.h"
#include "../Helper.h"

#include <fstream>
#include <print>
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

	std::vector<uint8_t> phrases(32000, 0);
	size_t phrasesOffset = 0;
	uint32_t accumulator = 0;
	uint32_t sizeline;
	while (accumulator < size_mis_phrases && phrasesOffset < 32000)
	{
		sizeline = readFileInt8(data, 1 + accumulator);
		position(data, phrases, 2 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(data, 2 + accumulator);
		position(data, phrases, 3 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(data, 3 + accumulator);
		position(data, phrases, 4 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(data, 4 + accumulator);
		position(data, phrases, 5 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;

		sizeline = readFileInt8(data, 5 + accumulator);
		position(data, phrases, 6 + accumulator, phrasesOffset, sizeline);
		phrasesOffset += 64;
		accumulator += sizeline;
		accumulator += 6;
	}

	f.write(reinterpret_cast<const char*>(phrases.data()),
	        static_cast<std::streamsize>(phrases.size()));
}

} // namespace convert
