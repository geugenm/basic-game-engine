#include "opengl_texture.hxx"

namespace sdk
{

float opengl_texture::get_image_aspect_ratio() const
{
    return static_cast<float>(_width) / static_cast<float>(_height);
}
} // namespace sdk
