#pragma once
#include <ctime>
#include <string>
#include <nlohmann/json.hpp>

enum CheckResults {FAIL, ACTIVE, INACTIVE, MASK};

class NIPdigests {
private:
	nlohmann::json m_json;
	int m_nHashes;
	std::string m_date;
	tm m_utcDate;
	nlohmann::json::array_t* m_activeDigs;
	nlohmann::json::array_t* m_inactiveDigs;
	nlohmann::json::array_t* m_masks;
	
	void loadFile(const std::string&);
	void init();
	CheckResults searchDigs(const std::string&, const std::string&) const;
	CheckResults searchMasks(const std::string&, const std::string&) const;
public:
	NIPdigests(const std::string& filename) {
		loadFile(filename);
		init();
	}
	CheckResults check(const std::string&, const std::string&) const;
};