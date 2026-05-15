
#include "Displayinfo.h"
#include "Lang.h"

#include <print>

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	switch (mapIdentifier)
	{
	case (0):
		own::print(Dictionary::getValue(STRINGS::MAP_SUMMER));
		break;
	case (1):
		own::print(Dictionary::getValue(STRINGS::MAP_WINTER));
		break;
	case (2):
		own::print(Dictionary::getValue(STRINGS::MAP_SEA));
		break;
	case (3):
		own::print(Dictionary::getValue(STRINGS::MAP_DESERT));
		break;
	case (0x28):
		own::print(Dictionary::getValue(STRINGS::MAP_MISSION));
		break;
	default:
		own::print(Dictionary::getValue(STRINGS::MAP_UNKNOWN));
		break;
	};

	(void)mapEndPosition;
	own::println(Dictionary::getValue(STRINGS::MAP_SIZE), mapSizeU, mapSizeV);
}

void errorExistsFile(const std::string_view& path)
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_FILE_NOT_EXIST), path);
}

void errorOpenFile(const std::string_view& path)
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_OPEN), path);
}

void errorWriteFile(std::string_view what)
{
	std::println(stderr, "\033[31m[Error]\033[0m {} ({})",
	             Dictionary::getValue(STRINGS::ERROR_WRITE), what);
}