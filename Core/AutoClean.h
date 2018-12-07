#ifndef AUTOCLEAN_CLASS
#define AUTOCLEAN_CLASS

#include <vector>

class AutoClean
{
private:
    static std::vector<AutoClean*> CleanUpList;
    static void CleanUp();
    static bool CleanUpRegistered, IsCleaningUp;
public:
    AutoClean();
    virtual ~AutoClean();
};

#endif