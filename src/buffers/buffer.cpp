// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "buffer.hpp"

Buffer::Buffer(GLenum type, GLenum usage) {
  this->type = type;
  this->usage = usage;
  glGenBuffers(1, &id);
}
Buffer::~Buffer() { glDeleteBuffers(1, &id); }
void Buffer::bind() { glBindBuffer(type, id); }
void Buffer::bind(GLuint index) { glBindBufferBase(type, index, id); }
void Buffer::unbind() { glBindBuffer(type, 0); }

void Buffer::setData(void *data, size_t size) {
  this->size = size;
  this->bind();
  glBufferData(type, size, data, usage);
}

void Buffer::setSubData(void *data, size_t size, size_t offset) {
  this->bind();
  glBufferSubData(type, offset, size, data);
}

void *Buffer::map() { return glMapBuffer(type, GL_READ_WRITE); }
void Buffer::unmap() { glUnmapBuffer(type); }
GLuint Buffer::getID() { return id; }
GLuint Buffer::getSize() { return size; }
