#include <iostream> 

#include "Displayinfo.h"

void displayinfo(uint32_t  mapSizeU, uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition)
{
	if (mapIdentifier == 0)
	{
		//std::cout << "This is Summer map size: ";
		std::cout << "��� ������ ����� ��������: ";
	}
	else
	if (mapIdentifier == 1)
	{
		//std::cout << "This is Winter map size: ";
		std::cout << "��� ������ ����� ��������: ";
	}
	else
	if (mapIdentifier == 2)
	{
		//std::cout << "This is Beach map size: ";
		std::cout << "��� ������� ����� ��������: ";
	} 
	else
	if (mapIdentifier == 3)
	{
		//std::cout << "This is Desert map size: ";
		std::cout << "��� ��������� ����� ��������: ";
	}
	else
	if (mapIdentifier == 40)
	{

	}
	std::cout << mapSizeU << " x " << mapSizeV << "\n" << std::endl;
	//std::cout << "Control file size: " << mapEndPosition << " byte\n" << std::endl;
	std::cout << "����������� ������ �����: " << mapEndPosition << " byte\n" << std::endl;
	//std::cout << "Press Enter to continue..." << std::endl;
	std::cout << "������� Enter ��� ����������..." << std::endl;
	return;
}

void eroropenfile()
{
	std::cout << "������ �������� �����!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}

void erorbuildfile()
{
	std::cout << "������ �������� ����� ��� ������!" << std::endl;
	//std::cout << "File opening error!" << std::endl;
}