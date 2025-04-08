
#include "stdafx.h"

#include "Lang.h"

LANGUAGE Dictionary::m_lang{ LANGUAGE::ENGLISH };

// please fill this array in an orderly way for std::find to work correctly
const std::array<DictionaryBlock, std::to_underlying(STRINGS::STRINGS_SIZE)> Dictionary::m_data{
	{
		{ STRINGS::ENTER_FILENAME, {{ { LANGUAGE::RUSSIAN, "Введите имя файла или папки с миссиями кампании:" }, { LANGUAGE::ENGLISH, "Enter file name or folder name with campaign missions:" } }} },
		{ STRINGS::ENTER_PARSE_FILENAME, {{ { LANGUAGE::RUSSIAN, "Вы вошли в режим парсера. Введите имя файла или папки с миссиями кампании:" }, { LANGUAGE::ENGLISH, "You entered a parser mode. Enter file name or folder name with campaign missions: " } }} },
		{ STRINGS::ERROR_FILE_NOT_EXIST, {{ { LANGUAGE::RUSSIAN, "Указанный файл не существует!" }, { LANGUAGE::ENGLISH, "The specified file doesn't exists!" } }} },
		{ STRINGS::ERROR_OPEN, {{ { LANGUAGE::RUSSIAN, "Ошибка открытия файла {}!" }, { LANGUAGE::ENGLISH, "File {} opening error!" } }} },
		{ STRINGS::ERROR_WRITE, {{ { LANGUAGE::RUSSIAN, "Ошибка создания файла для записи!" }, { LANGUAGE::ENGLISH, "File writing error!" } }} },
		{ STRINGS::ERROR_FILE, {{ { LANGUAGE::RUSSIAN, "Файл {} не является картой" }, { LANGUAGE::ENGLISH, "The specified file {} is not a map\n" } }} },
		{ STRINGS::CAMP_MAP, {{ { LANGUAGE::RUSSIAN, "\n{} - файл карты кампании, " }, { LANGUAGE::ENGLISH, "\n{} is a file of campaign map, " } }} },
		{ STRINGS::CAMP_MIS, {{ { LANGUAGE::RUSSIAN, "\n{} - файл миссии кампании, " }, { LANGUAGE::ENGLISH, "\n{} is a file of campaign mission, " } }} },
		{ STRINGS::MAP_SINGLE, {{ { LANGUAGE::RUSSIAN, "\n{} - одиночная " }, { LANGUAGE::ENGLISH, "\n{} is a singleplayer " } }} },
		{ STRINGS::MAP_MULTI, {{ { LANGUAGE::RUSSIAN, "\n{} - мультиплеерная " }, { LANGUAGE::ENGLISH, "\n{} is a multiplayer " } }} },
		{ STRINGS::SUCCESS_CONVERTED, {{ { LANGUAGE::RUSSIAN, "{} в \033[35m{}\033[0m конвертирован!" }, { LANGUAGE::ENGLISH, "Converted {} to \033[35m{}\033[0m!" } }} },

		{ STRINGS::MAP_SUMMER, {{ { LANGUAGE::RUSSIAN, "летняя карта" }, { LANGUAGE::ENGLISH, "summer map" } }} },
		{ STRINGS::MAP_WINTER, {{ { LANGUAGE::RUSSIAN, "зимняя карта" }, { LANGUAGE::ENGLISH, "winter map" } }} },
		{ STRINGS::MAP_SEA, {{ { LANGUAGE::RUSSIAN, "морская карта" }, { LANGUAGE::ENGLISH, "sea map" } }} },
		{ STRINGS::MAP_DESERT, {{ { LANGUAGE::RUSSIAN, "пустынная карта" }, { LANGUAGE::ENGLISH, "desert map" } }} },
		{ STRINGS::MAP_MISSION, {{ { LANGUAGE::RUSSIAN, "миссия" }, { LANGUAGE::ENGLISH, "mission" } }} },
		{ STRINGS::MAP_UNKNOWN, {{ { LANGUAGE::RUSSIAN, "Неизвестный тип карты" }, { LANGUAGE::ENGLISH, "This is an unknown map type" } }} },
		{ STRINGS::MAP_SIZE, {{ { LANGUAGE::RUSSIAN, " размером \033[35m{} x {}\033[0m." }, { LANGUAGE::ENGLISH, " of size \033[35m{} x {}\033[0m." } }} },

		{ STRINGS::OUT_OF_RANGE_ERROR, {{ { LANGUAGE::RUSSIAN, "Получена ошибка 'out of range' во время обработки карты. Скорее всего файл битый." }, { LANGUAGE::ENGLISH, "Got an 'out of range' error during file processing. Most likely it's broken." } }} },
		{ STRINGS::EXCEPTION_ERROR, {{ { LANGUAGE::RUSSIAN, "Получена ошибка '{}' во время обработки карты. Скорее всего файл битый." }, { LANGUAGE::ENGLISH, "Got an '{}' error during file processing. Most likely it's broken." } }} },
		{ STRINGS::UNKNOWN_EXCEPTION_ERROR, {{ { LANGUAGE::RUSSIAN, "Получена неизвестная ошибка во время обработки карты. Скорее всего файл битый." }, { LANGUAGE::ENGLISH, "Got an unknown error during file processing. Most likely it's broken." } }} }
	}
};

const std::string_view Dictionary::m_notFound = "Not found lang string";

const std::string_view& Dictionary::getValue(const STRINGS id)
{
	const auto stringId = std::find(m_data.cbegin(), m_data.cend(), id);
	if (stringId != m_data.cend())
	{
		const auto text = std::find(stringId->values.cbegin(), stringId->values.cend(), m_lang);
		if (text != stringId->values.cend())
			return text->text;
	}

	return m_notFound;
}