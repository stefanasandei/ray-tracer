//
// Created by Stefan on 7/21/2024.
//

#include <glad/glad.h>

#include "core/base.hpp"

namespace Viewer {

class Image {
 public:
  Image(int32_t width, int32_t height, const void* data = nullptr);
  ~Image();

  void SetData(const uint32_t* data) const;

  void Resize(int32_t width, int32_t height);

  [[nodiscard]] int32_t GetWidth() const { return m_Width; }
  [[nodiscard]] int32_t GetHeight() const { return m_Height; }

  [[nodiscard]] GLuint GetID() const { return m_ID; }

 private:
  int32_t m_Width = 0, m_Height = 0;
  GLuint m_ID{};
};

}  // namespace Viewer