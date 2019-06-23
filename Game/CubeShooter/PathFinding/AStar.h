#include "../../../Core/Engine.h"

namespace AStar
{
    std::vector<std::pair<int, int>>* CalculatePath(
        bool Obstacles[][10000], std::pair<int, int> Start, std::pair<int, int> Finish);
}