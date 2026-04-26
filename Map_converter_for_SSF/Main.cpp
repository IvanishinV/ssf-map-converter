
#include "stdafx.h"

#include "Converter.h"
#include "Parser.h"
#include "Displayinfo.h"
#include "util.h"

#include <gzip/decompress.hpp>
#ifdef _MSC_VER
#pragma comment(lib, "zlibstatic.lib")
#endif

void openFileAndProcess(const Action action, const std::string_view& filename)
{
	const auto start = std::chrono::high_resolution_clock::now();

	size_t count{ 0 };

	if (action == Action::Convert)
	{
		Converter conv;
		if (std::filesystem::is_directory(filename))
		{
			for (const auto& file : std::filesystem::directory_iterator(filename))
				if (file.is_regular_file())
				{
					conv.convertMap(file.path());
					++count;
				}
		}
		else
		{
			conv.convertMap(filename);
			++count;
		}
	}
	else
	{
		Parser parser;
		if (std::filesystem::is_directory(filename))
		{
			for (const auto& file : std::filesystem::directory_iterator(filename))
				if (file.is_regular_file())
				{
					parser.parseMap(file.path().string());
					++count;
				}
		}
		else
		{
			parser.parseMap(filename);
			++count;
		}
	}

	const auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::println("Processed {} file(s) in {}ms", count, duration.count());
}

void manualInput()
{
	Action action{ Action::Convert };
	std::string filename;

	own::println(Dictionary::getValue(STRINGS::ENTER_FILENAME));
	std::getline(std::cin >> std::ws, filename);

	using namespace std::literals::string_view_literals;	// only for "p"sv
	if (filename == "-p"sv)
	{
		own::println(Dictionary::getValue(STRINGS::ENTER_PARSE_FILENAME));
		std::getline(std::cin >> std::ws, filename);

		Action* act = const_cast<Action*>(&action);
		*act = Action::Parse;
	}

	openFileAndProcess(action, filename);

#ifdef _WIN32
	system("pause");
#else
	std::println("Press Enter to exit...");
	std::cin.get();
#endif
}

void printUsage(const std::string_view& appName)
{
	std::println("Usage: {} <file|campaign folder>", appName);
}

int main(int argc, char** argv)
{
#ifdef _WIN32
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
#else
	if (const char* lang = std::getenv("LANG"); lang)
	{
		std::string_view sv{lang};
		if (sv.starts_with("ru") || sv.starts_with("be") || sv.starts_with("uk")
		    || sv.starts_with("kk") || sv.starts_with("ky") || sv.starts_with("tt")
		    || sv.starts_with("uz"))
		{
			Dictionary::setLang(LANGUAGE::RUSSIAN);
		}
	}
#endif

	std::println("Map converer for SS1 & SSF, \033[32mv.0.6.8\033[0m by NASHRIPPER and IVA");

	using namespace std::literals;
	if (argc == 3)
	{
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
	else if (argc == 2)
	{
		if (argv[1] == "-h"sv)
		{
			printUsage(argv[0]);
		}
		else
		{
			openFileAndProcess(Action::Convert, argv[1]);
		}
		return 0;
	}
	else
		manualInput();

	return 0;
}
