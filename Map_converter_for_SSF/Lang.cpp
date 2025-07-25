
#include "stdafx.h"

#include "Lang.h"

LANGUAGE Dictionary::m_lang{ LANGUAGE::ENGLISH };

// please fill this array in an orderly way for std::find to work correctly
const std::array<DictionaryBlock, std::to_underlying(STRINGS::STRINGS_SIZE)> Dictionary::m_data{
	{
		{ STRINGS::ENTER_FILENAME, {{ { LANGUAGE::RUSSIAN, "������� ��� ����� ��� ����� � �������� ��������:" }, { LANGUAGE::ENGLISH, "Enter file name or folder name with campaign missions:" } }} },
		{ STRINGS::ENTER_PARSE_FILENAME, {{ { LANGUAGE::RUSSIAN, "�� ����� � ����� �������. ������� ��� ����� ��� ����� � �������� ��������:" }, { LANGUAGE::ENGLISH, "You entered a parser mode. Enter file name or folder name with campaign missions: " } }} },
		{ STRINGS::ERROR_FILE_NOT_EXIST, {{ { LANGUAGE::RUSSIAN, "��������� ���� �� ����������!" }, { LANGUAGE::ENGLISH, "The specified file doesn't exists!" } }} },
		{ STRINGS::ERROR_OPEN, {{ { LANGUAGE::RUSSIAN, "������ �������� ����� {}!" }, { LANGUAGE::ENGLISH, "File {} opening error!" } }} },
		{ STRINGS::ERROR_WRITE, {{ { LANGUAGE::RUSSIAN, "������ �������� ����� ��� ������!" }, { LANGUAGE::ENGLISH, "File writing error!" } }} },
		{ STRINGS::ERROR_FILE, {{ { LANGUAGE::RUSSIAN, "���� {} �� �������� ������" }, { LANGUAGE::ENGLISH, "The specified file {} is not a map\n" } }} },
		{ STRINGS::CAMP_MAP, {{ { LANGUAGE::RUSSIAN, "\n{} - ���� ����� ��������, " }, { LANGUAGE::ENGLISH, "\n{} is a file of campaign map, " } }} },
		{ STRINGS::CAMP_MIS, {{ { LANGUAGE::RUSSIAN, "\n{} - ���� ������ ��������, " }, { LANGUAGE::ENGLISH, "\n{} is a file of campaign mission, " } }} },
		{ STRINGS::MAP_SINGLE, {{ { LANGUAGE::RUSSIAN, "\n{} - ��������� " }, { LANGUAGE::ENGLISH, "\n{} is a singleplayer " } }} },
		{ STRINGS::MAP_MULTI, {{ { LANGUAGE::RUSSIAN, "\n{} - �������������� " }, { LANGUAGE::ENGLISH, "\n{} is a multiplayer " } }} },
		{ STRINGS::SUCCESS_CONVERTED, {{ { LANGUAGE::RUSSIAN, "{} � \033[35m{}\033[0m �������������!" }, { LANGUAGE::ENGLISH, "Converted {} to \033[35m{}\033[0m!" } }} },

		{ STRINGS::MAP_SUMMER, {{ { LANGUAGE::RUSSIAN, "������ �����" }, { LANGUAGE::ENGLISH, "summer map" } }} },
		{ STRINGS::MAP_WINTER, {{ { LANGUAGE::RUSSIAN, "������ �����" }, { LANGUAGE::ENGLISH, "winter map" } }} },
		{ STRINGS::MAP_SEA, {{ { LANGUAGE::RUSSIAN, "������� �����" }, { LANGUAGE::ENGLISH, "sea map" } }} },
		{ STRINGS::MAP_DESERT, {{ { LANGUAGE::RUSSIAN, "��������� �����" }, { LANGUAGE::ENGLISH, "desert map" } }} },
		{ STRINGS::MAP_MISSION, {{ { LANGUAGE::RUSSIAN, "������" }, { LANGUAGE::ENGLISH, "mission" } }} },
		{ STRINGS::MAP_UNKNOWN, {{ { LANGUAGE::RUSSIAN, "����������� ��� �����" }, { LANGUAGE::ENGLISH, "This is an unknown map type" } }} },
		{ STRINGS::MAP_SIZE, {{ { LANGUAGE::RUSSIAN, " �������� \033[35m{} x {}\033[0m." }, { LANGUAGE::ENGLISH, " of size \033[35m{} x {}\033[0m." } }} },

		{ STRINGS::OUT_OF_RANGE_ERROR, {{ { LANGUAGE::RUSSIAN, "�������� ������ 'out of range' �� ����� ��������� �����. ������ ����� ���� �����." }, { LANGUAGE::ENGLISH, "Got an 'out of range' error during file processing. Most likely it's broken." } }} },
		{ STRINGS::EXCEPTION_ERROR, {{ { LANGUAGE::RUSSIAN, "�������� ������ '{}' �� ����� ��������� �����. ������ ����� ���� �����." }, { LANGUAGE::ENGLISH, "Got an '{}' error during file processing. Most likely it's broken." } }} },
		{ STRINGS::UNKNOWN_EXCEPTION_ERROR, {{ { LANGUAGE::RUSSIAN, "�������� ����������� ������ �� ����� ��������� �����. ������ ����� ���� �����." }, { LANGUAGE::ENGLISH, "Got an unknown error during file processing. Most likely it's broken." } }} }
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