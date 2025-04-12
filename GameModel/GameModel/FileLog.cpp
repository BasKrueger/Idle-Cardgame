#include "FileLog.h"
#include "fstream"

void FileLog::Add(std::string content)
{
	std::ofstream file;
	file.open("C:/Users/Bastian_Krueger/AppData/LocalLow/DefaultCompany/Idle Roguelike/TestLog.txt", std::ios::app); // Append mode: adds to the end of the file
	if (file.is_open())
	{
		file << content << std::endl;
		file.close();
	}
}
