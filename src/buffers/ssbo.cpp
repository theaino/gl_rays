// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "ssbo.hpp"

SSBO::SSBO(GLenum usage) : Buffer(GL_SHADER_STORAGE_BUFFER, usage) {}
