#include "../../../Core/Engine.h"
#include <queue>

#ifndef ZOMBIE_CLASS
#define ZOMBIE_CLASS

class Zombie : public Cube
{
private:
    static std::mutex OrderQueueMutex;
    static std::vector<Zombie*> Instances;
    static std::queue<Zombie*> OrderQueue;
    static void ZombieRoutine();                            
    static void PathCallback(glm::vec3* Path, int Count);
    static void RequestPath(const glm::vec3& Start, const glm::vec3& Finish, 
                            glm::vec3* oldPath, int oldPathCount, Zombie* self);
    std::vector<Cube*> PathDebug;
    glm::vec3* Path;
    glm::vec3 LastPos;
    GameObject* Target;
    bool PathRequested, debugPath;
    double targetAngle;
    int PathCount, PathIndex;
    glm::vec3 Round(const glm::vec3&);
    void Main();
    void Walk();
    void ReceivePath(glm::vec3* Path, int Count);
public:
    Zombie(const glm::vec3& InitialPosition, GameObject* Target);
    void SetDebugging(bool);
    ~Zombie();
};

#endif