#ifndef CUBEMODEL_CLASS
#define CUBEMODEL_CLASS

#include <GL/glew.h>
#include "GameObject.h"
#include "texture.h"

class CubeModel : public Model 
{
public:
    CubeModel(Texture* texture);
    void Draw(GameObject&);
};
#endif