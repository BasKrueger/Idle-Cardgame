#pragma once
#include "csv.h"
#include "LocalizedString.h"
#include <map>
#include <array>

class Localizer
{
public:
	static std::string Localize(std::string key, LocalizedString::TABLE table);
	static void SetLanguage(std::string language, std::string cardTablePath, std::string logLabelPath, std::string characterTablePath);

private:
	static rapidcsv::Document* cardDoc;
	static rapidcsv::Document* characterDoc;

	static std::string currentLanguage;
	static std::array<std::map<std::string, std::string>, 2> cachedLocalizations;
};

