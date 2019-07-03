#include "Zombie.h"
#include "../PathFinding/PathFinidng.h"

std::mutex Zombie::OrderQueueMutex;
std::vector<Zombie*> Zombie::Instances;

void Zombie::PathCallback(glm::vec3* Path, int Count, Zombie* requester)
{
    requester->ReceivePath(Path, Count);
}
void Zombie::RequestPath(const glm::vec3& Start, const glm::vec3& Finish, 
    glm::vec3* oldPath, int oldPathCount, Zombie* self)
{
    PathFinidng::RequestPath(Start, Finish, oldPath, oldPathCount, PathCallback, self);
}
void Zombie::ZombieRoutine()
{
    for(auto i : Instances)
    {
        i->Main();
    }
}
Zombie::Zombie(const glm::vec3& InitialPos, GameObject* Target)
{
	this->LastDirection = glm::vec3(0);
	this->refreshDebug = false;
    this->debugPath = false;
    this->PathRequested = false;
    this->LastPos = glm::vec3(-999999, -9999999, -9999999);
    this->Path = nullptr;
    this->PathCount = 0;
    this->PathIndex = 0;
    this->Target = Target;
    Translate(InitialPos);
    Instances.push_back(this);
    if(Instances.size() == 1)
    {
        Engine::RegisterRoutine(ZombieRoutine, false);
    }
}
void Zombie::SetDebugging(bool val)
{
    if(debugPath && !val)
    {
        for(auto i : PathDebug)
        {
            delete i;
        }
        PathDebug.clear();
    }
    this->debugPath = val;
}
glm::vec3 Zombie::Round(const glm::vec3& v)
{
    return glm::vec3((int)round(v.x), (int)round(v.y), (int)round(v.z));
}
void Zombie::Main()
{
    glm::vec3 pos = Round(Target->GetPosition());
	if (refreshDebug)
	{
		refreshDebug = false;
		for (auto j : PathDebug)
		{
			delete j;
		}
		PathDebug.clear();
		for (int j = 0; j < PathCount; j++)
		{
			Cube* c = new Cube();
			c->Translate(Path[j]);
			c->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
			PathDebug.push_back(c);
		}
	}
    if(!PathRequested && pos != LastPos)
    {
        LastPos = pos;
        PathRequested = true;
        RequestPath(GetPosition(), Target->GetPosition(), Path, PathCount, this);
    }
    if(Path != nullptr)
    {
        Walk();
    }
}
void Zombie::Walk()
{
    glm::vec3 pos = Round(GetPosition());
    while(pos == Path[PathIndex] && PathIndex < PathCount - 1)
    {
        PathIndex++;
    }
    glm::vec3 Direction;
    if(pos == Path[PathIndex])
    {
        Direction = Target->GetPosition() - GetPosition();
    }
    else
    {
        Direction = Path[PathIndex] - pos;
    }
	Direction = (Direction * 0.2f) + (LastDirection * 0.8f);
	LastDirection = Direction;
	Direction = glm::normalize(Direction);
	Translate(Direction * 0.08f);
	ModelMatrix = glm::inverse(glm::lookAt(GetPosition(), GetPosition() + Direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	dirty = true;
}
void Zombie::ReceivePath(glm::vec3* Path, int Count)
{
    if(this->Path != nullptr && Path != nullptr)
    {
        double leastValue = glm::length(Path[0] - this->Path[PathIndex]);
        int leastIndex = 0;
        for(int i = 1; i < Count; i++)
        {
            double value = glm::length(Path[i] - this->Path[PathIndex]);
            if(value <= leastValue)
            {
                leastIndex = i;
                leastValue = value;
            }
        }
        PathIndex = leastIndex;
    }
    else
    {
        PathIndex = 0;
    }
    delete[] this->Path;
    this->Path = Path;
    this->PathCount = Count;
    if(debugPath)
    {
		refreshDebug = true;
    }
    PathRequested = false;
}
Zombie::~Zombie()
{
    delete[] this->Path;
    for(int i = 0; i < Instances.size(); i++)
    {
        if(Instances[i] == this)
        {
            Instances.erase(Instances.begin() + i);
            break;
        }
    }
    if(Instances.size() == 0)
    {
        Engine::UnRegisterRoutine(ZombieRoutine);
    }
    for(auto i : PathDebug)
    {
        delete i;
    }
}