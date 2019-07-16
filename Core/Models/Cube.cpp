#include "Cube.h"

CubeModel* Cube::CommonModel = nullptr;
Model* Cube::GetModel()
{
    if(CommonModel == nullptr)
    {
        CommonModel = new CubeModel();
    }
    return CommonModel;
}