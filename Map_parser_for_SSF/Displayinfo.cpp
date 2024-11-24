
#include "stdafx.h"

#include "Displayinfo.h"

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	switch (mapIdentifier)
	{
	case (0):
		std::print("��� ������ �����");
		break;
	case (1):
		std::print("��� ������ �����");
		break;
	case (2):
		std::print("��� ������� �����");
		break;
	case (3):
		std::print("��� ��������� �����");
		break;
	default:
		std::print("����������� ��� �����");
		break;
	};

	std::println(" ��������: {} x {}\n������ �����: {} ����", mapSizeU, mapSizeV, mapEndPosition);
}

void errorOpenFile()
{
	std::println("������ �������� �����!");
	//std::cout << "������ �������� �����!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}

void errorBuildFile()
{
	std::println("������ �������� ����� ��� ������!");
	//std::cout << "������ �������� ����� ��� ������!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}