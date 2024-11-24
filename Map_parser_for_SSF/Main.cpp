#include <iostream> 
#include <fstream>
#include <vector> 
#include <sstream>
#include <Windows.h>

#include "Main.h"
#include "Displayinfo.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

//------------------------------------------------------------------------------
void openFileMapForPasser()
{
	std::string rawData;
	//std::cout << "Specify the file name or path to it: ";
	std::cout << "Введите название файла или путь к нему (не забываем про расширение *.ssm *.smm *.ssc): ";
	std::getline(std::cin >> std::ws, rawData);
	std::ifstream inputFile(rawData, std::ios::ate | std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		eroropenfile();
		return;
	}
	std::vector<char> mapData(inputFile.tellg());

	inputFile.seekg(std::ios::beg);
	inputFile.read(mapData.data(), mapData.size());
	inputFile.close();

	std::string zipData = gzip::decompress(mapData.data(), mapData.size());
	std::stringstream zipData2;
	zipData2.write(zipData.data(), zipData.size());


	char mapType[5];
	zipData2.read(mapType, sizeof(mapType));
	mapType[4] = '\0';

	if (memcmp(mapType, "SSSM", 4) == 0)
	{
		//std::cout << "\nThis is a single player map" << "\n" << std::endl;
		std::cout << "\nЭто одиночная карта" << "\n" << std::endl;

		parserMapFileSSM(zipData2);
		return;
	}
	else
	if (memcmp(mapType, "SSMM", 4) == 0)
	{
		//std::cout << "\nThis is multiplayer map" << "\n" << std::endl;
		std::cout << "\nЭто карта мультиплеера" << "\n" << std::endl;
		parserMapFileSMM(zipData2);
		return;
	}
	else
	if (memcmp(mapType, "CAMP", 4) == 0)
	{
		//std::cout << "\nThis is part of the company's map, responsible for the landscape and objects" << "\n" << std::endl;
		std::cout << "\nЭто часть карты компании, данный файл отвечает за локацию" << "\n" << std::endl;
		parserMapFileSSC_map(zipData2);
		return;
	}
	else
	if (memcmp(mapType, "CAMS", 4) == 0)
	{
		//std::cout << "\nThis is part of the company's map, responsible for the mission" << "\n" << std::endl;
		std::cout << "\nЭто часть карты компании, данный файл отвечает за миссию" << "\n" << std::endl;
		parserMapFileSCC_mission(zipData2);
		return;
	}
	else
	{
		//std::cout << "This is not map file" << "\n" << std::endl;
		std::cout << "Выбранный файл не является картой" << "\n" << std::endl;
		return;
	}
}
//------------------------------------------------------------------------------
void openFileMapForConvert()
{
	std::string rawData;
	//std::cout << "Specify the file name or path to it: ";
	std::cout << "Введите название файла или путь к нему (не забываем про расширение *.ssm *.smm *.ssc): ";
	//std::cin >> rawData;
	std::getline(std::cin >> std::ws, rawData);
	std::ifstream inputFile(rawData, std::ios::ate | std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		eroropenfile();
		return;
	}
	std::vector<char> mapData(inputFile.tellg());

	inputFile.seekg(std::ios::beg);
	inputFile.read(mapData.data(), mapData.size());
	inputFile.close();

	std::string zipData = gzip::decompress(mapData.data(), mapData.size());
	std::stringstream zipData2;
	zipData2.write(zipData.data(), zipData.size());
	

	char mapType[5];
	zipData2.read(mapType, sizeof(mapType));
	mapType[4] = '\0';

	const std::string convertMapName = rawData.substr(0, rawData.find_last_of(".ssm") - 3);

	if (memcmp(mapType, "SSSM", 4) == 0)
	{
		//std::cout << "\nThis is a single player map" << "\n" << std::endl;
		std::cout << "\nЭто одиночная карта" << "\n" << std::endl;

		convertMapFileSSM(zipData2, convertMapName);
		return;
	}
	else
	if (memcmp(mapType, "SSMM", 4) == 0)
	{
		//std::cout << "\nThis is multiplayer map" << "\n" << std::endl;
		std::cout << "\nЭто карта мультиплеера" << "\n" << std::endl;
		convertMapFileSMM(zipData2, convertMapName);
		return;
	}
	else
	if (memcmp(mapType, "CAMP", 4) == 0)
	{
		//std::cout << "\nThis is part of the company's map, responsible for the landscape and objects" << "\n" << std::endl;
		std::cout << "\nЭто часть карты компании, данный файл отвечает за локацию" << "\n" << std::endl;
		convertMapFileSSC_map(zipData2, convertMapName);
		return;
	}
	else
	if (memcmp(mapType, "CAMS", 4) == 0)
	{
		//std::cout << "\nThis is part of the company's map, responsible for the mission" << "\n" << std::endl;
		std::cout << "\nЭто часть карты компании, данный файл отвечает за миссию" << "\n" << std::endl;
		convertMapFileSCC_mission(zipData2);
		return;
	}
	else
	{
		//std::cout << "This is not map file" << "\n" << std::endl;
		std::cout << "Этот файл не является картой" << "\n" << std::endl;
		return;
	}
}
//------------------------------------------------------------------------------
void command()
{
	std::string commandData;
	std::cout << "SSF Map parser for SS1 & SSF, v.0.6 by NASHRIPPER\n";
	//std::cout << "Program command:\n"
	//	<< "C - command for converter map;\n"
	//	<< "P - command for paser map.\n"
	//	<< "Enter the command: ";
	std::cout << "Команды для управления программой:\n"
		<< "C - команда для конвертирования карты;\n"
		<< "P - команда для разборки карты на составляющие (для разработчика).\n"
		<< "Команды вводятся на английском языке!\n"
		<< "Введите команду: ";
	std::cin >> commandData;
	if (commandData == "p" || commandData == "P")
	{
		openFileMapForPasser();
		return;
	}
	else
	if (commandData == "c" || commandData == "C")
	{
		//cout << "This is map converter" << "\n" << endl;
		openFileMapForConvert();
		return;
	}
	else
	{
		//std::cout << "The wrong command" << "\n" << std::endl;
		std::cout << "Неверная команда" << "\n" << std::endl;
		return;
	}
}
//___________________________________________________________________________________________________
uint32_t main()
{
	//std::system("chcp 1251");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	command();
	std::cin.ignore(10, '\n');
	std::cin.get();
	return 0;
}
