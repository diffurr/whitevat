#pragma once
#include <fstream>
#include <string>
#include "nipdigests.h"

void NIPdigests::loadFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (file) {
		file >> m_json;
	}
	else {
		throw std::runtime_error("Can't open file: " + filename);
	}
}