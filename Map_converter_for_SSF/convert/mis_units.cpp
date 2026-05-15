#include "mis_units.h"
#include "../Displayinfo.h"
#include "../io/wire_reader.h"
#include "../Displayinfo.h"
#include "../wire/types.h"

#include <format>
#include <fstream>
#include <span>
#include <vector>

namespace convert {

void mis_units(const std::filesystem::path& mis_folder,
               std::string_view mis_unitnames,
               std::string_view mis_mapunits,
               std::string_view mis_support)
{
	std::ofstream outputFileMapUnits(mis_folder / "mapunits", std::ios::binary);
	std::ofstream outputFileSupport(mis_folder / "support", std::ios::binary);
	if (!outputFileMapUnits || !outputFileSupport)
	{
		errorWriteFile("mapunits/support");
		return;
	}

	std::vector<std::string_view> nameunit;
	nameunit.reserve(mis_unitnames.size() / 16);
	int32_t numOfNames = 0;
	while (numOfNames * 16 < static_cast<int32_t>(mis_unitnames.size()))
	{
		nameunit.push_back(mis_unitnames.data() + numOfNames * 16);
		++numOfNames;
	}

	// mapunits file will have a reversed strings comparing to the original editor mapunits file
	WireReader r_mapunits{std::as_bytes(std::span{mis_mapunits})};
	r_mapunits.skip(sizeof(uint32_t)); // numberofunit
	while (r_mapunits.remaining() >= sizeof(unitsMap))
	{
		const auto misMapUnit = r_mapunits.read<unitsMap>();

		outputFileMapUnits << std::format("ID={} GRP={} HP={} Ammo={} Expa={} Lives={} U={} V={} Dir={} In={} Owner={}\n"
			, nameunit[misMapUnit.unit.unit.ID]
			, (uint16_t)misMapUnit.unit.unit.grp
			, (uint16_t)misMapUnit.unit.unit.hp
			, (uint16_t)misMapUnit.unit.unit.ammo
			, (uint16_t)misMapUnit.unit.unit.expa
			, (uint16_t)misMapUnit.unit.unit.lives
			, (uint16_t)misMapUnit.pos.u
			, (uint16_t)misMapUnit.pos.v
			, (uint16_t)misMapUnit.dir
			, (uint16_t)misMapUnit.unit.in
			, (uint16_t)misMapUnit.owner);

		for (uint32_t n = 0; n < misMapUnit.unit.in; ++n)
		{
			const auto passenger = r_mapunits.read<unitBase>();
			outputFileMapUnits << std::format(" ID={} GRP={} HP={} Ammo={} Expa={} Lives={}\n"
				, nameunit[passenger.ID]
				, (uint16_t)passenger.grp
				, (uint16_t)passenger.hp
				, (uint16_t)passenger.ammo
				, (uint16_t)passenger.expa
				, (uint16_t)passenger.lives);
		}
	}

	WireReader r_support{std::as_bytes(std::span{mis_support})};
	r_support.skip(sizeof(uint32_t)); // supportSize

	for (uint32_t i = 0; i < VALUEFLAG; ++i)
	{
		const auto buf = r_support.read<flag>();
		outputFileSupport << std::format("flag {},{},{},{},{}\n"
			, buf.redFlag.u
			, buf.redFlag.v
			, buf.valueRedFlag
			, buf.blueFlag.u
			, buf.blueFlag.v);
	}
	outputFileSupport << '\n';

	for (uint32_t i = 0; i < VALUESUPPORT; ++i)
	{
		const uint8_t NameSize = r_support.read<uint8_t>();
		if (NameSize > 0)
		{
			const auto name_span = r_support.read_span(NameSize);
			outputFileSupport << "support \"";
			outputFileSupport.write(reinterpret_cast<const char*>(name_span.data()), NameSize);
			outputFileSupport << "\"\n";
		}
		else
		{
			outputFileSupport << "support \"\"\n";
		}

		const uint32_t UnitScriptSize = r_support.read<uint32_t>();
		for (uint32_t k = 0; k < UnitScriptSize; ++k)
		{
			const auto unit = r_support.read<unitBaseIn>();
			outputFileSupport << std::format(" ID={} Grp={} HP={} Ammo={} Expa={} Lives={} In={}\n"
				, nameunit[unit.unit.ID]
				, (uint16_t)unit.unit.grp
				, (uint16_t)unit.unit.hp
				, (uint16_t)unit.unit.ammo
				, (uint16_t)unit.unit.expa
				, (uint16_t)unit.unit.lives
				, (uint16_t)unit.in);

			for (uint32_t n = 0; n < unit.in; ++n)
			{
				const auto passenger = r_support.read<unitBase>();
				outputFileSupport << std::format("  ID={} Grp={} HP={} Ammo={} Expa={} Lives={}\n"
					, nameunit[passenger.ID]
					, (uint16_t)passenger.grp
					, (uint16_t)passenger.hp
					, (uint16_t)passenger.ammo
					, (uint16_t)passenger.expa
					, (uint16_t)passenger.lives);
			}
		}
		outputFileSupport << "end\n";
	}
}

} // namespace convert
