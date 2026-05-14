#include "mis_groups.h"
#include "../util.h"

#include <array>
#include <format>
#include <fstream>
#include <print>
#include <string_view>

namespace convert {

namespace {

void ai_flags_convert(std::ofstream& f,
                      uint8_t aitype1, uint8_t aitype2,
                      uint8_t aitype3, uint8_t aitype4)
{
	const uint32_t AIflags = ((aitype4 & 0xFF) << 24) | ((aitype3 & 0xFF) << 16) | ((aitype2 & 0xFF) << 8) | (aitype1 & 0xFF);

	const uint8_t t1 = static_cast<uint8_t>((AIflags & 0xF0) >> 4);
	if (t1 & 0x1) f << " aif_rndmove";
	if (t1 & 0x2) f << " aif_zonehaveown";
	if (t1 & 0x4) f << " aif_zonenoemeny";
	if (t1 & 0x8) f << " aif_useobjects";

	const uint8_t t2 = static_cast<uint8_t>((AIflags >> 8) & 0xFF);
	if (t2 & 0x01) f << " aif_rndtarget";
	if (t2 & 0x02) f << " aif_grestrict";
	if (t2 & 0x04) f << " aif_zrestrict";
	if (t2 & 0x08) f << " aif_atgnouse";
	if (t2 & 0x10) f << " aif_atgzonehaveown";
	if (t2 & 0x20) f << " aif_atgzonenoenemy";
	if (t2 & 0x40) f << " aif_atgnomove";
	if (t2 & 0x80) f << " aif_atgnotow";

	const uint8_t t3 = static_cast<uint8_t>((AIflags >> 16) & 0xFF);
	if (t3 & 0x01) f << " aif_atgnodrop";
	if (t3 & 0x02) f << " aif_gaubnouse";
	if (t3 & 0x04) f << " aif_gaubzonehaveown";
	if (t3 & 0x08) f << " aif_gaubzonenoenemy";
	if (t3 & 0x10) f << " aif_gaubnomove";
	if (t3 & 0x20) f << " aif_gaubnotow";
	if (t3 & 0x40) f << " aif_gaubnodrop";
	if (t3 & 0x80) f << " aif_domnouse";

	const uint8_t t4 = static_cast<uint8_t>((AIflags >> 24) & 0x0F);
	if (t4 & 0x1) f << " aif_domambush";
	if (t4 & 0x2) f << " aif_domhide";
	if (t4 & 0x4) f << " aif_holdfire";
}

} // namespace

void mis_groups(const std::filesystem::path& mis_folder,
                std::string_view data)
{
	std::ofstream f(mis_folder / "groups", std::ios::binary);
	if (!f)
	{
		std::println(stderr, "\033[31m[Error]\033[0m cannot write groups");
		return;
	}

	constexpr std::array<std::string_view, 16> AI_type_array =
	{
		"\"ai_none\"",
		"\"ai_recon\"",
		"\"ai_sold_guard\"",
		"\"ai_sold_follow\"",
		"\"ai_sold_art\"",
		"\"ai_tank_guard\"",
		"\"ai_tank_follow\"",
		"\"ai_furg_help\"",
		"\"ai_furg_move\"",
		"\"ai_gruz_reload\"",
		"\"ai_katya_move\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\"",
		"\"ai_none\""
	};

	const size_t maxTries = data.size() / 27;
	for (size_t accumulator = 0; accumulator < maxTries; ++accumulator)
	{
		const uint8_t* buffer = reinterpret_cast<const uint8_t*>(data.data()) + accumulator * 27;
		const uint8_t aitype1 = *(buffer + 0);
		const uint8_t aitype2 = *(buffer + 1);
		const uint8_t aitype3 = *(buffer + 2);
		const uint8_t aitype4 = *(buffer + 3);

		const uint8_t zone1  = *(buffer + 4);
		const uint8_t zone2  = *(buffer + 5);
		const uint8_t group1 = *(buffer + 6);
		const uint8_t group2 = *(buffer + 7);
		const uint8_t flag   = *(buffer + 8);
		const uint8_t zone   = *(buffer + 9);

		const uint8_t atime1 = *(buffer + 10);
		const uint8_t atime2 = *(buffer + 11);
		const uint8_t atime3 = *(buffer + 12);
		const uint8_t atime4 = *(buffer + 13);

		const uint8_t  delay1 = *(buffer + 14);
		const uint8_t  delay2 = *(buffer + 15);
		const uint8_t  delay3 = *(buffer + 16);
		const uint8_t  delay4 = *(buffer + 17);
		const uint16_t min    = *(uint16_t*)(buffer + 18);

		const uint8_t force1 = *(buffer + 20);
		const uint8_t force2 = *(buffer + 21);
		const uint8_t force3 = *(buffer + 22);
		const uint8_t force4 = *(buffer + 23);

		const uint8_t hp   = *(buffer + 24);
		const uint8_t ammo = *(buffer + 25);
		const uint8_t expa = *(buffer + 26);

		const uint8_t AI_type = static_cast<uint8_t>(aitype1 & 0x0F);
		f << std::format("Group {}\n ai type={} group1={} group2={} zone1={} zone2={}\n  aiflags="
			, accumulator
			, AI_type_array[AI_type]
			, (uint16_t)group1
			, (uint16_t)group2
			, (uint16_t)zone1
			, (uint16_t)zone2);

		if (AI_type != 0 && AI_type != 1)
			ai_flags_convert(f, aitype1, aitype2, aitype3, aitype4);

		f << '\n';

		const bool delay_inf  = delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127;
		const bool force_inf  = force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127;
		const bool delay_timed = delay3 >= 1 && delay3 <= 13;
		const bool force_timed = force3 >= 1 && force3 <= 13;
		const bool min_set    = min >= 1 && min <= 98;

		auto pack32 = [](uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) -> uint32_t {
			return ((b4 & 0xFF) << 24) | ((b3 & 0xFF) << 16) | ((b2 & 0xFF) << 8) | (b1 & 0xFF);
		};

		if (atime1 == 0 && delay_inf && min == 0 && force_inf)
		{
			f << " reserv auto=0 delay=0 min=0 force=0 atime=0";
		}
		else if (atime1 > 0 && delay_inf && min == 0 && force_inf)
		{
			f << std::format(" reserv auto=1 delay=0 min={} force=0 atime={}",
			    min, pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else if (atime1 > 0 && delay_timed && min == 0 && force_inf)
		{
			f << std::format(" reserv auto=3 delay={} min={} force=0 atime={}",
			    pack32(delay1, delay2, delay3, delay4) / timeconvertnum,
			    min,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else if (atime1 > 0 && delay_timed && min_set && force_inf)
		{
			f << std::format(" reserv auto=7 delay={} min={} force=0 atime={}",
			    pack32(delay1, delay2, delay3, delay4) / timeconvertnum,
			    min,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else if (atime1 > 0 && delay_timed && min == 0 && force_timed)
		{
			f << std::format(" reserv auto=11 delay={} min={} force={} atime={}",
			    pack32(delay1, delay2, delay3, delay4) / timeconvertnum,
			    min,
			    pack32(force1, force2, force3, force4) / timeconvertnum,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else if (atime1 > 0 && delay_inf && min == 0 && force_timed)
		{
			f << std::format(" reserv auto=9 delay=0 min=0 force={} atime={}",
			    pack32(force1, force2, force3, force4) / timeconvertnum,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else if (atime1 > 0 && delay_inf && min_set && force_timed)
		{
			f << std::format(" reserv auto=13 delay=0 min={} force={} atime={}",
			    min,
			    pack32(force1, force2, force3, force4) / timeconvertnum,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}
		else
		{
			f << std::format(" reserv auto=15 delay={} min={} force={} atime={}",
			    pack32(delay1, delay2, delay3, delay4) / timeconvertnum,
			    min,
			    pack32(force1, force2, force3, force4) / timeconvertnum,
			    pack32(atime1, atime2, atime3, atime4) / timeconvertnum);
		}

		f << std::format(" flag={} zone={} hp={} ammo={} expa={}\n"
			, (uint16_t)flag
			, (uint16_t)zone
			, (uint16_t)hp
			, (uint16_t)ammo
			, (uint16_t)expa);
	}
}

} // namespace convert
