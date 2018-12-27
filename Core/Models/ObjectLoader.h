#ifndef OBJECTLOADER_PACKAGE
#define OBJECTLOADER_PACKAGE

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <iostream>

struct Coordinate
{
    float x, y, z;
    Coordinate(float a, float b, float c);
};

struct Face
{
    bool Four;
    int Faces[3];
    int TextureCoordinates[3];
    Face(int f1, int f2, int f3, int t1, int t2, int t3);
};

struct TextureCoordinate
{
    float u, v;
    TextureCoordinate(float a, float b);
};

class ObjectLoader
{
public:
    std::vector<std::string *> FileLines;
    std::vector<Coordinate *> Verts;
    std::vector<Face *> Faces;
    std::vector<Coordinate *> Normals;
    std::vector<unsigned int> Textures;
    std::vector<unsigned int> Lists;
    std::vector<TextureCoordinate *> TextureCoordinates; //all texture coorinate (UV coordinate)
    bool  IsNormal, IsTexture;
    unsigned int LoadTexture(const char *FileName);
    void Clean();
    int LoadModel(const char *FileName);
};

#endif