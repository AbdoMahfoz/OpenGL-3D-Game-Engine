#include "PathFinidng.h"
#include <queue>

std::queue<std::pair<void (*)(glm::vec3*, int), std::pair<glm::vec3, glm::vec3>>> JobQueue;
bool Obstacle[10000][10000];

std::mutex JobQueueMutex, ObstacleMutex;

//------------Helpers-------------------
std::pair<int, int> TranslatePosition(const glm::vec3& position)
{
    return std::pair<int, int>(round(position.x), round(position.z));
}
//--------------------------------------

//-------------Public-------------------
void PathFinidng::RequestPath(glm::vec3 Source, glm::vec3 Destination,
    glm::vec3* OldPath, int OldPathCount, void (*CallBack)(glm::vec3*, int))
{
    ObstacleMutex.lock();
    for(int i = 0; i < OldPathCount; i++)
    {
        UpdateMap(OldPath[i], 1);
    }
    ObstacleMutex.unlock();
    JobQueueMutex.lock();
    JobQueue.push({CallBack, {Source, Destination}});
    JobQueueMutex.unlock();
}
void PathFinidng::UpdateMap(glm::vec3 Position, bool isWalkable)
{
    auto p = TranslatePosition(Position);
    Obstacle[p.first][p.second] = !isWalkable;
}
//---------------------------------------
