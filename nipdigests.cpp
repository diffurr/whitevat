#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "nipdigests.h"
#include "hash.h"

using json = nlohmann::json;

void NIPdigests::init()
{
	m_nHashes = std::stoi(m_json["naglowek"]["liczbaTransformacji"].get<std::string>());
	m_date = m_json["naglowek"]["dataGenerowaniaDanych"].get<std::string>();

	memset(&m_utcDate, 0, sizeof(m_utcDate));
	m_utcDate.tm_year = std::stoi(m_date.substr(0, 4)) - 1900;
	m_utcDate.tm_mon = std::stoi(m_date.substr(4, 2)) - 1;
	m_utcDate.tm_mday = std::stoi(m_date.substr(6, 2));

	m_activeDigs = m_json["skrotyPodatnikowCzynnych"].get_ptr<json::array_t*>();
	m_inactiveDigs = m_json["skrotyPodatnikowZwolnionych"].get_ptr<json::array_t*>();
	m_masks = m_json["maski"].get_ptr<json::array_t*>();
}

CheckResults NIPdigests::check(const std::string& nip, const std::string& nrb) const
{
	CheckResults result = searchDigs(nip, nrb);
	if (!result) {
		result = searchMasks(nip, nrb);
	}
	return result;
}

CheckResults NIPdigests::searchDigs(const std::string& nip, const std::string& nrb) const
{
	const std::string digest = hash(m_date + nip + nrb, m_nHashes);

	if (std::binary_search(m_activeDigs->begin(), m_activeDigs->end(), digest)) {
		return CheckResults::ACTIVE;
	}
	else if (std::binary_search(m_inactiveDigs->begin(), m_inactiveDigs->end(), digest)) {
		return CheckResults::INACTIVE;
	}
	else {
		return CheckResults::FAIL;
	}
}

static std::string makeVirtualNrb(const std::string& nrb, const std::string& mask)
{
	std::string virtualNrb;
	virtualNrb.reserve(nrb.size());

	std::transform(nrb.begin(), nrb.end(), mask.begin(), std::back_inserter(virtualNrb),
		[](char nrbChar, char maskChar) -> char {
			if (maskChar == 'X') return maskChar;
			if (maskChar == 'Y') return nrbChar;
			return nrbChar;
		});

	return virtualNrb;
}

CheckResults NIPdigests::searchMasks(const std::string& nip, const std::string& nrb) const
{
	CheckResults result = CheckResults::FAIL;

	auto masks = std::equal_range(m_masks->begin(), m_masks->end(), nrb,
		[](const std::string& comp1, const std::string& comp2) {
			return comp1.substr(2, 8) < comp2.substr(2, 8);
		});
	for (auto i = masks.first; i != masks.second; i++) {
		if (searchDigs(nip, makeVirtualNrb(nrb, *i))) {
			result = CheckResults::MASK;
			break;
		}
	}

	return result;
}

std::string getDateStr(const NIPdigests& digs, const std::string& form)
{
	char buffer[BUFSIZ];
	strftime(buffer, BUFSIZ, form.c_str(), &digs.m_utcDate);
	return std::string(buffer);
}