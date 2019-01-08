#include "PathFinidng.h"
#include "AStar.h"
#include <queue>
#include <condition_variable>

std::queue<std::pair<void (*)(glm::vec3*, int), std::pair<std::pair<glm::vec3*, int>, std::pair<glm::vec3, glm::vec3>>>> JobQueue;
bool Obstacle[10000][10000];
std::thread* WorkerThread = nullptr;
std::mutex JobQueueMutex, ObstacleMutex, SleepMutex;
std::condition_variable cv;
bool RunWorker = false;

//------------Helpers-------------------
std::pair<int, int> TranslatePosition(const glm::vec3& position)
{
    return std::pair<int, int>(round(position.x / 2) + 5000, round(position.z / 2) + 5000);
}
glm::vec3 TranslatePosition(const std::pair<int, int> position)
{
    return glm::vec3((position.first - 5000) * 2, 0, (position.second - 5000) * 2);
}
//--------------------------------------
//-------------Private------------------
void Worker()
{
    std::unique_lock<std::mutex> ul(SleepMutex);
    while(RunWorker)
    {
        JobQueueMutex.lock();
        if(JobQueue.size() == 0)
        {
            JobQueueMutex.unlock();
            cv.wait(ul);
        }
        else
        {
            auto job = JobQueue.front();
            JobQueue.pop();
            JobQueueMutex.unlock();
            if(job.second.first.first != nullptr)
            {
                for(int i = 0; i < job.second.first.second; i++)
                {
                    PathFinidng::UpdateMap(job.second.first.first[i], 1);
                }
            }
            PathFinidng::UpdateMap(job.second.second.first, 1);
            PathFinidng::UpdateMap(job.second.second.second, 1);
            auto res = AStar::CalculatePath(Obstacle, 
                TranslatePosition(job.second.second.first), TranslatePosition(job.second.second.second));
            if(res == nullptr)
            {
                job.first(nullptr, 0);
            }
            else
            {
                int count = res->size();
                glm::vec3* arr = new glm::vec3[count];
                for(int i = 0; i < count; i++)
                {
                    arr[i] = TranslatePosition(res->at(i));
                    PathFinidng::UpdateMap(arr[i], 0);
                }
                delete res;
                job.first(arr, count);
            }
        }
    }
}
//--------------------------------------
//-------------Public-------------------
void PathFinidng::RequestPath(glm::vec3 Source, glm::vec3 Destination,
    glm::vec3* OldPath, int OldPathCount, void (*CallBack)(glm::vec3*, int))
{
    if(WorkerThread == nullptr)
    {
        RunWorker = true;
        WorkerThread = new std::thread(Worker);
        Engine::RegisterOnExit(TerminateWokrer);
    }
    JobQueueMutex.lock();
    JobQueue.push({CallBack, {{OldPath, OldPathCount}, {Source, Destination}}});
    if(JobQueue.size() == 1)
    {
        cv.notify_all();
    }
    JobQueueMutex.unlock();
}
void PathFinidng::TerminateWokrer()
{
    RunWorker = false;
    cv.notify_all();
    WorkerThread->join();
    delete WorkerThread;
    while(JobQueue.size())
    {
        JobQueue.pop();
    }
    Engine::UnRegisterOnExit(TerminateWokrer);
}
void PathFinidng::UpdateMap(glm::vec3 Position, bool isWalkable)
{
    ObstacleMutex.lock();
    auto p = TranslatePosition(Position);
    Obstacle[p.first][p.second] = !isWalkable;
    ObstacleMutex.unlock();
}
//---------------------------------------
