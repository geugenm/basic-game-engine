#include "render/shapes/test.hxx"
#include "render/textures/ppm_handler.hxx"

int main()
{
    const ColorRGB black = {0, 0, 0};

    constexpr size_t width = 320;
    constexpr size_t height = 240;

    Texture image;
    image.set_shape(BoundingBox(600, 600));

    triangle_interpolated interpolated_render(image);

    struct program : gfx_program
    {
        void set_uniforms(const uniforms &) override
        {
        }
        vertex vertex_shader(const vertex &v_in) override
        {
            vertex out = v_in;

            // rotate
            double alpha = 3.14159 / 6; // 30 degree
            double x = out.x;
            double y = out.y;
            out.x = x * std::cos(alpha) - y * std::sin(alpha);
            out.y = x * std::sin(alpha) + y * std::cos(alpha);

            // scale into 3 times
            out.x *= 0.3;
            out.y *= 0.3;

            // move
            out.x += (width / 2.0);
            out.y += (height / 2.0);

            return out;
        }
        ColorRGB fragment_shader(const vertex &v_in) override
        {
            ColorRGB out;
            out.red = static_cast<uint8_t>(v_in.f3 * 255);
            out.green = static_cast<uint8_t>(v_in.f4 * 255);
            out.blue = static_cast<uint8_t>(v_in.f5 * 255);
            return out;
        }
    } program01;

    //interpolated_render.clear(black);
    interpolated_render.set_gfx_program(program01);

    std::vector<vertex> triangle_v{
        {0, 0, 1, 0, 0, 0, 0, 0}, {0, 239, 0, 1, 0, 0, 239, 0}, {319, 239, 0, 0, 1, 319, 239, 0}};
    std::vector<uint16_t> indexes_v{0, 1, 2};

    interpolated_render.draw_triangles(triangle_v, indexes_v);

    PpmHandler handler("05_interpolated.ppm", image);
    handler.save();

    // texture example
    struct program_tex : gfx_program
    {
        Texture texture;

        void set_uniforms(const uniforms &) override
        {
        }
        vertex vertex_shader(const vertex &v_in) override
        {
            vertex out = v_in;
            return out;
        }
        ColorRGB fragment_shader(const vertex &v_in) override
        {
            ColorRGB out;

            out.red = static_cast<uint8_t>(v_in.f3 * 255);
            out.green = static_cast<uint8_t>(v_in.f4 * 255);
            out.blue = static_cast<uint8_t>(v_in.f5 * 255);

            ColorRGB from_texture = sample2d(v_in.f5, v_in.f6);
            out.red += from_texture.red;
            out.green = from_texture.green;
            out.blue += from_texture.blue;
            return out;
        }

        void set_texture(const Texture &tex)
        {
            texture = tex;
        }

        ColorRGB sample2d(double u_, double v_)
        {
            auto u = static_cast<int32_t>(std::round(u_));
            auto v = static_cast<int32_t>(std::round(v_));

            ColorRGB c = texture.get_pixel({v, u});
            return c;
        }
    } program02;

    program02.set_texture(image);

    interpolated_render.set_gfx_program(program02);

    interpolated_render.draw_triangles(triangle_v, indexes_v);

    PpmHandler handler1("06_textured_triangle.ppm", image);
    handler1.save();

    return 0;
}
