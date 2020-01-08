#include <string>
#include "sha512.hh"

std::string hash(std::string input, int count)
{
	std::string output;
	while (count--) {
		output = sw::sha512::calculate(input);
		input = output;
	}

	return output;
}