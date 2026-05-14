#pragma once

#include <cstdint>

constexpr uint32_t size_arr16 = 16;
constexpr uint32_t size_arr32 = 32;
constexpr uint32_t size_arr64 = 64;

constexpr uint32_t VALUEFLAG          = 16;  // flags per support
constexpr uint32_t VALUESUPPORT       = 64;  // scripts per support
constexpr uint32_t VALUEREINFORCEMENT = 4;   // reinforcements per player
constexpr uint32_t VALUEDESCENT       = 2;   // descent groups per player

using pixel = uint16_t;

struct coordinates16
{
	uint16_t u;
	uint16_t v;
};

struct coordinates32
{
	int u;
	int v;
};

struct flag
{
	coordinates32		redFlag;
	coordinates32		blueFlag;
	int					valueRedFlag;
};

struct unitBase //Entity, Passenger
{
	uint8_t grp;
	uint8_t hp;
	uint8_t ammo;
	uint8_t expa;
	uint8_t lives;
	uint8_t ID;
};

struct unitBaseIn //Vehicles, support
{
	unitBase unit;
	uint8_t in;
};

#pragma pack(push, 1)
/**
 * @param unit - parameters entity or vehicles
 * @param pos - unit position on the map.
 * @param dir - unit rotation, clockwise from 0 to 31, 32 positions in total.
 * @param owner - player=0, enemy=1, ally=2, neutral=3.
 */
struct unitsMap
{
	unitBaseIn unit;
	coordinates16 pos;
	uint8_t dir;
	uint8_t owner;
};
#pragma pack (pop)

struct aviation
{
	char name[size_arr32];
	uint32_t number;
	uint32_t bombs;
	uint32_t reloads;
};

#pragma pack(push, 1)
struct descent
{
	uint8_t group;
	uint8_t expa;
	char ID[4][size_arr16];
	uint8_t number[4];
};
#pragma pack (pop)

#pragma pack(push, 1)
/**
 * @param planesdir - plane rotation, clockwise from 0 to 7, 8 positions in total.
 */
struct players
{
	char name[size_arr32];
	uint8_t team;
	pixel	color;
	uint8_t nation;
	uint8_t planesdir;

	aviation airReinforcement[VALUEREINFORCEMENT];

	descent group[VALUEDESCENT];
};
#pragma pack (pop)

#pragma pack(push, 1)
struct woofers
{
	char name[size_arr64];
	coordinates16 pos;
	uint16_t radius;
	float worse;
	uint16_t minWait;
	uint16_t maxWait;
};
#pragma pack (pop)

static_assert(sizeof(coordinates16) == 4);
static_assert(sizeof(coordinates32) == 8);
static_assert(sizeof(flag) == 20);
static_assert(sizeof(unitBase) == 6);
static_assert(sizeof(unitBaseIn) == 7);
static_assert(sizeof(unitsMap) == 13);
static_assert(sizeof(aviation) == 44);
static_assert(sizeof(descent) == 70);
static_assert(sizeof(players) == 353);
static_assert(sizeof(woofers) == 78);
