#include "mis_groups.h"
#include "../io/wire_reader.h"
#include "../util.h"

#include <array>
#include <cstdint>
#include <format>
#include <fstream>
#include <print>
#include <span>
#include <string_view>

namespace convert {

namespace {

#pragma pack(push, 1)
struct GroupRecord
{
	uint8_t  aitype[4];
	uint8_t  zone1, zone2, group1, group2, flag, zone;
	uint8_t  atime[4];
	uint8_t  delay[4];
	uint16_t min;
	uint8_t  force[4];
	uint8_t  hp, ammo, expa;
};
#pragma pack(pop)
static_assert(sizeof(GroupRecord) == 27);

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

	WireReader r{std::as_bytes(std::span{data})};

	for (size_t idx = 0; r.remaining() >= sizeof(GroupRecord); ++idx)
	{
		const auto rec = r.read<GroupRecord>();

		const uint8_t aitype1 = rec.aitype[0];
		const uint8_t aitype2 = rec.aitype[1];
		const uint8_t aitype3 = rec.aitype[2];
		const uint8_t aitype4 = rec.aitype[3];

		const uint8_t AI_type = static_cast<uint8_t>(aitype1 & 0x0F);
		f << std::format("Group {}\n ai type={} group1={} group2={} zone1={} zone2={}\n  aiflags="
			, idx
			, AI_type_array[AI_type]
			, (uint16_t)rec.group1
			, (uint16_t)rec.group2
			, (uint16_t)rec.zone1
			, (uint16_t)rec.zone2);

		if (AI_type != 0 && AI_type != 1)
			ai_flags_convert(f, aitype1, aitype2, aitype3, aitype4);

		f << '\n';

		const bool delay_inf  = rec.delay[0] == 255 && rec.delay[1] == 255 && rec.delay[2] == 255 && rec.delay[3] == 127;
		const bool force_inf  = rec.force[0] == 255 && rec.force[1] == 255 && rec.force[2] == 255 && rec.force[3] == 127;
		const bool delay_timed = rec.delay[2] >= 1 && rec.delay[2] <= 13;
		const bool force_timed = rec.force[2] >= 1 && rec.force[2] <= 13;
		const bool min_set    = rec.min >= 1 && rec.min <= 98;

		auto pack32 = [](uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) -> uint32_t {
			return ((b4 & 0xFF) << 24) | ((b3 & 0xFF) << 16) | ((b2 & 0xFF) << 8) | (b1 & 0xFF);
		};

		if (rec.atime[0] == 0 && delay_inf && rec.min == 0 && force_inf)
		{
			f << " reserv auto=0 delay=0 min=0 force=0 atime=0";
		}
		else if (rec.atime[0] > 0 && delay_inf && rec.min == 0 && force_inf)
		{
			f << std::format(" reserv auto=1 delay=0 min={} force=0 atime={}",
			    rec.min, pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else if (rec.atime[0] > 0 && delay_timed && rec.min == 0 && force_inf)
		{
			f << std::format(" reserv auto=3 delay={} min={} force=0 atime={}",
			    pack32(rec.delay[0], rec.delay[1], rec.delay[2], rec.delay[3]) / timeconvertnum,
			    rec.min,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else if (rec.atime[0] > 0 && delay_timed && min_set && force_inf)
		{
			f << std::format(" reserv auto=7 delay={} min={} force=0 atime={}",
			    pack32(rec.delay[0], rec.delay[1], rec.delay[2], rec.delay[3]) / timeconvertnum,
			    rec.min,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else if (rec.atime[0] > 0 && delay_timed && rec.min == 0 && force_timed)
		{
			f << std::format(" reserv auto=11 delay={} min={} force={} atime={}",
			    pack32(rec.delay[0], rec.delay[1], rec.delay[2], rec.delay[3]) / timeconvertnum,
			    rec.min,
			    pack32(rec.force[0], rec.force[1], rec.force[2], rec.force[3]) / timeconvertnum,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else if (rec.atime[0] > 0 && delay_inf && rec.min == 0 && force_timed)
		{
			f << std::format(" reserv auto=9 delay=0 min=0 force={} atime={}",
			    pack32(rec.force[0], rec.force[1], rec.force[2], rec.force[3]) / timeconvertnum,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else if (rec.atime[0] > 0 && delay_inf && min_set && force_timed)
		{
			f << std::format(" reserv auto=13 delay=0 min={} force={} atime={}",
			    rec.min,
			    pack32(rec.force[0], rec.force[1], rec.force[2], rec.force[3]) / timeconvertnum,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}
		else
		{
			f << std::format(" reserv auto=15 delay={} min={} force={} atime={}",
			    pack32(rec.delay[0], rec.delay[1], rec.delay[2], rec.delay[3]) / timeconvertnum,
			    rec.min,
			    pack32(rec.force[0], rec.force[1], rec.force[2], rec.force[3]) / timeconvertnum,
			    pack32(rec.atime[0], rec.atime[1], rec.atime[2], rec.atime[3]) / timeconvertnum);
		}

		f << std::format(" flag={} zone={} hp={} ammo={} expa={}\n"
			, (uint16_t)rec.flag
			, (uint16_t)rec.zone
			, (uint16_t)rec.hp
			, (uint16_t)rec.ammo
			, (uint16_t)rec.expa);
	}
}

} // namespace convert
