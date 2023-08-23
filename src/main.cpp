#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;
    if(!glfwInit())
    {
        std::cout << "Error 1" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int errorCode = glewInit();
    if(errorCode != GLEW_OK)
    {
        std::cout << "Error :" << errorCode << std::endl;
        return -1;
    }

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}