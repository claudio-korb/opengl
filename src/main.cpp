#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;
    if(!glfwInit())
    {
        std::cout << "Error on glfWInit()" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Render Screen", NULL, NULL);

    if(!window)
    {
        std::cout << "Error on glfwCreateWindow()" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    int errorCode = glewInit();
    if(errorCode != GLEW_OK)
    {
        std::cout << "Error on glewInit():" << glGetString(errorCode) << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int vBuffer;
    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}