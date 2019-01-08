#include "Cube.h"

CubeModel* Cube::CommonModel = nullptr;
Texture* Cube::CommonTexture = nullptr;
Model* Cube::GetModel()
{
    if(CommonModel == nullptr)
    {
        CommonModel = new CubeModel();
    }
    return CommonModel;
}
Texture* Cube::GetTexture()
{
    if(CommonTexture == nullptr)
    {
        CommonTexture = new Texture("uvtemplate.bmp", 0);
    }
    return CommonTexture;
}