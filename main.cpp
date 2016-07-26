#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "lodepng/lodepng.h"
#include "ResourceManager/ResourceManager.h"

using namespace std;

int main(int xarg, char** args) {

    ResourceManager::loadFile("vertex_shader.cpp");
    ResourceManager::loadFile("fragment_shader.cpp");

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, "player.png");
    size_t u2 = 1;
    size_t v2 = 1;

    // If there's an error, display it.
    if(error != 0)
    {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1;
    }

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
    if (glewInit()) {
        cout << "Could not initialize GLEW" << endl;
    }

    // Shader sources
    const std::string vertex_str = ResourceManager::get("vertex_shader.cpp");
    const std::string fragment_str = ResourceManager::get("fragment_shader.cpp");
    const GLchar* vertexSource = vertex_str.c_str();
    const GLchar* fragmentSource = fragment_str.c_str();

    float x, y = 0.0f;

    GLfloat Vertices[] = {(float)x, (float)y, 0,
        (float)x + width, (float)y, 0,
        (float)x + (float)width, (float)y + (float)height, 0,
        (float)x, (float)y + (float)height, 0};
    GLfloat TexCoord[] = {0, 0,
        1, 0,
        1, 1,
        0, 1,
    };
    GLubyte indices[] = {0,1,2, 
        0,2,3};


    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();

    GLint posAttrib = glGetAttribLocation(shaderProgram, "myTextureSampler");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, textureID, 0);


    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);



    SDL_Event e;
    while(e.type != SDL_QUIT) {
        if (e.type == SDL_KEYUP &&
                e.key.keysym.sym == SDLK_ESCAPE) { break; }

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, Vertices);

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, TexCoord);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        SDL_GL_SwapWindow(window);
        SDL_PollEvent(&e);
    }

    SDL_GL_DeleteContext(context);
}
