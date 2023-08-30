#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static float pxToCoordinateX(double x)
{
    return (2*x - WINDOW_WIDTH) / (WINDOW_WIDTH);
}

static float pxToCoordinateY(double y)
{
    return (-1)*(2*y - WINDOW_HEIGHT) / (WINDOW_HEIGHT);

}

int main()
{
    GLFWwindow* window;
    if(!glfwInit())
    {
        std::cout << "Error on glfWInit()" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Render Screen", NULL, NULL);

    if(!window)
    {
        std::cout << "Error on glfwCreateWindow()" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    int errorCode = glewInit();
    if(errorCode != GLEW_OK)
    {
        std::cout << "Error on glewInit():" << glGetString(errorCode) << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
        
    IndexBuffer ib(indices, 6);

    Shader shader = Shader("../res/shaders/Basic.glsl");
    shader.CompileShaders();

    GLCall(glBindVertexArray(0));
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    float r = 0.0f;
    float increment = 0.05f;

    while(!glfwWindowShouldClose(window))
    {
        static uint8_t canClick = true;
        static float newPositions[] = { 0,0,
                                        0,0,
                                        0,0,
                                        0,0};
        static uint8_t clickCount = 0;  

        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glUseProgram(shader.GetProgram()));

        int uniform_location = glGetUniformLocation(shader.GetProgram(), "u_Color");
        if(uniform_location != -1)
        {
            GLCall(glUniform4f(uniform_location, r, 0.3f, 0.3f, 1.0f));
        }

        va.Bind();
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if(r > 1.0f)
            increment = -0.05f;
        else if(r < 0.0f)
            increment = 0.05f;

        r+=increment;

        glfwSwapBuffers(window);

        glfwPollEvents();

        //New Triangle ------------------------
        int click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT); 

        if(canClick && click == GLFW_PRESS)
        {
            double xpos, ypos;
            static uint8_t i = 0;
            glfwGetCursorPos(window, &xpos, &ypos);
            xpos = pxToCoordinateX(xpos);
            ypos = pxToCoordinateY(ypos);
            newPositions[2*i + 0] = xpos;
            newPositions[2*i + 1] = ypos;
            std::cout << "X: " << xpos << " | Y: " << ypos << std::endl;
            canClick = false;
            // i++;
            // if(i > 3)
            // {
            //     memcpy(positions, newPositions, sizeof(positions));
            //     GLCall(glBufferData(GL_ARRAY_BUFFER, 6 *2 * sizeof(float), positions, GL_STATIC_DRAW));
            //     i = 0;
            // }
        }

        if(click == GLFW_RELEASE)
        {
            canClick = true;
        }
    }

    glfwTerminate();

    return 0;
}