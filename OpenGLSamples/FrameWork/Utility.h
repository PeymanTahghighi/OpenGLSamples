#pragma once
//=======================================================================================
//=======================================================================================
#include<string>
#include <iostream>
#include <fstream>
#include<assert.h>
//=======================================================================================
//=======================================================================================

//---------------------------------------------------------------------------------------
inline bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);

	bool ret = false;

	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		ret = true;
	}
	else {
		assert(false);
	}

	return ret;
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
inline std::string extractPath(const std::string path)
{
	int index = path.rfind("\\");
	std::string ret = "";
	if (index != std::string::npos)
	{
		ret = path.substr(0, index);
		ret += "\\";
	}
	return ret;
}
//---------------------------------------------------------------------------------------
