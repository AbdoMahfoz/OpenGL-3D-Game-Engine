#include "PathFinidng.h"
#include <queue>

std::queue<std::pair<void (*)(glm::vec3*, int), std::pair<glm::vec3, glm::vec3>>> JobQueue;
bool Obstacle[10000][10000];

std::mutex JobQueueMutex, ObstacleMutex;

//------------Helpers-------------------
std::pair<int, int> TranslatePosition(const glm::vec3& position)
{
    return std::pair<int, int>(round(position.x) + 5000, round(position.z) + 5000);
}
//--------------------------------------

//-------------Public-------------------
void PathFinidng::RequestPath(glm::vec3 Source, glm::vec3 Destination,
    glm::vec3* OldPath, int OldPathCount, void (*CallBack)(glm::vec3*, int))
{
    for(int i = 0; i < OldPathCount; i++)
    {
        UpdateMap(OldPath[i], 1);
    }
    JobQueueMutex.lock();
    JobQueue.push({CallBack, {Source, Destination}});
    JobQueueMutex.unlock();
}
void PathFinidng::UpdateMap(glm::vec3 Position, bool isWalkable)
{
    ObstacleMutex.lock();
    auto p = TranslatePosition(Position);
    Obstacle[p.first][p.second] = !isWalkable;
    ObstacleMutex.unlock();
}
//---------------------------------------
