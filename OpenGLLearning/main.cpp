//
//  main.cpp
//  OpenGLLearning
//
//  Created by Darren Liu on 13-11-17.
//  Copyright (c) 2013年 Darren Liu. All rights reserved.
//

#define KeyCount 7
#define Frequency 3

#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>

char gameKey[KeyCount] = {'S', 'D', 'F', ' ', 'J', 'K', 'L'};

class Quad {
public:
    Quad() : left(0.f), down(0.f), right(0.f), up(0.f) {}
    Quad(const float &left, const float &down, const float &right, const float &up) : left(left), down(down), right(right), up(up) {}
    float left;
    float down;
    float right;
    float up;
};

static void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        for (int i = 0; i < KeyCount; i++) {
            if (key == gameKey[i]) {
                printf("Key Press: %c\n", key);
            }
        }
    }
}

void drawQuad(const Quad &quad) {
    glBegin(GL_QUADS); {
        glColor3f(1.f, 1.f, 1.f); glVertex3f(quad.left, quad.down, 0.f);
        glColor3f(1.f, 1.f, 1.f); glVertex3f(quad.right, quad.down, 0.f);
        glColor3f(1.f, 1.f, 1.f); glVertex3f(quad.right, quad.up, 0.f);
        glColor3f(1.f, 1.f, 1.f); glVertex3f(quad.left, quad.up, 0.f);
    } glEnd();
}

// const static Quad playground(-0.5f, -1.f, 0.5f, 1.f);
const static float linePos[KeyCount] = {
    -0.500000,
    0.071429,
    -0.357143,
    -0.214286,
    -0.071429,
    0.357143,
    0.214286
};



int main(void)
{
    GLFWwindow *window;
    std::vector<Quad> notes;
    srand((unsigned int)time(0));
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(1280, 720, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    Quad line[KeyCount];
    
    for (int i = 0; i < KeyCount; i++) {
        line[i] = Quad(linePos[i] - 0.005f, -1.f, linePos[i] + 0.005f, 1.f);
    }
    
    float time, lastTime;
    int count = 0, last = 0;
    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        for (int i = 0; i < KeyCount; i++) {
            drawQuad(line[i]);
        }
        time = glfwGetTime();
        if ((int)(time * Frequency) - lastTime * Frequency > 0) {
            if (last != (int)(time * Frequency)) {
                last = (int)(time * Frequency);
                float pos = ((int)((float)rand() / RAND_MAX * KeyCount + KeyCount / 2 + 1) - KeyCount / 2 - 1) * 1.f / KeyCount - 0.55f;
                printf("%d. Position = %f\n", ++count, pos + 0.05f);
                notes.push_back(Quad(pos, 1.f, pos + 0.1f, 1.025f));
            }
        }
        for (std::vector<Quad>::iterator p = notes.begin(); p != notes.end(); p++) {
            // printf("%f", time - lastTime);
            p -> down -= time - lastTime;
            p -> up -= time - lastTime;
            drawQuad(*p);
            if (p -> up < -1.f) {
                notes.erase(p);
                p--;
            }
        }
        lastTime = time;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
