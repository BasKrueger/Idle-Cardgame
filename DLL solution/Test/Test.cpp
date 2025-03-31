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
    std::string path = "C:/Users/Bastian_Krueger/Desktop/Repositories/Idle-Cardgame/Unity Project/Assets/Localization/Resources/";

    auto card = ReadFile(path + "Cards.csv");
    auto log = ReadFile(path + "AdventureLog.csv");
    auto character = ReadFile(path + "Characters.csv");

    SetLanguage(&language[0], &card[0], &log[0], &character[0]);
}

void Skip()
{
    const int loopCount = 25;

    double anArray[loopCount];
    auto totalStart = std::chrono::system_clock::now();

    std::cout << "Simulating a whole day " << loopCount << " times...\n\n";

    for (int loop = 0; loop < loopCount; loop++)
    {
        Initialize();
        Localize("English(en)");

        auto start = std::chrono::system_clock::now();
        auto restSeconds = Skip(86400);
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed_seconds = end - start;
        anArray[loop] = elapsed_seconds.count();

        if (loop < 9) 
        {
            std::cout << loop + 1 << ".  Day | Elapsed time: " << anArray[loop] << "\n";
        }
        else 
        {
            std::cout << loop + 1 << ". Day | Elapsed time: " << anArray[loop] << "\n";
        }
    }

    std::cout << "\n";

    float sum = 0;
    for (int i = 1; i < loopCount; i++) {
        sum += anArray[i];
    }

    std::cout << "Average simulation duration: " << sum / (loopCount - 1) << "\n";

    auto totalEnd = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = totalEnd - totalStart;
    std::cout << "Simulating a whole day " << loopCount << " times took " << elapsed_seconds.count() << " seconds \n";
}

#include "Json.hpp"
#include "csv.h"
#include <fstream>

int main()
{
    //Dump();
    //Skip();

    //Initialize();
    //for(int i = 0;i < 30;i++)
    //{
        //Tick();
    //}
    //GetSaveState(0,0);

    Skip();

    Initialize();
    SetSaveState(&ReadFile("C:/Users/Bastian_Krueger/AppData/LocalLow/DefaultCompany/Idle Roguelike/Savestate.sv")[0]);
    Skip(8000);

    //std::cout << "\n\n\n\n--------\n";

    //auto sa = json::JSON::Load(s);
    //std::cout << sa;

    

    //Skip();
    //Initialize();
    
    //Initialize();
    //Tick();

    //Dump();
    //GetSaveState(0, 0);

    /*

    //Localize("German (Germany)(de-DE)");
    // Localize("English(en)");

    //Initialize();
    //Skip();
    //GenerateGameState();
    */

    return 0;
}