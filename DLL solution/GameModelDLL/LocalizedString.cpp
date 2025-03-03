#include "pch.h"
#include "LocalizedString.h"
#include "Localizer.h"

LocalizedStringFormatVariable::LocalizedStringFormatVariable() { state = std::make_unique<json::JSON>(); }
LocalizedString::LocalizedString() {}
void LocalizedString::SetKey(std::string textKey) { key = textKey; }
void LocalizedString::SetTable(TABLE newTable) { table = newTable; }
void LocalizedString::BindFormatVariable(std::string staticValue) { formatValues.push_back(std::make_unique<LocalizedStringFormatVariable>(staticValue)); }
void LocalizedString::BindFormatVariable(LocalizedString* localizedValue) { formatValues.push_back(std::make_unique<LocalizedStringFormatVariable>(localizedValue)); }
void LocalizedString::BindFormatVariable(std::unique_ptr<LocalizedString>* localizedValue) { BindFormatVariable(localizedValue->get()); }
void LocalizedString::BindFormatVariable(int* dynamicValue) { formatValues.push_back(std::make_unique<LocalizedStringFormatVariable>(dynamicValue)); }

void LocalizedString::UnbindAll()
{
	formatValues.clear();
}

void ReplaceAll(std::string& s, const std::string& search, const std::string& replace) {
	for (size_t pos = 0; ; pos += replace.length()) {
		pos = s.find(search, pos);
		if (pos == std::string::npos) break;
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

LocalizedStringFormatVariable::LocalizedStringFormatVariable(std::string staticValue)
{
	staticVariable = staticValue;
	state = std::make_unique<json::JSON>();
}

LocalizedStringFormatVariable::LocalizedStringFormatVariable(LocalizedString* localizedValue)
{
	localizedVariable = localizedValue;
	state = std::make_unique<json::JSON>();
}

LocalizedStringFormatVariable::LocalizedStringFormatVariable(int* dynamicValue)
{
	dynamicVariable = dynamicValue;
	state = std::make_unique<json::JSON>();
}

std::string LocalizedStringFormatVariable::Format()
{
	if (staticVariable != "") return staticVariable;
	if (dynamicVariable != 0) return std::to_string(*dynamicVariable);
	if (localizedVariable != 0) return localizedVariable->Format();

	return "Empty Format Variable";
}


LocalizedString::LocalizedString(TABLE table)
{
	this->table = table;
	state = std::make_unique<json::JSON>();
}

LocalizedString::LocalizedString(TABLE table, std::string textKey)
{
	this->table = table;

	SetKey(textKey);
	state = std::make_unique<json::JSON>();
}

std::string LocalizedString::Format()
{
	if (key == "") return "Error: Missing LocalizationKey";

	auto raw = Localizer::Localize(key, table);

	Colorize(raw, Localizer::Localize("Player", TABLE::CHARACTERS), "#42f578");
	Colorize(raw, Localizer::Localize("GenericEnemy", TABLE::CHARACTERS), "#ff2929");

	for(int i = 0;i < formatValues.size(); i++)
	{
		ReplaceAll(raw, "{" + std::to_string(i) + "}", formatValues[i]->Format());
	}

	return raw;
}

void LocalizedString::Colorize(std::string& input, std::string toColorize, std::string hexKey)
{
	ReplaceAll(input, toColorize, "<color=" + hexKey + ">" + toColorize + "</color>");
}

