#pragma once

using pixel = unsigned int;


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
	uint32_t			valueRedFlag;
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
	char name[32];
	uint32_t number;
	uint32_t bombs;
	uint32_t reloads;
};

#pragma pack(push, 1)
struct descent
{
	uint8_t group;
	uint8_t expa;
	char ID0[16];
	char ID1[16];
	char ID2[16];
	char ID3[16];
	uint8_t number0;
	uint8_t number1;
	uint8_t number2;
	uint8_t number3;
};
#pragma pack (pop)

#pragma pack(push, 1)
struct players
{
	char name[32];
	uint8_t team;
	pixel	color;
	uint8_t nation;
	uint8_t planesdir;

	aviation airReinforcementBomb;
	aviation airReinforcementSpy;
	aviation airReinforcementTransport;
	aviation airReinforcementBoxer;

	descent group1;
	descent group2;
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