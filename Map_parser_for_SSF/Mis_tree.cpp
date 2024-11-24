
#include "stdafx.h"

#include "Mis_tree.h"
#include "Displayinfo.h"

void covertMisTree(const std::vector<uint8_t>& mis_desc)
{
		std::ofstream outputFileMisTree("map.000/mis.000/misdesc", std::ios::binary);
		if (!outputFileMisTree)
		{
			errorBuildFile();
			return;
		}

		outputFileMisTree.write((char*)mis_desc.data(), mis_desc.size());
		outputFileMisTree.close();
}