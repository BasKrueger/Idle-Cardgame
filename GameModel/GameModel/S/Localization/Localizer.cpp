#include "H/Localizer.h"

#define LOOK_UP(key, table, doc, cacheIndex)\
if (cachedLocalizations[0].count(key)) return cachedLocalizations[0][key];\
if (cardDoc != nullptr)\
{\
    auto result = doc->GetCell<std::string>(currentLanguage, key);\
    cachedLocalizations[0].insert({ key, result });\
    return result;\
}

rapidcsv::Document* Localizer::cardDoc = nullptr;
rapidcsv::Document* Localizer::characterDoc = nullptr;
std::string Localizer::currentLanguage = "undefined";
std::array<std::map<std::string, std::string>, 2> Localizer::cachedLocalizations;

std::string Localizer::Localize(std::string key, LocalizedString::TABLE table)
{
    if(currentLanguage != "undefined")
    {
        switch (table)
        {
            case LocalizedString::TABLE::CARDS:
                LOOK_UP(key, table, cardDoc, 0);
       
            case LocalizedString::TABLE::CHARACTERS:
                LOOK_UP(key, table, characterDoc, 1);
        }
    }

    return "Localization " + key + " --- " + currentLanguage + " not found!";;
}

void Localizer::SetLanguage(std::string language, std::string cardTable, std::string logTable, std::string characterTable)
{
    {
        if (cardDoc != 0) delete cardDoc;
        std::stringstream sstream(cardTable);
        cardDoc = new rapidcsv::Document(sstream, rapidcsv::LabelParams(0, 0));
    }
    
    {
        if (characterDoc != 0) delete characterDoc;
        std::stringstream sstream(characterTable);
        characterDoc = new rapidcsv::Document(sstream, rapidcsv::LabelParams(0, 0));
    }

    currentLanguage = language;
}
