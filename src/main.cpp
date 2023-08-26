#include "../include/shader.h"

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

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    
    Shader shader = Shader("../res/shaders/Basic.shader");
    shader.CompileShaders();

    glUseProgram(shader.GetProgram());

    while(!glfwWindowShouldClose(window))
    {
        static uint8_t canClick = true;
        static float newPositions[6] = {0,0,
                                        0,0,
                                        0,0};
        static uint8_t clickCount = 0;        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
            i++;
            if(i > 2)
            {
                memcpy(positions, newPositions, sizeof(positions));
                glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
                i = 0;
            }
        }

        if(click == GLFW_RELEASE)
        {
            canClick = true;
        }
    }

    glfwTerminate();

    return 0;
}