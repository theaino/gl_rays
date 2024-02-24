#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "buffer.hpp"

class SSBO : public Buffer {
public:
  SSBO();
};
