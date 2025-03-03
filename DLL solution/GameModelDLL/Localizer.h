#pragma once
#include "csv.h"
#include "LocalizedString.h"

class Localizer
{
public:
	static std::string Localize(std::string key, LocalizedString::TABLE table);
	static void SetLanguage(std::string language, std::string cardTablePath, std::string logLabelPath, std::string characterTablePath);

private:
	static rapidcsv::Document* cardDoc;
	static rapidcsv::Document* logDoc;
	static rapidcsv::Document* characterDoc;

	static std::string currentLanguage;
};

