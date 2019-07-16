#ifndef BLENDEROBJECT_CLASS
#define BLENDEROBJECT_CLASS

#include <GL/glew.h>
#include "Model.h"
#include "../ImportedAssets/texture.h"

class BlenderModel : public Model 
{
public:
    BlenderModel(const char* Path, int type);
};
#endif