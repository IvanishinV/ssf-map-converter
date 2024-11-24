#include <iostream>
#include <sstream>
#include <fstream> 

#include "Mis_tree.h"
#include "Displayinfo.h"

void covertMisTree(std::stringstream& mis_desc)
{
		std::ofstream outputFileMisTree("map.000/mis.000/misdesc", std::ios::binary);
		if (!outputFileMisTree)
		{
			erorbuildfile();
			return;
		}
		outputFileMisTree << mis_desc.str();
		outputFileMisTree.close();
}