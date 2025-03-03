#pragma once
#include <iostream>
#include <vector>
#include "Json.hpp"

class LocalizedString;

class LocalizedStringFormatVariable 
{
public:
	LocalizedStringFormatVariable();
	LocalizedStringFormatVariable(std::string staticValue);
	LocalizedStringFormatVariable(LocalizedString* localizedValue);
	LocalizedStringFormatVariable(int* dynamicVariable);

	std::string Format();

private:
	std::string staticVariable;
	LocalizedString* localizedVariable = 0;
	int* dynamicVariable = 0;

	std::unique_ptr<json::JSON> state;
};

class LocalizedString
{
public: 
	enum TABLE { ADVENTURE_LOG, CHARACTERS, CARDS, MENUS };
	
	LocalizedString();
	LocalizedString(TABLE table);
	LocalizedString(TABLE table, std::string textKey);

	void SetKey(std::string textKey);
	void SetTable(TABLE table);
	void BindFormatVariable(std::string staticValue);
	void BindFormatVariable(LocalizedString* localizedValue);
	void BindFormatVariable(std::unique_ptr<LocalizedString>* localizedValue);
	void BindFormatVariable(int* dynamicValue);
	void UnbindAll();

	std::string Format();

private:
	void Colorize(std::string& input, std::string toColorize, std::string hexKey);

	TABLE table;
	std::string key;
	std::vector<std::unique_ptr<LocalizedStringFormatVariable>> formatValues;

	std::unique_ptr<json::JSON> state = 0;
};

