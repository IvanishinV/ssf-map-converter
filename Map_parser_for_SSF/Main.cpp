
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

void openFileAndProcess(const Action action, const std::string_view& filename)
{
	std::ifstream inputFile(filename.data(), std::ios::ate | std::ios::in | std::ios::binary);
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

	//const auto start = std::chrono::high_resolution_clock::now();

	const uint32_t mapType = *(uint32_t*)rawData.data();
	switch (mapType)
	{
	case (0x4d535353):	// SSSM
	{
		own::println(Dictionary::getValue(STRINGS::MAP_SINGLE));
		//std::println("\nЭто одиночная карта\n");
		if (action == Action::Convert)
			convertMapFileSSM(rawData, convertMapName);
		else
			parserMapFileSSM(rawData);
		break;
	}
	case (0x4d4d5353):	// SSMM
	{
		own::println(Dictionary::getValue(STRINGS::MAP_MULTI));
		//std::println("\nЭто карта мультиплеера\n");
		if (action == Action::Convert)
			convertMapFileSMM(rawData, convertMapName);
		else
			parserMapFileSMM(rawData);
		break;
	}
	case (0x504d4143):	// CAMP
	{
		own::println(Dictionary::getValue(STRINGS::CAMP_MAP));
		//std::println("\nЭто часть карты компании, данный файл отвечает за локацию\n");
		if (action == Action::Convert)
			convertMapFileSSC_map(rawData, convertMapName);
		else
			parserMapFileSSC_map(rawData);
		break;
	}
	case (0x534d4143):	// CAMS
	{
		own::println(Dictionary::getValue(STRINGS::CAMP_MIS));
		//std::println("\nЭто часть карты компании, данный файл отвечает за миссию\n");
		if (action == Action::Convert)
			convertMapFileSCC_mission(rawData);
		else
			parserMapFileSCC_mission(rawData);
		break;
	}
	default:
	{
		own::println(Dictionary::getValue(STRINGS::ERROR_FILE));
		//std::println("\033[31m[Error]\033[0m Выбранный файл не является картой\n");
	}
	};

	//const auto end = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	//std::println("Map processed in {}ms\n", duration.count());
}

void manualInput()
{
	Action action{ Action::Convert };
	std::string filename;

	own::println(Dictionary::getValue(STRINGS::ENTER_FILENAME));
	std::getline(std::cin >> std::ws, filename);

	using namespace std::literals::string_view_literals;	// only for "p"sv
	if (filename == "p"sv)
	{
		own::println(Dictionary::getValue(STRINGS::ENTER_PARSE_FILENAME));
		std::getline(std::cin >> std::ws, filename);

		Action* act = const_cast<Action*>(&action);
		*act = Action::Parse;
	}

	openFileAndProcess(action, filename);

	system("pause");
}

void printUsage(const std::string_view& appName)
{
	std::println("Usage: {} <ch> <filename>", appName);
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

	const LANGID langId = GetUserDefaultUILanguage();
	switch (langId)
	{
	case 0x0423:	// be-BY
	case 0x0419:	// ru-RU
	case 0x0444:	// tt-RU
	case 0x046D:	// ba-RU
	case 0x0485:	// sah-RU
	case 0x0819:	// ru-MD
	case 0x742C:	// az-Cyrl
	case 0x0440:	// ky-KG
	case 0x0442:	// tk-TM
	case 0x0443:	// uk-UA
	case 0x7843:	// uz-Cyrl-UZ
	case 0x0843:	// uz-Cyrl-UZ
		Dictionary::setLang(LANGUAGE::RUSSIAN);

		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		setlocale(LC_ALL, "rus");
		break;
	};

	std::println("SSF Map parser for SS1 & SSF, \033[32mv.0.6.1\033[0m by NASHRIPPER");

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
	if (argc == 2)
	{
		openFileAndProcess(Action::Convert, argv[1]);
		return 0;
	}
	else
		manualInput();

	return 0;
}
