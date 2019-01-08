#include "Engine.h"

namespace AStar
{
    std::vector<std::pair<int, int>>* CalculatePath(
        bool** Obstacles, std::pair<int, int> Start, std::pair<int, int> Finish);
}