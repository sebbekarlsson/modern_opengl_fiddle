#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

using namespace std;

int main(int xarg, char** args) {

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(
            "Test",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640,
            480,
            SDL_WINDOW_OPENGL
            );

    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    glewInit();

    // Shader sources
    const GLchar* vertexSource =
        "#version 150\n"
        "in vec2 position;\n"
        "in vec3 color;\n"
        "out vec3 Color;\n"
        "void main()"
        "{"
        "Color = color;"
        "gl_Position = vec4(position, 0.0, 1.0);"
        "}";

    const GLchar* fragmentSource =
        "#version 150\n"
        "in vec3 Color;\n"
        "out vec4 outColor;\n"
        "void main()"
        "{"
        "outColor = vec4(Color, 1.0);"
        "}";


    float vertices[] = {
        0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    SDL_Event e;
    while(e.type != SDL_QUIT) {
        if (e.type == SDL_KEYUP &&
                e.key.keysym.sym == SDLK_ESCAPE) { break; }

        glClear(GL_COLOR_BUFFER_BIT);
        //glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
        SDL_PollEvent(&e);
    }

    SDL_GL_DeleteContext(context);
}
