#include "Engine.h"
#include <math.h>


bool Collision::IsCollied(const GameObject &Object_A, const GameObject &Object_B)
{
    //Get Position Vector of The First Object..
     glm::vec3 Position_A = Object_A.GetPosition();

     //Get Scale Vector of The First Object..
     glm::vec3 Scale_A = Object_A.GetScale();

    //Get Position Vector of The Second Object..
     glm::vec3 Position_B = Object_B.GetPosition();

     //Get Scale Vector of The Second Object..
     glm::vec3 Scale_B = Object_B.GetScale();


    //Apply AABB Algorithm (Axis Aligned Bounding Box)..
    return (   abs(Position_A.x-Position_B.x)<Scale_A.x+Scale_B.x
            && abs(Position_A.y-Position_B.y)<Scale_A.y+Scale_B.y 
            && abs(Position_A.z-Position_B.z)<Scale_A.z+Scale_B.z );
}