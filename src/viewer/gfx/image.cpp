//
// Created by Stefan on 7/21/2024.
//

#include "gfx/image.hpp"

namespace Viewer {

Image::Image(int32_t width, int32_t height, const void* data)
    : m_Width(width), m_Height(height) {
  glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

  glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (data != nullptr) {
    glTextureSubImage2D(
        m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
}

Image::~Image() { glDeleteTextures(1, &m_ID); }

void Image::SetData(const uint32_t* data) const {
  glTextureSubImage2D(
      m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Image::Resize(int32_t width, int32_t height) {
  m_Width = width;
  m_Height = height;

  glDeleteTextures(1, &m_ID);
  glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
  glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);

  glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

}  // namespace Viewer
