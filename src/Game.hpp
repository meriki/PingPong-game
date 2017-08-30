
#ifndef ASSIGNMENT1_GAME_HPP
#define ASSIGNMENT1_GAME_HPP


#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>


class GLFWwindow;


class Game {

public:
    Game(int width,int height,const std::string &title = "MainWindow");

    void run();
    ~Game();


    int height;
    int width;
private:

    GLFWwindow *mainWindow = nullptr;

    GLuint VAO_1,VAO_2,VAO_3;
    void renderInitialize();
    void update(double delta);
    void draw();
    float movement;
    float rec_vertices[12] = {
                0.3f,  -0.7f, 0.0f,  // top right
                0.3f, -0.8f, 0.0f,  // bottom right
                -0.3f, -0.8f, 0.0f, // bottom left
                -0.3f, -0.7f, 0.0f //top left
    };

    unsigned int rec_indices[6]={
            0,1,2,
            0,2,3
    };

    float walls[24] = {
            -1.0f,-1.0f,0.0f,
            -0.90f, -1.0f, 0.0f,
            -0.90f, 0.90f, 0.0f,
            -1.0f, 1.0f ,0.0f,
            1.0f, 1.0f, 0.0f,
            0.90f, 0.90f, 0.0f,
            0.90f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
    };

    unsigned int walls_indices[18]={
            0,1,2,
            0,2,3,
            3,4,5,
            4,5,7,
            2,3,5,
            5,6,7
    };

private:

    GLuint spriteProgram;

    glm::mat4 projectionMatrix;
    int game_score;
    void showScore(int score);
    bool pause=false;


};



#endif //ASSIGNMENT1_GAME_HPP
