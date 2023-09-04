#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shader.h"
#include "texture.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

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
        -2.0f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
    };
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    unsigned int vao;

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);
        
    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    Shader shader = Shader("../res/shaders/Basic.glsl");
    shader.Bind();        
    
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniformMat4f("u_MVP", proj);

    Texture texture("../res/textures/gold.jpg");

    Renderer renderer;

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

        texture.Bind();

        renderer.Draw(va, ib, shader);

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
            //     vb = VertexBuffer(positions, 4 * 4 * sizeof(float));
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
