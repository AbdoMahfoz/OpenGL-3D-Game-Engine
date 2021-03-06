#include "CubeModel.h"

Texture* CubeModel::tex = nullptr;
bool CubeModel::TextureLoaded = false;

CubeModel::CubeModel()
{
	if (!TextureLoaded)
	{
		tex = new Texture("uvtemplate.bmp", 0);
		TextureLoaded = true;
	}
	texture = tex;
	Specularity = 10;
	count = 72;
	indicesCount = 36;
	indices = new unsigned short[indicesCount]
	{
		0 , 1 , 2 , 0 , 2 , 3 , //Front
		4 , 5 , 6 , 4 , 6 , 7 , //Back
		8 , 9 , 10, 8 , 10, 11, //left
		12, 13, 14, 12, 14, 15, //right
		16, 17, 18, 16, 18, 19, //Top
		20, 21, 22, 20, 22, 23, //Bottom
	};
	verts = new float[count]
	{
		//Front
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		//Back
		-1.0f, +1.0f, -1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		//Left
		-1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, +1.0f,
		-1.0f, -1.0f, +1.0f,
		-1.0f, -1.0f, -1.0f,
		//Right
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		+1.0f, -1.0f, -1.0f,
		+1.0f, -1.0f, +1.0f,
		//Top
		-1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, +1.0f,
		+1.0f, +1.0f, -1.0f,
		-1.0f, +1.0f, -1.0f,
		//Bottom
		+1.0f, -1.0f, +1.0f,
		+1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, +1.0f,
	};
	normals = new float[count]
	{
		//Front
		+0.0f, +0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		+0.0f, +0.0f, +1.0f,
		//Back
		+0.0f, +0.0f, -1.0f,
		+0.0f, +0.0f, -1.0f,
		+0.0f, +0.0f, -1.0f,
		+0.0f, +0.0f, -1.0f,
		//Left
		-1.0f, +0.0f, +0.0f,
		-1.0f, +0.0f, +0.0f,
		-1.0f, +0.0f, +0.0f,
		-1.0f, +0.0f, +0.0f,
		//Right
		+1.0f, +0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		//Top
		+0.0f, +1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f,
		//Bottom
		+0.0f, -1.0f, +0.0f,
		+0.0f, -1.0f, +0.0f,
		+0.0f, -1.0f, +0.0f,
		+0.0f, -1.0f, +0.0f,
	};
	uvs = new float[(count/3)*2]
	{
		//Front
		+0.0f, +1.0f,
		+1.0f, +1.0f,
		+1.0f, +0.0f,
		+0.0f, +0.0f,
		//Back
		+1.0f, +1.0f,
		+0.0f, +1.0f,
		+0.0f, +0.0f,
		+1.0f, +0.0f,
		//Left
		+1.0f, +1.0f,
		+0.0f, +1.0f,
		+0.0f, +0.0f,
		+1.0f, +0.0f,
		//Right
		+1.0f, +1.0f,
		+0.0f, +1.0f,
		+0.0f, +0.0f,
		+1.0f, +0.0f,
		//Top
		+0.0f, +0.0f,
		+0.0f, +0.0f,
		+0.0f, +0.0f,
		+0.0f, +0.0f,
		//Bottom
		+0.0f, +0.0f,
		+0.0f, +0.0f,
		+0.0f, +0.0f,
		+0.0f, +0.0f,
	};
}