
#include "stdafx.h"

#include "Displayinfo.h"

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	switch (mapIdentifier)
	{
	case (0):
		std::print("Это летняя карта");
		break;
	case (1):
		std::print("Это зимняя карта");
		break;
	case (2):
		std::print("Это морская карта");
		break;
	case (3):
		std::print("Это пустынная карта");
		break;
	default:
		std::print("Неизвестный тип карты");
		break;
	};

	std::println(" размером: {} x {}\nРазмер файла: {} байт", mapSizeU, mapSizeV, mapEndPosition);
}

void errorOpenFile()
{
	std::println("Ошибка открытия файла!");
	//std::cout << "Ошибка открытия файла!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}

void errorBuildFile()
{
	std::println("Ошибка создания файла для записи!");
	//std::cout << "Ошибка создания файла для записи!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}