// #include "pch.h" Uncomment for Visual Studio 2017 and earlier
#include <iostream>
#include <fstream>
#include "GameModelLib.h"
#include "Chrono"
#include "json.hpp"

std::string ReadFile(std::string path)
{
    std::ifstream file(path);
    std::string result;

    for (std::string line; std::getline(file, line); )
    {
        result += line + "\n";
    } 

    return result;
}

void Localize(std::string language) 
{
    std::string path = "C:/Users/Bastian Krueger/Desktop/Idle-Cardgame/Unity Project/Assets/Localization/Resources/";

    auto card = ReadFile(path + "Cards.csv");
    auto log = ReadFile(path + "AdventureLog.csv");
    auto character = ReadFile(path + "Characters.csv");

    SetLanguage(&language[0], &card[0], &log[0], &character[0]);
}

void Skip() 
{
    const int loopCount = 50;

    double anArray[loopCount];
    auto totalStart = std::chrono::system_clock::now();

    std::cout << "Simulating a whole day " << loopCount << " times...\n";

    for (int loop = 0; loop < loopCount; loop++)
    {
        Initialize();
        Localize("English(en)");

        auto start = std::chrono::system_clock::now();
        auto restSeconds = Skip(86400);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        anArray[loop] = elapsed_seconds.count();

        std::cout << loop + 1 << ". Loop | Elapsed time: " << anArray[loop] << " | seconds left: " << restSeconds << "\n";
    }

    std::cout << "\n";

    float sum = 0;
    for (int i = 1; i < loopCount; i++) {
        sum += anArray[i];
    }

    std::cout << "Average simulation duration: " << sum / loopCount << "\n";

    auto totalEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = totalEnd - totalStart;
    std::cout << "Simulating a whole day " << loopCount << " times took " << elapsed_seconds.count() << " seconds \n";
}

#include "Json.hpp"
#include "csv.h"
#include <fstream>

int main()
{
    Skip();
    Tick();
    Tick();
    Tick();
    Tick();
    Dump();

    /*

    //Localize("German (Germany)(de-DE)");
    // Localize("English(en)");

    //Initialize();
    //Skip();
    //GenerateGameState();
    */

    return 0;
}