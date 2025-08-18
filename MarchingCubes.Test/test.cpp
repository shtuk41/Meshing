#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <camera.h>
#include <cube.h>
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


TEST_F(OpenGLTestFixture, TestName1) 
{
    //glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    Camera cameraGlobal(g_window);

    Cube cube(200);
    cube.Setup();

    while (!glfwWindowShouldClose(g_window)) 
    {
        int width, height;
        glfwGetFramebufferSize(g_window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glEnable(GL_CULL_FACE);

        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;

        cameraGlobal.computeViewProjectionMatrices(false, false);
        projection_matrix = cameraGlobal.getProjectionMatrix();
        view_matrix = cameraGlobal.getViewMatrix();

        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1000.0f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
        cube.UpdateModel(view);
        cube.SetProjection(proj);

        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_BLEND);
        //glFrontFace(GL_CW);
        //glDisable(GL_CULL_FACE);

        glUseProgram(cube.GetProgramId());

        cube.UpdateModel(view);
        cube.SetProjection(proj);
        cube.Draw();


        // draw geometry here
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }

  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}