#include "mis_units.h"
#include "../wire/types.h"

#include <format>
#include <fstream>
#include <print>
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
		std::println(stderr, "\033[31m[Error]\033[0m cannot write mapunits/support");
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

	uint32_t numberofunit = *(uint32_t*)(mis_mapunits.data());
	const uint8_t* dataPtr = reinterpret_cast<const uint8_t*>(mis_mapunits.data());
	// mapunits file will have a reversed strings comparing to the original editor mapunits file
	size_t curOffset = sizeof(numberofunit);
	while (curOffset + sizeof(unitsMap) <= mis_mapunits.size())
	{
		const unitsMap* misMapUnit = reinterpret_cast<const unitsMap*>(dataPtr + curOffset);
		curOffset += sizeof(unitsMap);

		outputFileMapUnits << std::format("ID={} GRP={} HP={} Ammo={} Expa={} Lives={} U={} V={} Dir={} In={} Owner={}\n"
			, nameunit[misMapUnit->unit.unit.ID]
			, (uint16_t)misMapUnit->unit.unit.grp
			, (uint16_t)misMapUnit->unit.unit.hp
			, (uint16_t)misMapUnit->unit.unit.ammo
			, (uint16_t)misMapUnit->unit.unit.expa
			, (uint16_t)misMapUnit->unit.unit.lives
			, (uint16_t)misMapUnit->pos.u
			, (uint16_t)misMapUnit->pos.v
			, (uint16_t)misMapUnit->dir
			, (uint16_t)misMapUnit->unit.in
			, (uint16_t)misMapUnit->owner);

		if (misMapUnit->unit.in > 0) // Passenger
		{
			for (uint32_t n = 0; n < misMapUnit->unit.in; ++n)
			{
				const unitBase* misMapUnitPassenger = reinterpret_cast<const unitBase*>(dataPtr + curOffset);
				curOffset += sizeof(unitBase);
				outputFileMapUnits << std::format(" ID={} GRP={} HP={} Ammo={} Expa={} Lives={}\n"
					, nameunit[misMapUnitPassenger->ID]
					, (uint16_t)misMapUnitPassenger->grp
					, (uint16_t)misMapUnitPassenger->hp
					, (uint16_t)misMapUnitPassenger->ammo
					, (uint16_t)misMapUnitPassenger->expa
					, (uint16_t)misMapUnitPassenger->lives);
			}
		}
	}

	const uint32_t supportSize = *(uint32_t*)(mis_support.data());	// not used
	dataPtr = reinterpret_cast<const uint8_t*>(mis_support.data());

	curOffset = sizeof(supportSize);
	for (uint32_t i = 0; i < VALUEFLAG; ++i)
	{
		const flag* buffer = reinterpret_cast<const flag*>(dataPtr + curOffset);
		curOffset += sizeof(flag);

		outputFileSupport << std::format("flag {},{},{},{},{}\n"
			, buffer->redFlag.u
			, buffer->redFlag.v
			, buffer->valueRedFlag
			, buffer->blueFlag.u
			, buffer->blueFlag.v);
	}
	outputFileSupport << '\n';

	uint32_t accumulator = 0;
	for (uint32_t i = 0; i < VALUESUPPORT; ++i)
	{
		const uint8_t NameSize = mis_support[sizeof(flag) * VALUEFLAG + sizeof(supportSize) + accumulator];

		if (NameSize > 0)
		{
			const uint8_t* buffername = reinterpret_cast<const uint8_t*>(mis_support.data()) + sizeof(flag) * VALUEFLAG + sizeof(supportSize) + sizeof(NameSize) + accumulator;
			outputFileSupport << "support \"";
			outputFileSupport.write((const char*)buffername, NameSize);
			outputFileSupport << "\"\n";
		}
		else
		{
			outputFileSupport << "support \"\"\n";
		}

		uint32_t UnitScriptSize = *(uint32_t*)(mis_support.data() + sizeof(flag) * VALUEFLAG + sizeof(supportSize) + sizeof(NameSize) + NameSize + accumulator);
		curOffset = sizeof(flag) * VALUEFLAG + sizeof(supportSize) + sizeof(NameSize) + NameSize + sizeof(UnitScriptSize) + accumulator;
		accumulator += sizeof(NameSize) + NameSize + sizeof(UnitScriptSize);
		if (UnitScriptSize > 0)
		{
			for (uint32_t k = 0; k < UnitScriptSize; ++k)
			{
				dataPtr = reinterpret_cast<const uint8_t*>(mis_support.data()) + curOffset;
				const unitBaseIn* misSupportUnit = reinterpret_cast<const unitBaseIn*>(dataPtr);

				curOffset += sizeof(unitBaseIn);
				accumulator += sizeof(unitBaseIn);
				outputFileSupport << std::format(" ID={} Grp={} HP={} Ammo={} Expa={} Lives={} In={}\n"
					, nameunit[misSupportUnit->unit.ID]
					, (uint16_t)misSupportUnit->unit.grp
					, (uint16_t)misSupportUnit->unit.hp
					, (uint16_t)misSupportUnit->unit.ammo
					, (uint16_t)misSupportUnit->unit.expa
					, (uint16_t)misSupportUnit->unit.lives
					, (uint16_t)misSupportUnit->in);

				if (misSupportUnit->in > 0) // Passenger
				{
					for (uint32_t n = 0; n < misSupportUnit->in; ++n)
					{
						dataPtr = reinterpret_cast<const uint8_t*>(mis_support.data()) + curOffset;
						const unitBase* misSupportUnitPassenger = reinterpret_cast<const unitBase*>(dataPtr);
						curOffset += sizeof(unitBase);
						accumulator += sizeof(unitBase);

						outputFileSupport << std::format("  ID={} Grp={} HP={} Ammo={} Expa={} Lives={}\n"
							, nameunit[misSupportUnitPassenger->ID]
							, (uint16_t)misSupportUnitPassenger->grp
							, (uint16_t)misSupportUnitPassenger->hp
							, (uint16_t)misSupportUnitPassenger->ammo
							, (uint16_t)misSupportUnitPassenger->expa
							, (uint16_t)misSupportUnitPassenger->lives);
					}
				}
			}
		}
		outputFileSupport << "end\n";
	}
}

} // namespace convert
