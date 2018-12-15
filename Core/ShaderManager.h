#include <GL/glew.h>

namespace ShaderManager
{
    GLuint GetShaders(const char* VertexShader, const char* FragmentShader);
    void UnLoadShader(const char* VertexShader, const char* FragmentShader);
}