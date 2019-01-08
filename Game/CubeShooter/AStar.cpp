#include "AStar.h"
#include "Heap.cpp"

struct AStarItem
{
public:
    AStarItem(std::pair<int, int> pos, int cost, double distance, AStarItem* Parent)
    {
        this->pos = pos;
        this->cost = cost;
        this->distance = distance;
        this->Parent = Parent;
    }
    std::pair<int, int> pos;
    AStarItem* Parent;
    int cost;
    double distance;
    inline double total() const { return cost + distance; }
    bool operator<(const AStarItem& other){ return total() < other.total(); }
    bool operator>(const AStarItem& other){ return total() > other.total(); }
    bool operator<=(const AStarItem& other){ return total() <= other.total(); }
    bool operator>=(const AStarItem& other){ return total() >= other.total(); }
    bool operator==(const AStarItem& other){ return total() == other.total(); }
};

HeapItem<AStarItem>* Grid[10000][10000];
bool Visited[10000][10000];

double AbsDiff(const std::pair<int, int>& l, const std::pair<int, int>& r)
{
    return sqrt((l.second - r.second) * (l.second - r.second) + (l.first - r.first) * (l.first - r.first));
}

std::vector<std::pair<int, int>>* AStar::CalculatePath(
    bool** Obstacles, std::pair<int, int> Start, std::pair<int, int> Finish)
{
    Heap<AStarItem> h;
    for(int i = 0; i < 10000; i++)
    {
        for(int j = 0; j < 10000; j++)
        {
            Visited[i][j] = 0;
            Grid[i][j] = nullptr;
        }
    }
    Grid[Start.first][Start.second] = h.push(
        AStarItem({Start.first, Start.second}, 0, AbsDiff(Start, Finish), nullptr));
    bool FinishNotFound = true;
    while(h.size() && FinishNotFound)
    {
        auto val = h.pop();
        Visited[val->pos.first][val->pos.second] = 1;
        for(int x = -1; x <= 1 && FinishNotFound; x++)
        {
            for(int y = -1; y <= 1; y++)
            {
                std::pair<int, int> targetPos = {val->pos.first + x, val->pos.second + y};
                if((x == 0 && y == 0) ||
                   targetPos.first < 0 || targetPos.first >= 10000 ||
                   targetPos.second < 0 || targetPos.second >= 10000 ||
                   Visited[targetPos.first][targetPos.second] || Obstacles[targetPos.first][targetPos.second])
                {
                    continue;
                }
                double coeff = ((x != 0 && y != 0) ? 1.5 : 1);
                auto neighbour = Grid[targetPos.first][targetPos.second];
                if(neighbour == nullptr)
                {
                    Grid[targetPos.first][targetPos.second] = h.push(
                        AStarItem(targetPos, val->cost + coeff, AbsDiff(neighbour->val.pos, Finish), val));
                    if(targetPos == Finish)
                    {
                        FinishNotFound = false;
                        break;
                    }
                }
                else if(neighbour->val.cost > val->cost + coeff)
                {
                    neighbour->val.cost = val->cost + coeff;
                    neighbour->val.Parent = val;
                    h.update(neighbour);
                }
            }
        }
    }
    auto itr = &Grid[Finish.first][Finish.second]->val;
    if(itr == nullptr)
    {
        return nullptr;
    }
    std::vector<std::pair<int, int>>* res = new std::vector<std::pair<int, int>>();
    while(itr != nullptr)
    {
        res->push_back(itr->pos);
        itr = itr->Parent;
    }
    std::reverse(res->begin(), res->end());
    return res;
}