#include "AutoClean.h"

std::vector<AutoClean*> AutoClean::CleanUpList;
bool AutoClean::CleanUpRegistered = false, AutoClean::IsCleaningUp = false;

void AutoClean::CleanUp()
{
    IsCleaningUp = true;
    for(auto i : CleanUpList)
    {
        delete i;
    }
}
AutoClean::AutoClean()
{
    if(!CleanUpRegistered)
    {
        Engine::RegisterOnExit(CleanUp);
        CleanUpRegistered = true;
    }
    CleanUpList.push_back(this);
}
AutoClean::~AutoClean()
{
    if(!IsCleaningUp)
    {
        for(int i = 0; i < CleanUpList.size(); i++)
        {
            if(CleanUpList[i] == this)
            {
                CleanUpList.erase(CleanUpList.begin() + i);
                return;
            }
        }
    }
}