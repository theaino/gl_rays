// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "ssbo.hpp"

SSBO::SSBO() : Buffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_READ) {}
