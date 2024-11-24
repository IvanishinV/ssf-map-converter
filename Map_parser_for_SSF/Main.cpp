
#include "stdafx.h"

#include "Converter.h"
#include "Parser.h"
#include "Displayinfo.h"

#include <gzip/decompress.hpp>
#pragma comment(lib, "zlibstatic.lib")

enum class Action
{
	Parse,
	Convert
};

void openFileAndProcess(const Action action, const std::string_view& file_arg = "")
{
	std::string filename;

	if (file_arg == "")
	{
		std::println("\nВведите название файла или путь к нему (не забываем про расширение *.ssm *.smm *.ssc): ");
		std::getline(std::cin >> std::ws, filename);
	}
	else
	{
		filename = file_arg;
	}

	std::ifstream inputFile(filename, std::ios::ate | std::ios::in | std::ios::binary);
	if (!inputFile)
	{
		errorOpenFile();
		return;
	}
	
	std::vector<char> zipData(inputFile.tellg());

	inputFile.seekg(std::ios::beg);
	inputFile.read(zipData.data(), zipData.size());
	inputFile.close();

	const std::string rawData = gzip::decompress(zipData.data(), zipData.size());
	const std::string_view convertMapName{ std::string_view(filename).substr(0, filename.find_last_of(".")) };

	const auto start = std::chrono::high_resolution_clock::now();

	const uint32_t mapType = *(uint32_t*)rawData.data();
	switch (mapType)
	{
	case (0x4d535353):	// SSSM
	{
		std::println("\nЭто одиночная карта\n");
		if (action == Action::Parse)
			parserMapFileSSM(rawData);
		else
			convertMapFileSSM(rawData, convertMapName);
		break;
	}
	case (0x4d4d5353):	// SSMM
	{
		std::println("\nЭто карта мультиплеера\n");
		if (action == Action::Parse)
			parserMapFileSMM(rawData);
		else
			convertMapFileSMM(rawData, convertMapName);
		break;
	}
	case (0x504d4143):	// CAMP
	{
		std::println("\nЭто часть карты компании, данный файл отвечает за локацию\n");
		if (action == Action::Parse)
			parserMapFileSSC_map(rawData);
		else
			convertMapFileSSC_map(rawData, convertMapName);
		break;
	}
	case (0x534d4143):	// CAMS
	{
		std::println("\nЭто часть карты компании, данный файл отвечает за миссию\n");
		if (action == Action::Parse)
			parserMapFileSCC_mission(rawData);
		else
			convertMapFileSCC_mission(rawData);
		break;
	}
	default:
	{
		std::println("\033[31m[Error]\033[0m Выбранный файл не является картой\n");
	}
	};

	const auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::println("Map processed in {}ms\n", duration.count());
}

void manualInput()
{
	char commandData;

	std::println("\nКоманды для управления программой:\n"
		"(C)onvert - команда для конвертирования карты;\n"
		"(P)arse - команда для разборки карты на составляющие (для разработчика).\n"
		"Введите команду (C/P): ");

	commandData = getchar();
	switch (commandData)
	{
	case 'p':
	case 'P':
		openFileAndProcess(Action::Parse);
		break;
	case 'c':
	case 'C':
		openFileAndProcess(Action::Convert);
		break;
	default:
		std::println("\033[31m[Error]\033[0m Неизвестная команда. Завершение приложения.\n");
		break;
	};

	system("pause");
}

void printUsage(const std::string_view& appName)
{
	std::println("Usage: {} -<chp> <filename>", appName);
}

int main(int argc, char** argv)
{
	// This is for color output
	const HANDLE hs = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	if (GetConsoleMode(hs, &consoleMode))
	{
		if ((consoleMode & (ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) != (ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
		{
			if (SetConsoleMode(hs, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING) == NULL)
			{
				std::println("[Error] Couldn't set color output mode. Output will be with \"\033[32m\" symbols. Last error: {}", GetLastError());
			}
		}
	}

	std::println("SSF Map parser for SS1 & SSF, \033[32mv.0.6.1\033[0m by NASHRIPPER");

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");

	if (argc == 3)
	{
		using namespace std::literals;
		if (argv[1] == "-c"sv)
			openFileAndProcess(Action::Convert, argv[2]);
		else if (argv[1] == "-p"sv)
			openFileAndProcess(Action::Parse, argv[2]);
		else
		{
			printUsage(argv[0]);
			return 0;
		}
	}
	if (argc == 2)\
	{
		printUsage(argv[0]);
		return 0;
	}
	else
		manualInput();

	return 0;
}
