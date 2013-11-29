//
//  main.cpp
//  OpenGLLearning
//
//  Created by Darren Liu on 13-11-17.
//  Copyright (c) 2013年 Darren Liu. All rights reserved.
//

#define KeyCount 7
#define Frequency 6

#define JPosT -0.6f
#define JPosB -0.8f
#define Speed 1.0f

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define OGLFT_NO_SOLID
#define OGLFT_NO_QT
#include <oglft/OGLFT.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>

int count;

char gameKey[KeyCount] = {'S', 'D', 'F', ' ', 'J', 'K', 'L'};
bool gKeyS[KeyCount] = {false};

class Quad {
public:
    Quad() : left(0.f), down(0.f), right(0.f), up(0.f), r(0.f), g(0.f), b(0.f) {}
    Quad(const float &left, const float &down, const float &right, const float &up) : left(left), down(down), right(right), up(up), r(0.f), g(0.f), b(0.f) {}
    float left;
    float down;
    float right;
    float up;
    float r;
    float g;
    float b;
};

static void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

std::vector<Quad> notes;
const static float linePos[KeyCount] = {
    -0.428571f,
    -0.285714f,
    -0.142857f,
    0.f,
    0.142857f,
    0.285714f,
    0.428571f
};

static void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_LEFT) {
//        printf("left\n");
//    } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
//        printf("right\n");
//    }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else {
        for (int i = 0; i < KeyCount; i++) {
            if (key == gameKey[i] && !gKeyS[i]) {
                gKeyS[i] = true;
                printf("Key Press: '%c'", key);
                for (std::vector<Quad>::iterator p = notes.begin(); p != notes.end(); p++) {
                    if (p -> up < JPosB) {
                        continue;
                    }
                    if (p -> down > JPosT) {
                        break;
                    }
                    if (p -> left < linePos[i] && linePos[i] < p -> right) {
                        count++;
                        printf("\tHit!");
                        notes.erase(p);
                        break;
                    }
                }
                printf("\n");
            }
        }
    }
    if (action == GLFW_RELEASE) {
        for (int i = 0; i < KeyCount; i++) {
            if (key == gameKey[i]) {
                gKeyS[i] = false;
//                printf("Key Release: %c\n", key);
            }
        }
    }
}

void drawQuad(const Quad &quad) {
    glBegin(GL_QUADS); {
        glColor3f(quad.r, quad.g, quad.b); glVertex3f(quad.left, quad.down, 0.f);
        glColor3f(quad.r, quad.g, quad.b); glVertex3f(quad.right, quad.down, 0.f);
        glColor3f(quad.r, quad.g, quad.b); glVertex3f(quad.right, quad.up, 0.f);
        glColor3f(quad.r, quad.g, quad.b); glVertex3f(quad.left, quad.up, 0.f);
    } glEnd();
}


int main(void)
{
    GLFWwindow *window;
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
    glfwSetMouseButtonCallback(window, mouse_callback);
    glewInit();
    Quad line[KeyCount];
    
    for (int i = 0; i < KeyCount; i++) {
        line[i] = Quad(linePos[i] - 0.005f, -1.f, linePos[i] + 0.005f, 1.f);
    }
    
    Quad JLineT(-0.428572f, JPosT - 0.005f, 0.428572f, JPosT + 0.005f);
    Quad JLineB(-0.428572f, JPosB - 0.005f, 0.428572f, JPosB + 0.005f);
    
    double time, lastTime;
    int last = 0;
    count = 0;
    lastTime = glfwGetTime();
//    OGLFT::Monochrome *face = new OGLFT::Monochrome("/System/Library/Fonts/Avenir Next.ttc", 48);
    OGLFT::Translucent *face = new OGLFT::Translucent("/System/Library/Fonts/Avenir Next.ttc", 48);
    face -> setForegroundColor(0.f, 0.f, 0.f);
    face -> setAdvance(true);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.f, 1.f, 1.f, 1.f);
//        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        
        glLoadIdentity();
        for (int i = 0; i < KeyCount; i++) {
            drawQuad(line[i]);
        }
        
        
        drawQuad(JLineT);
        drawQuad(JLineB);
        time = glfwGetTime();
        if ((int)(time * Frequency) - lastTime * Frequency > 0) {
            if (last != (int)(time * Frequency)) {
                last = (int)(time * Frequency);
                int keyNow = (float)rand() / RAND_MAX * KeyCount + 1;
                float pos = (keyNow - 0.5f) * 1.f / KeyCount - 0.55f;
//                printf("Note: '%c'\n", gameKey[keyNow - 1]);
                notes.push_back(Quad(pos, 1.f, pos + 0.1f, 1.025f));
            }
        }
        for (std::vector<Quad>::iterator p = notes.begin(); p != notes.end(); p++) {
            p -> down -= (time - lastTime) * Speed;
            p -> up -= (time - lastTime) * Speed;
            if (p -> g == 0.f) {
                if (p -> up < JPosT) {
                    p -> g = 1.f;
                }
            } else if (p -> r == 0.f) {
                if (p -> up < JPosB) {
                    p -> r = 1.f;
                }
            }
            drawQuad(*p);
            if (p -> up < -1.f) {
//                printf("Miss: '%c'\n", gameKey[(int)((p -> left + 0.55f) * KeyCount + 0.5f) - 1]);
                count = 0;
                notes.erase(p);
                p--;
            }
        }
        lastTime = time;
        
        if (face) {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            char num[6];
            sprintf(num, "%d", count);
            OGLFT::BBox box = face -> OGLFT::Face::measure(num);
            face -> draw(0.f - (box.x_max_ - box.x_min_) / 2.f, .4f, num);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
