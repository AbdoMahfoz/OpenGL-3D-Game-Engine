#include "CubeModel.h"

#ifndef CUBE_CLASS
#define CUBE_CLASS

class Cube : public GameObject
{
private:
    static CubeModel* CommonModel;
    static Texture* CommonTexture;
    static Model* GetModel();
    static Texture* GetTexture();
protected:
	virtual ~Cube() {};
public:
    Cube() : GameObject(GetModel(), GetTexture()) { }
};

#endif