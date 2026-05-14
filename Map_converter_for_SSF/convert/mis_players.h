#pragma once

#include <filesystem>
#include <string_view>

namespace convert {

void mis_players(const std::filesystem::path& mis_folder,
                 std::string_view data);

} // namespace convert
