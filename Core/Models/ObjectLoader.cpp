#include "ObjectLoader.h"

Coordinate::Coordinate(float a, float b, float c)
{
    x = a;
    y = b;
    z = c;
}

Face::Face(int f1, int f2, int f3, int t1, int t2, int t3)
{
    Faces[0] = f1;
    Faces[1] = f2;
    Faces[2] = f3;
    TextureCoordinates[0] = t1;
    TextureCoordinates[1] = t2;
    TextureCoordinates[2] = t3;
}
Face::Face(int f1, int f2, int f3, int t1, int t2, int t3, int n1, int n2, int n3)
{
    Faces[0] = f1;
    Faces[1] = f2;
    Faces[2] = f3;
    TextureCoordinates[0] = t1;
    TextureCoordinates[1] = t2;
    TextureCoordinates[2] = t3;
    Normals[0] = n1;
    Normals[1] = n2;
    Normals[2] = n3;
}
TextureCoordinate::TextureCoordinate(float a, float b)
{
    u = a;
    v = b;
}

int ObjectLoader::LoadModel(const char *filename, int type)
{
    std::ifstream in(filename); //open the model file
    if (!in.is_open())
    {
        std::cout << "Can't Open the File" << std::endl; //if it's not opened then error message, and return with -1
        return -1;
    }
    char LineBuffer[256];
    // Reads the file and store all of its lines in FileLines Vector
    while (!in.eof())
    {
        in.getline(LineBuffer, 256);
        FileLines.push_back(new std::string(LineBuffer));
    }

    for (int q = 0; q < FileLines.size(); q++)
    {
        if ((*FileLines[q])[0] == '#') //if it's a comment
            continue;
        else if ((*FileLines[q])[0] == 'v' && (*FileLines[q])[1] == ' ')
        {
            float tmpx, tmpy, tmpz;
            sscanf(FileLines[q]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz); //read the 3 floats, which makes up the vertex
            Verts.push_back(Coordinate(tmpx, tmpy, tmpz));                    //and put it in the vertex vector
        }
        else if ((*FileLines[q])[0] == 'v' && (*FileLines[q])[1] == 'n')
        {
            float tmpx, tmpy, tmpz;
            sscanf(FileLines[q]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
            Normals.push_back(Coordinate(tmpx, tmpy, tmpz)); //basically do the same
            IsNormal = true;
        }
        else if ((*FileLines[q])[0] == 'v' && (*FileLines[q])[1] == 't') //texture coorinate
        {
            float u, v;
            sscanf(FileLines[q]->c_str(), "vt %f %f", &u, &v);         //read the uv coordinate
            TextureCoordinates.push_back(TextureCoordinate(u, 1 - v)); //I push back 1-v instead of normal v, because obj file use the upper left corner as 0,0 coorinate
            //but OpenGL use bottom left corner as 0,0, so I convert it
            IsTexture = true;
        }
        else if ((*FileLines[q])[0] == 'f')
        {
            int a, b, c;
            int t[3];
            if (FileLines[q]->find("/") != std::string::npos)
            {
                if (type == 3)
                {
                    int n[3];
                    sscanf(FileLines[q]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t[0],&n[0], &b, &t[1],&n[1], &c, &t[2],&n[2]);
                    Faces.push_back(Face(a, b, c, t[0], t[1], t[2],n[0],n[1],n[2]));
                }
                else
                {
                    sscanf(FileLines[q]->c_str(), "f %d/%d %d/%d %d/%d", &a, &t[0], &b, &t[1], &c, &t[2]);
                    Faces.push_back(Face(a, b, c, t[0], t[1], t[2]));
                }
            }
        }
    }
    return 0;
}
// unsigned int ObjectLoader::LoadTexture(const char *FileName)
// {
//     unsigned int num;
//     glGenTextures(1, &num);
//     SDL_Surface *img = SDL_LoadBMP(filename);
//     glBindTexture(GL_TEXTURE_2D, num);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, img->pixels);
//     glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE); //maybe just this
//     SDL_FreeSurface(img);
//     texture.push_back(num);
//     return num;
// }