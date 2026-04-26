
#include "stdafx.h"

#include "Converter.h"
#include "Parser.h"
#include "Displayinfo.h"
#include "util.h"

#include <gzip/decompress.hpp>
#ifdef _MSC_VER
#pragma comment(lib, "zlibstatic.lib")
#endif

template <typename Fn>
static size_t processPath(const std::filesystem::path& path, Fn&& process)
{
	if (!std::filesystem::is_directory(path))
	{
		process(path);
		return 1;
	}

	// Snapshot entries before processing: the binary writes outputs back into
	// the same directory, and concurrent mutation of a directory_iterator is
	// implementation-defined.
	std::vector<std::filesystem::path> files;
	for (const auto& entry : std::filesystem::directory_iterator(path))
		if (entry.is_regular_file())
			files.push_back(entry.path());

	for (const auto& f : files)
		process(f);
	return files.size();
}

void openFileAndProcess(const Action action, std::string_view filename)
{
	const auto start = std::chrono::high_resolution_clock::now();

	size_t count{ 0 };
	if (action == Action::Convert)
	{
		Converter conv;
		count = processPath(filename, [&](const std::filesystem::path& p) { conv.convertMap(p); });
	}
	else
	{
		Parser parser;
		count = processPath(filename, [&](const std::filesystem::path& p) { parser.parseMap(p.string()); });
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

	using namespace std::literals::string_view_literals;
	if (filename == "-p"sv)
	{
		own::println(Dictionary::getValue(STRINGS::ENTER_PARSE_FILENAME));
		std::getline(std::cin >> std::ws, filename);
		action = Action::Parse;
	}

	openFileAndProcess(action, filename);

	std::println("Press Enter to exit...");
	std::cin.get();
}

void printUsage(std::string_view appName)
{
	std::println("Usage: {} <file|campaign folder>", appName);
}

static void setupConsole()
{
#ifdef _WIN32
	const HANDLE hs = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	if (!GetConsoleMode(hs, &consoleMode))
		return;

	const DWORD wanted = ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if ((consoleMode & wanted) == wanted)
		return;

	if (SetConsoleMode(hs, wanted) == NULL)
		std::println("[Error] Couldn't set color output mode. Output will be with \"\033[32m\" symbols. Last error: {}", GetLastError());
#endif
}

static void detectLanguage()
{
#ifdef _WIN32
	switch (GetUserDefaultUILanguage())
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
	}
#else
	const char* lang = std::getenv("LANG");
	if (!lang)
		return;

	const std::string_view sv{lang};
	for (const auto prefix : {"ru", "be", "uk", "kk", "ky", "tt", "uz"})
		if (sv.starts_with(prefix))
		{
			Dictionary::setLang(LANGUAGE::RUSSIAN);
			return;
		}
#endif
}

struct CliArgs
{
	enum class Mode { Help, Manual, Process, BadUsage };
	Mode mode{ Mode::Manual };
	Action action{ Action::Convert };
	std::string_view path;
};

static CliArgs parseArgs(int argc, char** argv)
{
	using namespace std::literals;
	CliArgs args;

	if (argc == 1)
	{
		args.mode = CliArgs::Mode::Manual;
		return args;
	}
	if (argc == 2)
	{
		if (argv[1] == "-h"sv)
			args.mode = CliArgs::Mode::Help;
		else
		{
			args.mode = CliArgs::Mode::Process;
			args.action = Action::Convert;
			args.path = argv[1];
		}
		return args;
	}
	if (argc == 3)
	{
		if (argv[1] == "-c"sv)
		{
			args.mode = CliArgs::Mode::Process;
			args.action = Action::Convert;
			args.path = argv[2];
			return args;
		}
		if (argv[1] == "-p"sv)
		{
			args.mode = CliArgs::Mode::Process;
			args.action = Action::Parse;
			args.path = argv[2];
			return args;
		}
	}

	args.mode = CliArgs::Mode::BadUsage;
	return args;
}

int main(int argc, char** argv)
{
	setupConsole();
	detectLanguage();

	std::println("Map converter for SS1 & SSF, \033[32mv.0.6.8\033[0m by NASHRIPPER and IVA");

	const CliArgs args = parseArgs(argc, argv);
	switch (args.mode)
	{
	case CliArgs::Mode::Help:
	case CliArgs::Mode::BadUsage:
		printUsage(argv[0]);
		return 0;
	case CliArgs::Mode::Process:
		openFileAndProcess(args.action, args.path);
		return 0;
	case CliArgs::Mode::Manual:
		manualInput();
		return 0;
	}
	return 0;
}
