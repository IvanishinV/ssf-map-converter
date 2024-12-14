#pragma once

#include <array>
#include <string>

namespace own
{
	template<typename... Args>
	inline void println(const std::string_view& fmt, Args&&... args)
	{
		std::println("{}", std::vformat(fmt, std::make_format_args(args...)));
	}

	template<typename... Args>
	inline void print(const std::string_view& fmt, Args&&... args)
	{
		std::print("{}", std::vformat(fmt, std::make_format_args(args...)));
	}

	template<typename... Args>
	inline void printlnError(const std::string_view& fmt, Args&&... args)
	{
		std::println("\033[31m[Error]\033[0m {}", std::vformat(fmt, std::make_format_args(args...)));
	}

	template<typename... Args>
	inline void printlnSuccess(const std::string_view& fmt, Args&&... args)
	{
		std::println("\033[32m[Success]\033[0m {}", std::vformat(fmt, std::make_format_args(args...)));
	}
}

enum class STRINGS
{
	ENTER_FILENAME,
	ENTER_PARSE_FILENAME,
	ERROR_FILE_NOT_EXIST,
	ERROR_OPEN,
	ERROR_WRITE,
	ERROR_FILE,
	CAMP_MAP,
	CAMP_MIS,
	MAP_SINGLE,
	MAP_MULTI,
	SUCCESS_CONVERTED,

	MAP_SUMMER,
	MAP_WINTER,
	MAP_SEA,
	MAP_DESERT,
	MAP_UNKNOWN,
	MAP_SIZE,
	STRINGS_SIZE	// used only for static assert
};

enum class LANGUAGE
{
	RUSSIAN,
	ENGLISH,
	NUMBER_OF_LANGUAGES
};

struct DictionaryVal
{
	LANGUAGE lang;
	std::string_view text;

	bool operator==(const LANGUAGE& rhs) const
	{
		return this->lang == rhs;
	}
};

struct DictionaryBlock
{
	STRINGS stringId;
	std::array<DictionaryVal, std::to_underlying(LANGUAGE::NUMBER_OF_LANGUAGES)> values;

	bool operator ==(const STRINGS& rhs) const
	{
		return this->stringId == rhs;
	}
};

class Dictionary
{
	static LANGUAGE m_lang;

	static const std::string_view m_notFound;

public:
	static const std::array<DictionaryBlock, std::to_underlying(STRINGS::STRINGS_SIZE)> m_data;

	static const std::string_view& getValue(const STRINGS id);

	static void setLang(const LANGUAGE lang)
	{
		m_lang = lang;
	}
};