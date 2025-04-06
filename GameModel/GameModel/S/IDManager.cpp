#include "H/IDManager.h"

int IDManager::counter = 0;

int IDManager::NewID()
{
    counter++;
    return counter;
}
