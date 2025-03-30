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
MapConverterSSF.exe <file|campaign folder>
```

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

To build the project from source:

### Requirements

- Windows 10 or 11
- Visual Studio 2022 or later
- Visual C++ build tools

### Steps

1. Clone or download this repository.
2. Open the solution file: `MapConverterSSF.sln` in **Visual Studio 2022**.
3. Set the build configuration to either `Release` or `Debug`.
4. Build the solution.

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

## Contact

For author information and ways to get in touch, see the [Contact Information](CONTACT.md) file.