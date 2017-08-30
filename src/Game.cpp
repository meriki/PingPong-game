#include <imgui_impl_glfw_gl3.h>
#include "Game.hpp"
#include "utitliy.hpp"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

using std::cout;
using std::endl;


Game::Game(int width,int height,const std::string & title):width(width),height(height) {

    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
    // Create a GLFWwindow object
    mainWindow = glfwCreateWindow(width,height,title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(mainWindow,this);
    if (mainWindow == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(mainWindow);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize OpenGL context" << endl;
        exit(-1);
    }
    ImGui_ImplGlfwGL3_Init(mainWindow,false);
    //setup matrix
    {
        projectionMatrix = glm::ortho(0.0f,width*1.0f,0.0f,height*1.0f,-1.0f,1.0f);
    }
    glViewport(0,0, width, height);
    //compile shader
    {
        auto vs = fileToCharArr ("resources/sprite.vert");
        auto fs = fileToCharArr ("resources/sprite.frag");
       spriteProgram = makeProgram(compileShader(shDf(GL_VERTEX_SHADER,&vs[0],vs.size())),compileShader(shDf(GL_FRAGMENT_SHADER,&fs[0],fs.size())));
    }


}

void Game::run() {

    double last = 0;
    glfwSetTime(last);
    double delta = 0.0f;
    glfwSwapInterval(1);

//    renderInitialize();
    float c_x=0.0f,c_y=-0.4f;
    float r=0.06f;
    float temp_x=0.02f,temp_y=0.02f;
    int n_sides = 18;

    while (!glfwWindowShouldClose(mainWindow)) {

        double curr = glfwGetTime();
        delta = curr-last;
//        last = curr;

        //input polling
        glfwPollEvents();

        update(delta);

        if(!pause) {

            if(delta > 10){
                if(temp_x < 0)
                    temp_x = temp_x -0.01;
                else
                    temp_x=temp_x+0.01;
                if(temp_y > 0)
                    temp_y=temp_y+0.01;
                else
                    temp_y=temp_y-0.01;
                last=curr;
            }


            if (c_x + r >= 0.90)
                temp_x = -temp_x;
            if (c_y - r <= -0.7 && rec_vertices[0] >= c_x && rec_vertices[6] <= c_x) {
                temp_y = -temp_y;
                game_score = game_score + 1;
            }

            if (c_x - r <= -0.90)
                temp_x = -temp_x;
            if (c_y + r >= 0.90)
                temp_y = -temp_y;

            if (c_y - r <= -1.0) {
                temp_y = -temp_y;
                game_score = 0;
            }

            c_x = temp_x + c_x;
            c_y = temp_y + c_y;
        }

            int n_vertices = n_sides + 2;
            float twopi = 2.0f * M_PI;

            float cv_x[n_vertices];
            float cv_y[n_vertices];

            cv_x[0] = c_x;
            cv_y[0] = c_y;

            for (int i = 1; i < n_vertices; i++) {
                cv_x[i] = c_x + (r * cos(i * twopi / n_sides));
                cv_y[i] = c_y + (r * sin(i * twopi / n_sides));
            }

            float circle[n_vertices * 3];

            for (int i = 0; i < n_vertices; i++) {
                circle[i * 3] = cv_x[i];
                circle[i * 3 + 1] = cv_y[i];
                circle[i * 3 + 2] = 0;
            }

            for (int i = 0; i < n_vertices; i++) {
                printf("%f %f %f\n", circle[i * 3], circle[i * 3 + 1], circle[i * 3 + 2]);
            }

            unsigned int VBO_R, EBO_R, VBO_T, VBO_W, EBO_W;

            glGenVertexArrays(1, &VAO_1);
            glGenVertexArrays(1, &VAO_2);
            glGenVertexArrays(1, &VAO_3);
            glGenBuffers(1, &VBO_R);
            glGenBuffers(1, &EBO_R);
            glBindVertexArray(VAO_1);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_R);
            glBufferData(GL_ARRAY_BUFFER, sizeof(rec_vertices), rec_vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_R);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rec_indices), rec_indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            glGenBuffers(1, &VBO_T);
            glBindVertexArray(VAO_2);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_T);
            glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            glGenBuffers(1, &VBO_W);
            glGenBuffers(1, &EBO_W);
            glBindVertexArray(VAO_3);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_W);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_W);
            glBufferData(GL_ARRAY_BUFFER, sizeof(walls), walls, GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(walls_indices), walls_indices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);

            //update objects


            //draw them

            draw();


        glfwSwapBuffers(mainWindow);
    }
    printf("done");
}
Game::~Game() {
    glfwTerminate();
}
void Game::update(double delta) {
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
    if (glfwGetKey(mainWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if(rec_vertices[0] <= 0.90) {
            rec_vertices[0] = rec_vertices[0] + 0.05;
            rec_vertices[3] = rec_vertices[3] + 0.05;
            rec_vertices[6] = rec_vertices[6] + 0.05;
            rec_vertices[9] = rec_vertices[9] + 0.05;
        }
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if(rec_vertices[6] >= -0.90) {
            rec_vertices[0] = rec_vertices[0] - 0.05;
            rec_vertices[3] = rec_vertices[3] - 0.05;
            rec_vertices[6] = rec_vertices[6] - 0.05;
            rec_vertices[9] = rec_vertices[9] - 0.05;
        }
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS){
        pause=!pause;
    }
}

void Game::draw() {

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(spriteProgram);
    glBindVertexArray(VAO_1);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    glUseProgram(spriteProgram);
    glBindVertexArray(VAO_2);
    glDrawArrays(GL_TRIANGLE_FAN,0,20);
    glBindVertexArray(0);

    glUseProgram(spriteProgram);
    glBindVertexArray(VAO_3);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

    showScore(game_score);

}


void Game::showScore(int score) {


    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(690,0));
    bool show_another_window = true;
    ImGui::Begin("Score", &show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

    ImGui::Text("Score %d    ",score);


    ImGui::End();
    ImGui::Render();


}

