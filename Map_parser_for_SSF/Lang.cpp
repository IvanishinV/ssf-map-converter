
#include "stdafx.h"

#include "Lang.h"

LANGUAGE Dictionary::m_lang{ LANGUAGE::ENGLISH };

// please fill this array in an orderly way for std::find to work correctly
const std::array<DictionaryBlock, std::to_underlying(STRINGS::STRINGS_SIZE)> Dictionary::m_data{
	{
		{ STRINGS::ENTER_FILENAME, {{ { LANGUAGE::RUSSIAN, "������� ��� �����:" }, { LANGUAGE::ENGLISH, "Enter file name:" } }} },
		{ STRINGS::ENTER_PARSE_FILENAME, {{ { LANGUAGE::RUSSIAN, "�� ����� � ����� �������. ������� ��� �����:" }, { LANGUAGE::ENGLISH, "You entered a parser mode. Enter file name: " } }} },
		{ STRINGS::ERROR_FILE_NOT_EXIST, {{ { LANGUAGE::RUSSIAN, "��������� ���� �� ����������!" }, { LANGUAGE::ENGLISH, "The specified file doesn't exists!" } }} },
		{ STRINGS::ERROR_OPEN , {{ { LANGUAGE::RUSSIAN, "������ �������� ����� {}!" }, { LANGUAGE::ENGLISH, "File {} opening error!" } }} },
		{ STRINGS::ERROR_WRITE , {{ { LANGUAGE::RUSSIAN, "������ �������� ����� ��� ������!" }, { LANGUAGE::ENGLISH, "File writing error!" } }} },
		{ STRINGS::ERROR_FILE , {{ { LANGUAGE::RUSSIAN, "���� {} �� �������� ������\n" }, { LANGUAGE::ENGLISH, "The specified file {} is not a map\n" } }} },
		{ STRINGS::CAMP_MAP , {{ { LANGUAGE::RUSSIAN, "\n{} - ����� ����� ��������, ������ ���� �������� �� �������" }, { LANGUAGE::ENGLISH, "\n{} is a part of campaign map, this file is responsible for the location" } }} },
		{ STRINGS::CAMP_MIS , {{ { LANGUAGE::RUSSIAN, "\n{} - ����� ����� ��������, ������ ���� �������� �� ������" }, { LANGUAGE::ENGLISH, "\n{} is a part of campaign map, this file is reponsible for the mission" } }} },
		{ STRINGS::MAP_SINGLE , {{ { LANGUAGE::RUSSIAN, "\n{} - ��������� �����" }, { LANGUAGE::ENGLISH, "\n{} is a singleplayer map" } }} },
		{ STRINGS::MAP_MULTI , {{ { LANGUAGE::RUSSIAN, "\n{} - ����� ������������" }, { LANGUAGE::ENGLISH, "\n{} is a multiplayer map" } }} },
		{ STRINGS::SUCCESS_CONVERTED , {{ { LANGUAGE::RUSSIAN, "{} � \033[35m{}\033[0m �������������!" }, { LANGUAGE::ENGLISH, "Converted {} to \033[35m{}\033[0m!" } }} },

		{ STRINGS::MAP_SUMMER , {{ { LANGUAGE::RUSSIAN, "��� ������ �����" }, { LANGUAGE::ENGLISH, "This is a summer map" } }} },
		{ STRINGS::MAP_WINTER , {{ { LANGUAGE::RUSSIAN, "��� ������ �����" }, { LANGUAGE::ENGLISH, "This is a witner map" } }} },
		{ STRINGS::MAP_SEA , {{ { LANGUAGE::RUSSIAN, "��� ������� �����" }, { LANGUAGE::ENGLISH, "This is a sea map" } }} },
		{ STRINGS::MAP_DESERT , {{ { LANGUAGE::RUSSIAN, "��� ��������� �����" }, { LANGUAGE::ENGLISH, "This is a desert map" } }} },
		{ STRINGS::MAP_UNKNOWN , {{ { LANGUAGE::RUSSIAN, "����������� ��� �����" }, { LANGUAGE::ENGLISH, "This is an unknown map type" } }} },
		{ STRINGS::MAP_SIZE , {{ { LANGUAGE::RUSSIAN, " �������� \033[35m{} x {}\033[0m. ������ �����: {} ����" }, { LANGUAGE::ENGLISH, " with size \033[35m{} x {}\033[0m. File size: {} bytes" } }} }
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