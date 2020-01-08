#include <iostream>
#include <chrono>
#include "nipdigests.h"

using namespace std::chrono;

int main()
{
	try {
		const NIPdigests digs("input.json");
		//std::cout << digs.check("5860106143", "10830900000066040620000010") << "\n";
		std::cout << digs.check("5832736211", "12109010980000000009169739") << "\n";
	}
	catch (const std::exception & e) {
		std::cerr << "Exception: " << e.what() << "\n";
		return 1;
	}
	return 0;
}