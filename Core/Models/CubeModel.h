#ifndef CUBEMODEL_CLASS
#define CUBEMODEL_CLASS

#include <GL/glew.h>
#include "../GameObject.h"
#ifdef _WIN32
	#include "../ImportedAssets/texture.h"
#else
	#include "texture.h"
#endif	

class CubeModel : public Model 
{
public:
    CubeModel();
};
#endif