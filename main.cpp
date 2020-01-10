#include <iostream>
#include <chrono>
#include <string>
#include "nipdigests.h"

using namespace std::chrono;

static std::string clean(const std::string& input)
{
	std::string output;
	std::copy_if(input.begin(), input.end(), std::back_inserter(output),
		[](const char c) {
			if (c == '-' || c == ' ') return false;
			else return true;
		});

	return output;
}

static bool correct(const std::string& input)
{
	return !std::any_of(input.begin(), input.end(),
		[](const char c) {
			if (isdigit(c)) return false;
			else return true;
		});
}

int main()
{
	try {
		const NIPdigests digs("input.json");
		std::string line, nip, nrb;
		while (std::getline(std::cin, line)) {
			auto commaPos = line.find_first_of(',');
			if (std::string::npos == commaPos) {
				throw std::runtime_error("Wrong input");
			}
			else {
				nip = clean(line.substr(0, commaPos));
				nrb = clean(line.substr(commaPos + 1, line.size() - commaPos - 1));
				if (nip.size() != 10 || nrb.size() != 26 || !correct(nip) || !correct(nrb)) {
					throw std::runtime_error("Wrong input");
				}
			}

			CheckResults result = digs.check(nip, nrb);
			std::cout << "Zgodnie z danymi z dnia " << getDateStr(digs, "%Y-%m-%d") <<
				" NIP:" << nip << " NRB:" << nrb;
			if (ACTIVE == result) {
				std::cout << " jest aktywnym platnikiem vat\n";
			}
			else if (INACTIVE == result) {
				std::cout << " jest zwolnionym platnikiem vat\n";
			}
			else {
				std::cout << " nie zostal znaleziony na bialej liscie platnikow vat\n";
			}
		}
	}                                            
	catch (const std::exception & e) {
		std::cerr << "Exception: " << e.what() << "\n";
		return 1;
	}
	return 0;
}