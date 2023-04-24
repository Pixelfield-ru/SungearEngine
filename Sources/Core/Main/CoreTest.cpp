//
// Created by stuka on 18.04.2023.
//

#include "CoreTest.h"
//#date <GLFW/glfw3.h>
#include <iostream>
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/Api/Base.h"
#include "../Logging/Log.h"

GLFWwindow* wnd = nullptr;

void print_glfw_errors(int errc, const char* err)
{
    std::cerr << "GLFW error: " << err << std::endl;
}

class Test
{
public:
    int t0 = 5;
    int t1 = 6;
    int t2 = 7;
};

int main()
{
    std::cout << "Hello test!" << std::endl;

    Core::Logging::init();
    // TODO: just test to delete! -----------------------------

    glfwSetErrorCallback(print_glfw_errors);
    if(!glfwInit())
    {
        print_glfw_errors(0, "Error initializing GLFW!");
    }

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    wnd = glfwCreateWindow(500, 500, "telezhechka", NULL, NULL);

    glfwMakeContextCurrent(wnd);

    glfwSwapInterval(0);

    // сделать окно видимым
    glfwShowWindow(wnd);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while(true)
    {
        glBegin(GL_TRIANGLES);

        glVertex2f(0, 0);
        glVertex2f(0, 0.5);
        glVertex2f(0.5, 0);

        glEnd();

        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }

    // ---------------------------------------------------

    return 0;
}