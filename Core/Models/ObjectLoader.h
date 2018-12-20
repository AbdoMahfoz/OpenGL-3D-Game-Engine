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
    int FaceNum;
    bool Four;
    int Faces[4];
    int TextureCoordinates[4];
    Face(int facenum, int f1, int f2, int f3, int t1, int t2, int t3);
    Face(int facenum, int f1, int f2, int f3, int f4, int t1, int t2, int t3, int t4);
};

struct TextureCoordinate
{
    float u, v;
    TextureCoordinate(float a, float b);
};

class ObjectLoader
{
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

  public:
    ObjectLoader();
    ~ObjectLoader();
    int  LoadModel(const char *FileName);
};