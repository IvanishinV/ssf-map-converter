#include "mis_players.h"
#include "../io/wire_reader.h"
#include "../util.h"
#include "../wire/types.h"

#include <format>
#include <fstream>
#include <print>
#include <span>

namespace convert {

void mis_players(const std::filesystem::path& mis_folder,
                 std::string_view data)
{
	std::ofstream f(mis_folder / "players", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write players");
		return;
	}

	static constexpr const char* TYPEREINFORCEMENT[] = { "bomb", "spy", "transport", "boxer" };
	WireReader r{std::as_bytes(std::span{data})};

	for (size_t idx = 0; r.remaining() >= sizeof(players); ++idx)
	{
		const auto buffer = r.read<players>();

		const uint16_t red   = (buffer.color >> 11) & 0x1F;
		const uint16_t green = (buffer.color >>  5) & 0x3F;
		const uint16_t blue  =  buffer.color        & 0x1F;
		f << std::format("Player {}\n name=\"{}\"\n team={}\n nation={}\n color={} {} {}\n"
			, idx
			, buffer.name
			, buffer.team
			, buffer.nation
			, red   << static_cast<uint8_t>(RGB565_SHIFT::R)
			, green << static_cast<uint8_t>(RGB565_SHIFT::G)
			, blue  << static_cast<uint8_t>(RGB565_SHIFT::B));

		for (uint32_t i = 0; i < VALUEREINFORCEMENT; ++i)
		{
			f << std::format(" {}\n  ID={}\n  Number={}\n  Bombs={}\n  Reload={}\n"
				, TYPEREINFORCEMENT[i]
				, buffer.airReinforcement[i].name
				, buffer.airReinforcement[i].number
				, buffer.airReinforcement[i].bombs
				, buffer.airReinforcement[i].reloads);
		}

		for (uint32_t k = 0; k < VALUEDESCENT; ++k)
		{
			f << std::format(" descent {}\n  group={}\n  expa={}\n"
				, k
				, buffer.group[k].group
				, buffer.group[k].expa);
			f << std::format("  ID 0={}\n  number 0={}\n  ID 1={}\n  number 1={}\n  ID 2={}\n  number 2={}\n  ID 3={}\n  number 3={}\n"
				, buffer.group[k].ID[0]
				, buffer.group[k].number[0]
				, buffer.group[k].ID[1]
				, buffer.group[k].number[1]
				, buffer.group[k].ID[2]
				, buffer.group[k].number[2]
				, buffer.group[k].ID[3]
				, buffer.group[k].number[3]);
		}
		f << std::format(" planesdir={}\n", buffer.planesdir);
	}
}

} // namespace convert
