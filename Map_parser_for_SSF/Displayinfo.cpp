
#include "stdafx.h"

#include "Displayinfo.h"

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	switch (mapIdentifier)
	{
	case (0):
		own::print(Dictionary::getValue(STRINGS::MAP_SUMMER));
		//std::print("Это летняя карта");
		break;
	case (1):
		own::print(Dictionary::getValue(STRINGS::MAP_WINTER));
		//std::print("Это зимняя карта");
		break;
	case (2):
		own::print(Dictionary::getValue(STRINGS::MAP_SEA));
		//std::print("Это морская карта");
		break;
	case (3):
		own::print(Dictionary::getValue(STRINGS::MAP_DESERT));
		//std::print("Это пустынная карта");
		break;
	case (0x28):
		own::print(Dictionary::getValue(STRINGS::MAP_MISSION));
		//std::print("Это миссия");
		break;
	default:
		own::print(Dictionary::getValue(STRINGS::MAP_UNKNOWN));
		//std::print("Неизвестный тип карты");
		break;
	};


	own::println(Dictionary::getValue(STRINGS::MAP_SIZE), mapSizeU, mapSizeV, mapEndPosition);
	//std::println(" размером: {} x {}\nРазмер файла: {} байт", mapSizeU, mapSizeV, mapEndPosition);
}

void errorExistsFile(const std::string_view& path)
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_FILE_NOT_EXIST), path);
}

void errorOpenFile(const std::string_view& path)
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_OPEN), path);
	//std::println("\033[31m[Error]\033[0m Ошибка открытия файла!");
}

void errorWriteFile()
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_WRITE));
	//std::println("\033[31m[Error]\033[0m Ошибка создания файла для записи!");
}