#include "pch.h"
#include "Localizer.h"

rapidcsv::Document* Localizer::cardDoc = 0;
rapidcsv::Document* Localizer::logDoc = 0;
rapidcsv::Document* Localizer::characterDoc = 0;
std::string Localizer::currentLanguage;

std::string Localizer::Localize(std::string key, LocalizedString::TABLE table)
{
    switch(table)
    {
        case LocalizedString::TABLE::CARDS:
            if (cardDoc != 0) return cardDoc->GetCell<std::string>(currentLanguage, key);
        case LocalizedString::TABLE::ADVENTURE_LOG:
            if (logDoc != 0) return logDoc->GetCell<std::string>(currentLanguage, key);
        case LocalizedString::TABLE::CHARACTERS:
            if (characterDoc != 0) return characterDoc->GetCell<std::string>(currentLanguage, key);
    }

    return "Localization " + key + " not found!";
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
