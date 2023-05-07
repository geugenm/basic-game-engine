#include <SDL.h>
#include <glad/glad.h>
#include <gtest/gtest.h>
#include <opengl_functions.hxx>

// Helper function to create a shader program
GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

TEST(ShaderTest, ShaderOutput)
{
    // Initialize SDL and create an OpenGL ES 3.2 context
    GL::init_sdl();
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_Window* window    = GL::create_window("Test", 800, 450);
    SDL_GLContext context = GL::create_opengl_context(window);

    GL::load_opengl_functions();
    GL::is_opengl_version_supported();

    // Set up OpenGL viewport
    glViewport(0, 0, 800, 450);

    // Create a shader program with the provided GLSL code
    // You'll need to provide a simple vertex shader for this test
    const char* vertexShaderSource = R"(
        #version 300 es
        precision mediump float;

        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        void main() {
            gl_Position = vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 300 es
        precision mediump float;

        in vec2 TexCoord;
        out vec4 fragColor;

        uniform vec2 iResolution;
        uniform float iTime;
        uniform vec4 iMouse;

        #define samples 8

        #define pi 3.141592564

        // Scene definition is kinda inspired by .obj file formatting but idk if I'll keep it

        vec3 materials[] = vec3[](
        vec3(1.000,1.000,1.000),    // white
        vec3(1.000,0.067,0.157),    // red
        vec3(0.027,0.945,0.259),    // green
        vec3(0.318,0.553,0.992));   // blue

        vec2 points[] = vec2[](
        vec2(.1,-.25),
        vec2(.3,-.25),
        vec2(.1,-.05),
        vec2(.3,-.05),
        vec2(-.9,-.4),
        vec2(.8,-.4),
        vec2(-.9,-1.),
        vec2(.8,1.),
        vec2(-.4,-.3),
        vec2(-.2,-.3),
        vec2(-.4,-.1),
        vec2(-.2,-.1),
        vec2(-.05,-.05),
        vec2(-.05,-.15),
        vec2(0,-.1),
        vec2(-.1,-.1));

        int segmentCount = 15;
        ivec3 segments[] = ivec3[](
        ivec3(0,1,1),   // ivec3(a,b,c)
        ivec3(0,2,1),   // a = endpoint a index
        ivec3(1,3,1),   // b = endpoint b index
        ivec3(2,3,1),   // c = material index
        ivec3(4,5,0),
        ivec3(4,6,0),
        ivec3(5,7,0),
        ivec3(8,9,3),
        ivec3(8,10,3),
        ivec3(9,11,3),
        ivec3(10,11,3),
        ivec3(12,14,2),
        ivec3(14,13,2),
        ivec3(13,15,2),
        ivec3(15,12,2));

        // https://www.shadertoy.com/view/4djSRW
        vec2 hash21(float p) {
        vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
        p3 += dot(p3, p3.yzx + 33.33);
        return fract((p3.xx+p3.yz)*p3.zy);

        }

        // Interleaved gradient noise
        // https://blog.demofox.org/2022/01/01/interleaved-gradient-noise-a-different-kind-of-low-discrepancy-sequence/
        float IGN(ivec2 p) {
        return mod(52.9829189f * mod(.06711056f*float(p.x) + .00583715f*float(p.y), 1.), 1.);
        }

        // Ray intersection with line segment
        float segmentIntersect(vec2 ro, vec2 rd, vec2 a, vec2 b) {
        vec2 v1 = ro - a;
        vec2 v2 = b - a;
        vec2 v3 = vec2(-rd.y, rd.x);

        float d = dot(v2, v3);
        float t1 = cross(vec3(v2,0), vec3(v1,0)).z / d;
        float t2 = dot(v1, v3) / d;

        if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0)) {
            return t1;
        }
        return 1000.;
        }

        //ray intersection with scene
        //sceneIntersect.w is the distance, sceneIntersect.xyz is the color
        vec4 sceneIntersect(vec2 ro, vec2 rd) {
        float v0 = 1000.;
        vec3 col;

        for(int i=0; i<segmentCount; i++) {

        vec2 a = points[segments[i].x];
        vec2 b = points[segments[i].y];

        float v1 = segmentIntersect(ro, rd, a, b);
        if(v1<v0) {
            col = materials[segments[i].z];
            v0 = v1;
        }
        }
        return vec4(col,v0);
        }

        //line segment SDF
        float line(vec2 p, vec2 a,vec2 b) {
        p -= a, b -= a;
        float h = clamp(dot(p, b) / dot(b, b), 0., 1.);
        return length(p - b * h);
        }

        //scene SDF
        //sceneDist.w is the distance, sceneDist.xyz is the color
        vec4 sceneDist(vec2 p) {
        float v0 = 1000.;
        vec3 col;

        for(int i=0; i<segmentCount; i++) {

        vec2 a = points[segments[i].x];
        vec2 b = points[segments[i].y];

        float v1 = line(p, a, b);
        if(v1<v0) {
            col = materials[segments[i].z];
            v0 = v1;
        }
        }
        return vec4(col,v0);
        }

        vec2 sceneNormal(vec2 p) {
        vec2 epsilon = vec2(.001, -.001);
        return normalize(vec2(sceneDist(p+epsilon.xx).w) - vec2(sceneDist(p-epsilon.xy).w,sceneDist(p-epsilon.yx).w));
        }

        // ACES Tonemapping
        vec3 ACESFilm(vec3 x) {

        float a = 2.51f;
        float b = 0.03f;
        float c = 2.43f;
        float d = 0.59f;
        float e = 0.14f;
        return clamp((x*(a*x + b)) / (x*(c*x + d) + e), 0.0f, 1.0f);
        }

        #define lightFalloff 2.
        void main() {
        vec2 fragCoord = gl_FragCoord.xy;
        vec3 col;
        vec2 p = (2.*fragCoord-iResolution.xy-.5)/iResolution.x;
        float rand = IGN(ivec2(fragCoord.xy));

        vec3 spot;
        vec3 gi;

        vec2 lightPos = vec2(sin(iTime*.5)*.75,cos(iTime*.25)*.25+.25);

        vec2 lightDir = normalize(vec2(sin(iTime*1.5),-1));
        if (iMouse.z > 0.){
        lightPos = vec2(2,-2)*iMouse.zw/iResolution.x-vec2(1.,.56);
        lightDir = normalize(2.*iMouse.xy/iResolution.x-vec2(1.,.561)-lightPos);
        }
        float lightRad = .005;

        if (sceneIntersect(p, normalize(lightPos-p)).w > distance(p,lightPos)) {
        spot = vec3(max((.5*float(dot(normalize(p-lightPos),lightDir))-.5)/lightRad+1.,0. ));
        }

        vec2 hit;
        for (int i=0; i<samples; i++) {
        vec2 ro = lightPos;
        float rot = .08*pi*((float(i)+rand)/float(samples)-.5) + atan(lightDir.y,lightDir.x);
        vec2 rd = vec2(cos(rot),sin(rot));
        vec2 lightDirSampled = rd;

        float d = sceneIntersect(ro, rd).w;
        hit = ro + rd*d;
        vec2 nor = sceneNormal(hit - rd*.01);

        ro = p;
        rd = normalize(hit-p);

        // Circle arc for bounce light falloff just beause I thought it looked better than inverse square law :p
        float hitDist = min(distance(p,hit)/lightFalloff,1.);

        vec4 lightRay = sceneIntersect(ro, rd);
        d = lightRay.w;

        if (d + .01 > distance(p,hit)) {
            gi += 1./float(samples) * lightRay.rgb * clamp(dot(-rd,nor),0.,1.) * ( 1.-sqrt(2.*hitDist-hitDist*hitDist) )
            * (sceneDist(p).w > .005 ? 1. : dot(sceneNormal(p),lightDirSampled)*.5+.5 );
        }
        }

        vec4 scene = sceneDist(p);
        col = spot*.5 + gi*1.;
        col *= scene.w > .005 ? vec3(.25) : 3.*scene.rgb;

        // Tonemapping

        col = ACESFilm(col);
        col = pow(col,vec3(1./2.2));

        fragColor = vec4(col,1);
        }

    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
                          1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    bool running = true;
    SDL_Event event;

    GLint success;
    GLchar infoLog[512];

    // Check vertex shader compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Check fragment shader compilation status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Check shader program linking status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    float mouseClickX = 0.0f;
    float mouseClickY = 0.0f;

    while (running)
    {

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                mouseClickX = event.button.x;
                mouseClickY = event.button.y;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                mouseClickX = 0.f;
                mouseClickY = 0.f;
            }
        }

        // Set uniforms
        glUseProgram(shaderProgram);

        GLint resolutionUniformLocation = glGetUniformLocation(shaderProgram, "iResolution");
        glUniform2f(resolutionUniformLocation, 800.0f, 450.0f);

        GLint timeUniformLocation = glGetUniformLocation(shaderProgram, "iTime");
        glUniform1f(timeUniformLocation, SDL_GetTicks() / 1000.0f);

        GLint mouseUniformLocation = glGetUniformLocation(shaderProgram, "iMouse");
        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        glUniform4f(mouseUniformLocation, mouseX, -mouseY + 450.f, mouseClickX, mouseClickY - 450.f);

        // Render quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    // Clean up
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
