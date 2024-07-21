//
// Created by Stefan on 7/21/2024.
//

#include "gfx/image.hpp"

namespace Viewer {

Image::Image(int32_t width, int32_t height, const void* data)
    : m_Width(width), m_Height(height) {
  glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (data != nullptr) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

Image::~Image() { glDeleteTextures(1, &m_ID); }

void Image::SetData(const void* data) const {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Image::Resize(int32_t width, int32_t height) {
  m_Width = width;
  m_Height = height;
}

}  // namespace Viewer
