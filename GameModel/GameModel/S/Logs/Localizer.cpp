#include "H/Localizer.h"

rapidcsv::Document* Localizer::cardDoc = nullptr;
rapidcsv::Document* Localizer::logDoc = nullptr;
rapidcsv::Document* Localizer::characterDoc = nullptr;
std::string Localizer::currentLanguage = "undefined";

std::string Localizer::Localize(std::string key, LocalizedString::TABLE table)
{
    if(currentLanguage != "undefined")
    {
        switch (table)
        {
        case LocalizedString::TABLE::CARDS:
            if (cardDoc != nullptr) return cardDoc->GetCell<std::string>(currentLanguage, key);
        case LocalizedString::TABLE::ADVENTURE_LOG:
            if (logDoc != nullptr) return logDoc->GetCell<std::string>(currentLanguage, key);
        case LocalizedString::TABLE::CHARACTERS:
            if (characterDoc != nullptr) return characterDoc->GetCell<std::string>(currentLanguage, key);
        }
    }

    return "Localization " + key + " --- " + currentLanguage + " not found!";
}

void Localizer::SetLanguage(std::string language, std::string cardTable, std::string logTable, std::string characterTable)
{
    {
        if (cardDoc != 0) delete cardDoc;
        std::stringstream sstream(cardTable);
        cardDoc = new rapidcsv::Document(sstream, rapidcsv::LabelParams(0, 0));
    }
    
    {
        if (logDoc != 0) delete logDoc;
        std::stringstream sstream(logTable);
        logDoc = new rapidcsv::Document(sstream, rapidcsv::LabelParams(0, 0));
    }

    {
        if (characterDoc != 0) delete characterDoc;
        std::stringstream sstream(characterTable);
        characterDoc = new rapidcsv::Document(sstream, rapidcsv::LabelParams(0, 0));
    }

    currentLanguage = language;
}
