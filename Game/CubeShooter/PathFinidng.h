#include "Engine.h"

namespace PathFinidng
{
    void RequestPath(glm::vec3 Source, glm::vec3 Destination,
        glm::vec3* OldPath, int OldPathCount, void (*CallBack)(glm::vec3*, int));
    void TerminateWokrer();
    void UpdateMap(glm::vec3 Position, bool isWalkable);
}