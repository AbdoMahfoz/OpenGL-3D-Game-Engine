#include "CubeModel.h"

float Tverts[120] =
{
	//front
	-1.0f, -1.0f, 1.0f, //Vertex 0
	0.0f, 0.0f,//uv
	+1.0f, -1.0f, 1.0f,  //v1
	+1.0f, 0.0f,
	+1.0f, +1.0f, 1.0f,   //v3
	+1.0f, +1.0f,
	-1.0f, +1.0f, 1.0f,  //v2
		0.0f, +1.0f,
	//right
	1.0f, -1.0f, 1.0f,  //...
	0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,
	+1.0f, 0.0f,
	1.0f, 1.0f, -1.0f,
	+1.0f, +1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, +1.0f,
	//back
	1.0f, -1.0f, -1.0f,
	0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,
	+1.0f, 0.0f,
	-1.0f, 1.0f, -1.0f,
	+1.0f, +1.0f,
	1.0f, 1.0f, -1.0f,
	0.0f, +1.0f,
	//left
	-1.0f, -1.0f, -1.0f,
	+1.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,
	0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,
	0.0f, +1.0f,
	-1.0f, 1.0f, -1.0f,
	+1.0f, +1.0f,
	//bottom
	-1.0f, -1.0f, -1.0f,
	+1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,
	+1.0f, +1.0f,
	1.0f, -1.0f, 1.0f,
	0.0f, +1.0f,
	-1.0f, -1.0f, 1.0f,
	0.0f, 0.0f,
	//top
	-1.0f, 1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, +1.0f,
	1.0f, 1.0f, -1.0f,	
	+1.0f, +1.0f,
	-1.0f, 1.0f, -1.0f,
	+1.0f, 0.0f,
};
CubeModel::CubeModel(Texture* texture) : Model(texture, Tverts, 120)
{
}
void CubeModel::Draw(GameObject& obj)
{
    glUniform3fv(colorID, 1, &(obj.GetColor()[0]));
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &(MVPMatrix * obj.GetModelMatrix())[0][0]);
    for(int i = 0; i < count; i += 4)
    {
        glDrawArrays(GL_TRIANGLE_FAN,i,4);
    }
}