#include "AStar.h"
#include "Heap.cpp"
#include <map>

struct AStarItem
{
public:
    AStarItem(){}
    AStarItem(std::pair<int, int> pos, double cost, double distance, AStarItem* Parent)
    {
        this->pos = pos;
        this->cost = cost;
        this->distance = distance;
        this->Parent = Parent;
    }
    std::pair<int, int> pos;
    AStarItem* Parent;
    double cost;
    double distance;
    inline double total() const { return cost + distance; }
    bool operator<(const AStarItem& other){ return total() < other.total(); }
    bool operator>(const AStarItem& other){ return total() > other.total(); }
    bool operator<=(const AStarItem& other){ return total() <= other.total(); }
    bool operator>=(const AStarItem& other){ return total() >= other.total(); }
    bool operator==(const AStarItem& other){ return total() == other.total(); }
};

double AbsDiff(const std::pair<int, int>& l, const std::pair<int, int>& r)
{
    return sqrt((l.second - r.second) * (l.second - r.second) + (l.first - r.first) * (l.first - r.first));
}

std::vector<std::pair<int, int>>* AStar::CalculatePath(
    bool Obstacles[][10000], std::pair<int, int> Start, std::pair<int, int> Finish)
{
    int minX = std::min(Start.first, Finish.first) - 10;
    int maxX = std::max(Start.first, Finish.first) + 10;
    int minY = std::min(Start.second, Finish.second) - 10;
    int maxY = std::max(Start.second, Finish.second) + 10;
    Heap<AStarItem> h;
    std::map<std::pair<int, int>, HeapItem<AStarItem>*> Grid;
    std::set<std::pair<int, int>> Visited;
    Grid[{Start.first, Start.second}] = h.push(
        AStarItem({Start.first, Start.second}, 0, AbsDiff(Start, Finish), nullptr));
    while(h.size())
    {
        auto val = h.pop();
        if(val->pos == Finish)
        {
            break;
        }
        if(Visited.find({val->pos.first, val->pos.second}) != Visited.end())
        {
            continue;
        }
        Visited.insert({val->pos.first, val->pos.second});
        for(int x = -1; x <= 1; x++)
        {
            for(int y = -1; y <= 1; y++)
            {
                std::pair<int, int> targetPos = {val->pos.first + x, val->pos.second + y};
                if((x == 0 && y == 0) ||
                   targetPos.first < minX || targetPos.first > maxX ||
                   targetPos.second < minY || targetPos.second > maxY ||
                   Obstacles[targetPos.first][targetPos.second] || 
                   Visited.find(targetPos) != Visited.end())
                {
                    continue;
                }
                double coeff = ((x != 0 && y != 0) ? 1.25 : 1);
                if(coeff == 1.25 && (Obstacles[val->pos.first][val->pos.second + y] || 
                                    Obstacles[val->pos.first + x][val->pos.second]))
                {
                    continue;
                }
                auto& neighbour = Grid[{targetPos.first, targetPos.second}];
                if(neighbour == 0)
                {
                    neighbour = h.push(AStarItem(targetPos, val->cost + coeff, AbsDiff(targetPos, Finish), val));
                }
                else if(neighbour->val.cost >= val->cost + coeff)
                {
                    neighbour->val.cost = val->cost + coeff;
                    neighbour->val.Parent = val;
                    h.update(neighbour);
                }
            }
        }
    }
    auto itr = &Grid[{Finish.first, Finish.second}]->val;
    if(itr == 0)
    {
        return nullptr;
    }
    std::vector<std::pair<int, int>>* res = new std::vector<std::pair<int, int>>();
    while(itr != nullptr)
    {
        res->push_back(itr->pos);
        itr = itr->Parent;
    }
    return res;
}