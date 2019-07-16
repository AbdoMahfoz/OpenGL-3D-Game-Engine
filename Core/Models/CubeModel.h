#ifndef CUBEMODEL_CLASS
#define CUBEMODEL_CLASS

#include <GL/glew.h>
#include "../GameObject.h"
#include "../ImportedAssets/texture.h"

class CubeModel : public Model 
{
private:
	static Texture* tex;
	static bool TextureLoaded;
public:
    CubeModel();
};
#endif