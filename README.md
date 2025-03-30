Map_converter_for_SSF is an utility that allows to convert (and parse) maps and missions compiled for Sudden Strike, Sudden Strike Forever and other derivative games into an editor-readiable format. It converts campaign, single and multiplayer maps and missions.

Usage
-----------

You can run the application by double-clicking or via the command line. The converter supports English and Russian languages, which will be selected automatically depending on the system language. By default, it uses English.

After launching the program, specify the relative or full path to the folder or file of the map/mission. In case of launch via command line, you can specify the path to the folder/file as an argument: Map_converter_for_SSF.exe <file|campaign folder>. 

If a folder was specified, the program will convert all possible files from this folder. If a file was specified, only this file will be converted. For each converted file the program specifies the output folder:
- campaign map (e.g. "042.ssc") is converted into "map.042" folder
- campaign mission (e.g. "042001.ssc", campaign missions are separate from maps) is converted into "map.042/mis.001" folder
- single player map (e.g. "Spionage.ssm") is converted into map.Spionage.ssm" folder
- multi player map (e.g. "I (3-3).smm") is converted into "map.I (3-3).smm" folder
After converting, you can place the output folder in the game's "maps.ca" folder (the editor only reads folders with names in the format "map.***").

Compilation
-----------

Use Visual Studio 2022 and invoke the .sln.
