#pragma once

void displayinfo(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition);

void errorExistsFile(const std::string_view& path);

void errorOpenFile(const std::string_view& path);

void errorWriteFile();