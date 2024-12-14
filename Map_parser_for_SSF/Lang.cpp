
#include "stdafx.h"

#include "Lang.h"

LANGUAGE Dictionary::m_lang{ LANGUAGE::ENGLISH };

// please fill this array in an orderly way for std::find to work correctly
const std::array<DictionaryBlock, std::to_underlying(STRINGS::STRINGS_SIZE)> Dictionary::m_data{
	{
		{ STRINGS::ENTER_FILENAME, {{ { LANGUAGE::RUSSIAN, "Введите имя файла:" }, { LANGUAGE::ENGLISH, "Enter file name:" } }} },
		{ STRINGS::ENTER_PARSE_FILENAME, {{ { LANGUAGE::RUSSIAN, "Вы вошли в режим парсера. Введите имя файла:" }, { LANGUAGE::ENGLISH, "You entered a parser mode. Enter file name: " } }} },
		{ STRINGS::ERROR_FILE_NOT_EXIST, {{ { LANGUAGE::RUSSIAN, "Указанный файл не существует!" }, { LANGUAGE::ENGLISH, "The specified file doesn't exists!" } }} },
		{ STRINGS::ERROR_OPEN , {{ { LANGUAGE::RUSSIAN, "Ошибка открытия файла {}!" }, { LANGUAGE::ENGLISH, "File {} opening error!" } }} },
		{ STRINGS::ERROR_WRITE , {{ { LANGUAGE::RUSSIAN, "Ошибка создания файла для записи!" }, { LANGUAGE::ENGLISH, "File writing error!" } }} },
		{ STRINGS::ERROR_FILE , {{ { LANGUAGE::RUSSIAN, "Файл {} не является картой\n" }, { LANGUAGE::ENGLISH, "The specified file {} is not a map\n" } }} },
		{ STRINGS::CAMP_MAP , {{ { LANGUAGE::RUSSIAN, "\n{} - часть карты кампании, данный файл отвечает за локацию" }, { LANGUAGE::ENGLISH, "\n{} is a part of campaign map, this file is responsible for the location" } }} },
		{ STRINGS::CAMP_MIS , {{ { LANGUAGE::RUSSIAN, "\n{} - часть карты кампании, данный файл отвечает за миссию" }, { LANGUAGE::ENGLISH, "\n{} is a part of campaign map, this file is reponsible for the mission" } }} },
		{ STRINGS::MAP_SINGLE , {{ { LANGUAGE::RUSSIAN, "\n{} - одиночная карта" }, { LANGUAGE::ENGLISH, "\n{} is a singleplayer map" } }} },
		{ STRINGS::MAP_MULTI , {{ { LANGUAGE::RUSSIAN, "\n{} - карта мультиплеера" }, { LANGUAGE::ENGLISH, "\n{} is a multiplayer map" } }} },
		{ STRINGS::SUCCESS_CONVERTED , {{ { LANGUAGE::RUSSIAN, "{} в \033[35m{}\033[0m конвертирован!" }, { LANGUAGE::ENGLISH, "Converted {} to \033[35m{}\033[0m!" } }} },

		{ STRINGS::MAP_SUMMER , {{ { LANGUAGE::RUSSIAN, "Это летняя карта" }, { LANGUAGE::ENGLISH, "This is a summer map" } }} },
		{ STRINGS::MAP_WINTER , {{ { LANGUAGE::RUSSIAN, "Это зимняя карта" }, { LANGUAGE::ENGLISH, "This is a witner map" } }} },
		{ STRINGS::MAP_SEA , {{ { LANGUAGE::RUSSIAN, "Это морская карта" }, { LANGUAGE::ENGLISH, "This is a sea map" } }} },
		{ STRINGS::MAP_DESERT , {{ { LANGUAGE::RUSSIAN, "Это пустынная карта" }, { LANGUAGE::ENGLISH, "This is a desert map" } }} },
		{ STRINGS::MAP_UNKNOWN , {{ { LANGUAGE::RUSSIAN, "Неизвестный тип карты" }, { LANGUAGE::ENGLISH, "This is an unknown map type" } }} },
		{ STRINGS::MAP_SIZE , {{ { LANGUAGE::RUSSIAN, " размером \033[35m{} x {}\033[0m. Размер файла: {} байт" }, { LANGUAGE::ENGLISH, " with size \033[35m{} x {}\033[0m. File size: {} bytes" } }} }
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