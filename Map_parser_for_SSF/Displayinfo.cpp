
#include "stdafx.h"

#include "Displayinfo.h"

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	switch (mapIdentifier)
	{
	case (0):
		own::print(Dictionary::getValue(STRINGS::MAP_SUMMER));
		//std::print("��� ������ �����");
		break;
	case (1):
		own::print(Dictionary::getValue(STRINGS::MAP_WINTER));
		//std::print("��� ������ �����");
		break;
	case (2):
		own::print(Dictionary::getValue(STRINGS::MAP_SEA));
		//std::print("��� ������� �����");
		break;
	case (3):
		own::print(Dictionary::getValue(STRINGS::MAP_DESERT));
		//std::print("��� ��������� �����");
		break;
	default:
		own::print(Dictionary::getValue(STRINGS::MAP_UNKNOWN));
		//std::print("����������� ��� �����");
		break;
	};


	own::println(Dictionary::getValue(STRINGS::MAP_SIZE), mapSizeU, mapSizeV, mapEndPosition);
	//std::println(" ��������: {} x {}\n������ �����: {} ����", mapSizeU, mapSizeV, mapEndPosition);
}

void errorExistsFile()
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_FILE_NOT_EXIST));
}

void errorOpenFile()
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_OPEN));
	//std::println("\033[31m[Error]\033[0m ������ �������� �����!");
}

void errorWriteFile()
{
	own::printlnError(Dictionary::getValue(STRINGS::ERROR_WRITE));
	//std::println("\033[31m[Error]\033[0m ������ �������� ����� ��� ������!");
}