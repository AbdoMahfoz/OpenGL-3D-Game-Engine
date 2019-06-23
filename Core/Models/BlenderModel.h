#ifndef BLENDEROBJECT_CLASS
#define BLENDEROBJECT_CLASS

#include <GL/glew.h>
#include "Model.h"
#ifdef _WIN32
	#include "../ImportedAssets/texture.h"
#else
	#include "texture.h"
#endif

class BlenderModel : public Model 
{
public:
    BlenderModel(const char* Path, int type);
};
#endif