#ifndef BLENDEROBJECT_CLASS
#define BLENDEROBJECT_CLASS

#include <GL/glew.h>
#include "Model.h"
#include "texture.h"

class BlenderModel : public Model 
{
public:
    BlenderModel(const char* Path);
};
#endif