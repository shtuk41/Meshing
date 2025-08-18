#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <MarchingCubes.h>

static void glfw_error_callback(int error, const char* description)
{
    std::cout << "Glfw Error " << error << " : " << description << "\n";
}

static void key_handle(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

static void mouse_handle(GLFWwindow* window, double xpos, double ypos)
{

}

static void mouse_button_handle(GLFWwindow* window, int button, int action, int mods)
{

}

static void scroll_handle(GLFWwindow* window, double xoffset, double yoffset)
{

}

class OpenGLTestFixture : public ::testing::Test {
protected:
    // Common objects/resources used by your tests
    GLuint shaderProgram;
    static GLFWwindow* g_window;

    OpenGLTestFixture() 
    {
        // Initialize common objects here if they don't involve fatal assertions or exceptions in teardown
        // Example:  Load shaders, create a simple OpenGL context (if not done globally)
        // You might consider using SetUp() for  creating the OpenGL context
    }

    void SetUp() override 
    {
        glfwSetErrorCallback(glfw_error_callback);

        ASSERT_TRUE(glfwInit());

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        //enable anti-alising 4x with GLFW
        glfwWindowHint(GLFW_SAMPLES, 4);

        //specify the client API version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //make the GLFW forward compatible

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //enable the OpenGL core profile for GLFW

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        //create a GLFW window object

        g_window = glfwCreateWindow(800, 600, "MarchingCubes", NULL, NULL);

        if (!g_window)
        {
            glfwTerminate();

            throw std::exception("create window failed");
        }

        glfwMakeContextCurrent(g_window);
        glfwSwapInterval(1);

        glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glfwSetKeyCallback(g_window, key_handle);
        glfwSetCursorPosCallback(g_window, mouse_handle);
        glfwSetMouseButtonCallback(g_window, mouse_button_handle);
        glfwSetScrollCallback(g_window, scroll_handle);

        glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_FALSE);

        glfwSwapInterval(1); // Enable vsync
        glewExperimental = true;
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initlize GLEW" << "\n";
        }
    }

    void TearDown() override 
    {
        if (g_window) {
            glfwDestroyWindow(g_window);
            g_window = nullptr;
        }

        glfwTerminate();
    }
};

GLFWwindow* OpenGLTestFixture::g_window = nullptr;


TEST_F(OpenGLTestFixture, TestName1) {

    while (!glfwWindowShouldClose(g_window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // draw geometry here
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }

  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}