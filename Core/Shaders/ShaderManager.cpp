#include "ShaderManager.h"
#include "ImportedAssets/shader.hpp"
#include <map>
#include <string>

std::map<std::pair<std::string, std::string>, GLuint> LoadedShaders;

GLuint ShaderManager::GetShaders(const char* vertex, const char* fragment)
{
    auto itr = LoadedShaders.find({vertex, fragment});
    if(itr != LoadedShaders.end())
    {
        return itr->second;
    }
    return LoadedShaders[{vertex, fragment}] = LoadShaders(vertex, fragment);
}
void ShaderManager::UnloadShader(const char* vertex, const char* fragment)
{
    auto itr = LoadedShaders.find({vertex, fragment});
    if(itr == LoadedShaders.end())
    {
        return;
    }
    glDeleteProgram(itr->second);
    LoadedShaders.erase(itr);
}
