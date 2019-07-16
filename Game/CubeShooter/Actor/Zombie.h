#include "../../../Core/Engine.h"
#include <queue>

#ifndef ZOMBIE_CLASS
#define ZOMBIE_CLASS

class Zombie : public Cube
{
private:
    static std::vector<Zombie*> Instances;
	static std::set<Zombie*> InstanceSet;
    static void ZombieRoutine();                            
    static void PathCallback(glm::vec3* Path, int Count, Zombie* requester);
    static void RequestPath(const glm::vec3& Start, const glm::vec3& Finish, 
                            glm::vec3* oldPath, int oldPathCount, Zombie* self);
    std::vector<Cube*> PathDebug;
    glm::vec3* Path;
    glm::vec3 LastPos, debugColor;
    GameObject* Target;
	glm::vec3 LastDirection;
    bool PathRequested, debugPath, refreshDebug;
    int PathCount, PathIndex;
    glm::vec3 Round(const glm::vec3&);
    void Main();
    void Walk();
    void ReceivePath(glm::vec3* Path, int Count);
protected:
	virtual ~Zombie() {}
public:
    Zombie(const glm::vec3& InitialPosition, GameObject* Target, glm::vec3 debugColor = glm::vec3(0.0f, 1.0f, 0.0f));
    void SetDebugging(bool);
	void Delete();
};

#endif