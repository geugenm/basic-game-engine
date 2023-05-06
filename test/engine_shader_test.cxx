#include "opengl_functions.hxx"
#include "shader.hxx"
#include "vertex.hxx"

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <gtest/gtest.h>


TEST(TriangleTest, BasicInterpolation)
{

}


auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
