#include "../Engine.h"

BlenderModel::BlenderModel(const char* Path,int type)
{
    ObjectLoader o;
    o.LoadModel(Path,type);
	Specularity = 10;
    count = o.Verts.size() * 3;
    indicesCount = o.Faces.size() * 3;
    verts = new float[count];
    normals = new float[count];
    uvs = new float[(count/3)*2];
    indices = new unsigned short[indicesCount];
    for(unsigned int i = 0; i < std::max((unsigned int)(count/3), o.Faces.size()); i++)
    {
        int j = i * 3;
        int k = i * 2;
        if(i < o.Verts.size())
        {
            verts[j]   = o.Verts[i].x;
            verts[j+1] = o.Verts[i].y;
            verts[j+2] = o.Verts[i].z;
        }
        if(i < o.Normals.size())
        {
            normals[j]   = o.Normals[i].x;
            normals[j+1] = o.Normals[i].y;
            normals[j+2] = o.Normals[i].z;
        }
        if(i < o.TextureCoordinates.size())
        {
            uvs[k]   = o.TextureCoordinates[i].u;
            uvs[k+1] = o.TextureCoordinates[i].v;
        }
        if(i < o.Faces.size())
        {
            indices[j]   = o.Faces[i].Faces[0];
            indices[j+1] = o.Faces[i].Faces[1];
            indices[j+2] = o.Faces[i].Faces[2];
        }
    }
}