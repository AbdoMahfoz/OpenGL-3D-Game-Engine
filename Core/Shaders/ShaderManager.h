#include <GL/glew.h>

namespace ShaderManager
{
    GLuint GetShaders(const char* VertexShader, const char* FragmentShader);
    void UnloadShader(const char* VertexShader, const char* FragmentShader);
}