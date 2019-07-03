#include "../../../Core/Engine.h"
#include "../Actor/Zombie.h"

namespace PathFinidng
{
    void RequestPath(glm::vec3 Source, glm::vec3 Destination,
        glm::vec3* OldPath, int OldPathCount, void (*CallBack)(glm::vec3*, int, Zombie*), Zombie* requester);
    void TerminateWokrer();
    void UpdateMap(glm::vec3 Position, bool isWalkable);
}