# SSF Map Converter

**SSF Map Converter** is a utility for converting and parsing maps and missions compiled for **Sudden Strike**, **Sudden Strike Forever** and other derivative games into an **editor-readable format**. It converts campaign, single and multiplayer maps and missions.

## Features

- Converts `.ssc`, `.ssm` and `.smm` files into editable folders
- Supports campaign, single, and multiplayer missions
- Automatic language detection (English / Russian)
- Simple drag-and-drop or CLI usage

## Usage

You can run the application by double-clicking or via the command line. The converter supports English and Russian languages, which will be selected automatically depending on the system language. By default, it uses English.

### Language Support

- Automatically selects language based on system locale
- Defaults to English if detection fails

### Input options

You can specify a **file** or **folder** as input:
- If a **folder** is specified, all supported files inside will be converted.
- If a **file** is specified, only that file will be converted.

You can specify the **relative or full path** to the file or folder when running the program. This works both via drag-and-drop and command-line usage.

#### Command Line Example

```bash
map_converter <file|campaign folder>           # convert (default)
map_converter -c <file|campaign folder>        # convert (explicit)
map_converter -p <file|campaign folder>        # parse — split the packed binary
                                               #   into per-section files for
                                               #   inspection / reverse engineering
```

`-c` (convert) produces the editor's working format (`map.<name>/...`) — the normal end-user mode.

`-p` (parse) produces `parser_map.<input-path-string>/...` containing each raw on-disk section as its own file. Useful for debugging or reverse-engineering the wire format; not needed for ordinary use.

### Output Format

| Input File Type        | Example Name       | Output Folder Name         |
|------------------------|--------------------|----------------------------|
| Campaign map           | `042.ssc`          | `map.042`                  |
| Campaign mission       | `042001.ssc`       | `map.042/mis.001`          |
| Single-player map      | `Spionage.ssm`     | `map.Spionage.ssm`         |
| Multiplayer map        | `I (3-3).smm`      | `map.I (3-3).smm`          |

After conversion, move the folder into your game's `maps.ca` directory. The editor only recognizes folders named like `map.***`.

## Dependencies

This project uses the following third-party libraries:

- **zlib** by Jean-loup Gailly and Mark Adler

- **gzip-hpp** by Mapbox Inc.

## Compilation

Cross-platform via CMake. Builds on Windows, macOS, and Linux.

### Requirements

- C++23 compiler: Apple Clang 18+, MSVC 19.40+, or GCC 14+
- CMake 3.21+
- zlib (system-provided on Linux/macOS; bundled `libs/zlib/` on Windows)

### Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

- **macOS:** install zlib via `brew install zlib`, then pass `-DZLIB_ROOT=/opt/homebrew/opt/zlib` to `cmake -S . -B build`.
- **Linux:** system zlib is used automatically; no extra flag needed.
- **Windows:** the bundled `libs/zlib/` is linked statically; no system zlib required.

The output binary is `build/map_converter` (or `build/Release/map_converter.exe` on multi-config Windows generators).

Release builds enable LTO (`CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE=ON`) for a smaller binary; no flag needed.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Contact

For author information and ways to get in touch, see the [Contact Information](CONTACT.md) file.