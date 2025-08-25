#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <camera.h>
#include <cube.h>
#include <MarchingCubes.h>

//controls
static double previous_xpos = 0.0;
static double previous_ypos = 0.0;
static bool rotateEnable = false;
static double rotateX = 0.0;
static double rotateY = 0.0;
static bool nextConfiguration = false;


static void glfw_error_callback(int error, const char* description)
{
    std::cout << "Glfw Error " << error << " : " << description << "\n";
}

static void key_handle(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        nextConfiguration = true;
    }
}

static void mouse_handle(GLFWwindow* window, double xpos, double ypos)
{
    if (rotateEnable)
    {
        double delta_x = xpos - previous_xpos;

        float ry = (float)(0.005f * delta_x);
        rotateY += ry;

        double delta_y = ypos - previous_ypos;

        float rx = (float)(0.005f * delta_y);
        rotateX += rx;

        previous_xpos = xpos;
        previous_ypos = ypos;
    }
}

static void mouse_button_handle(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            double xpos, ypos;

            glfwGetCursorPos(window, &xpos, &ypos);

            previous_xpos = xpos;
            previous_ypos = ypos;

            rotateEnable = true;
        }
        else if (action == GLFW_RELEASE)
        {
            rotateEnable = false;
        }
    }
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


TEST_F(OpenGLTestFixture, DISABLED_TestCase1)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int corner = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(corner == 0 ? 1500 : 0) });
        cell.push_back({ {half,-half,half}, unsigned short(corner == 1 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(corner == 2 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(corner == 3 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(corner == 4 ? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(corner == 5 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(corner == 6 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(corner == 7 ? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                corner++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (corner == 8)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}

TEST_F(OpenGLTestFixture, DISABLED_TestCase2)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int pair = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(pair == 0 || pair == 3 || pair == 8 ? 1500 : 0)});
        cell.push_back({ {half,-half,half}, unsigned short(pair == 0 || pair == 1 || pair == 9 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(pair == 2 || pair == 3 || pair == 11 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(pair == 1 || pair == 2  || pair == 10 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(pair == 4 || pair == 7 || pair == 8? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(pair == 4 || pair == 5 || pair == 9 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(pair == 6 || pair == 7 || pair == 11 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(pair == 5 || pair == 6 || pair == 10 ? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                pair++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (pair == 12)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}

TEST_F(OpenGLTestFixture, TestCase4)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int pair = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(pair == 0 ? 1500 : 0) });
        cell.push_back({ {half,-half,half}, unsigned short(pair == 1 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(pair == 2 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(pair == 3 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(pair == 3 ? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(pair == 2 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(pair == 1 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(pair == 0? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                pair++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (pair == 4)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}

TEST_F(OpenGLTestFixture, DISABLED_TestCase5)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int pair = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(pair == 0 || pair == 2 || pair == 3 || pair == 8 || pair == 10 || pair == 11 ? 1500 : 0) });
        cell.push_back({ {half,-half,half}, unsigned short(pair == 0 || pair == 1 || pair == 3 || pair == 12 || pair == 14 || pair == 15 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(pair == 1 || pair == 2 || pair == 3 || pair == 8 || pair == 9 || pair == 11 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(pair == 0 || pair == 1 || pair == 2 || pair == 12 || pair == 13 || pair == 15 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(pair == 4 || pair == 6 || pair == 7 || pair == 9 || pair == 10 || pair == 11 ? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(pair == 4 || pair == 5 || pair == 7 || pair == 13 || pair == 14 || pair == 15 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(pair == 5 || pair == 6 || pair == 7 || pair == 8 || pair == 9 || pair == 10 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(pair == 4 || pair == 5 || pair == 6 || pair == 12 || pair == 13 || pair == 14 ? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                pair++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (pair == 16)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}

TEST_F(OpenGLTestFixture, DISABLED_TestCase8)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int quad = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(quad == 0 || quad == 2 ? 1500 : 0) });
        cell.push_back({ {half,-half,half}, unsigned short(quad == 0 || quad == 3 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(quad == 0 || quad == 2 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(quad == 0 || quad == 3 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(quad == 1 || quad == 2 ? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(quad == 1 || quad == 3 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(quad == 1 || quad == 2 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(quad == 1 || quad == 3 ? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                quad++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (quad == 4)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}

TEST_F(OpenGLTestFixture, DISABLED_TestCase9)
{
    float cubeEdgeLength = 200;
    float half = cubeEdgeLength / 2;

    int quad = 0;

    while (!glfwWindowShouldClose(g_window))
    {
        std::vector<std::pair<std::array<float, 3>, unsigned short>> cell;

        cell.push_back({ {-half,-half, half}, unsigned short(quad == 0 || quad == 1 || quad == 3 || quad == 4 ? 1500 : 0) });
        cell.push_back({ {half,-half,half}, unsigned short(quad == 0 || quad == 1 || quad == 2 || quad == 5 ? 1500 : 0) });
        cell.push_back({ {-half,-half,-half}, unsigned short(quad == 0 || quad == 2 || quad == 3 || quad == 7 ? 1500 : 0) });
        cell.push_back({ {half,-half,-half}, unsigned short(quad == 1 || quad == 2 || quad == 3 || quad == 6 ? 1500 : 0) });
        cell.push_back({ {-half,half,half}, unsigned short(quad == 0 || quad == 4 || quad == 5 || quad == 7 ? 1500 : 0) });
        cell.push_back({ {half,half,half}, unsigned short(quad == 1 || quad == 4 || quad == 5 || quad == 6 ? 1500 : 0) });
        cell.push_back({ {-half,half,-half}, unsigned short(quad == 3 || quad == 4 || quad == 6 || quad == 7 ? 1500 : 0) });
        cell.push_back({ {half,half,-half}, unsigned short(quad == 2 || quad == 5 || quad == 6 || quad == 7 ? 1500 : 0) });

        std::pair<unsigned short, unsigned short> innerRange = { 1000,2000 };

        int cornerSet{ 0 };

        mesh m = getTriangles(cell, innerRange, cornerSet);

        std::vector<std::array<std::array<float, 3>, 3>>;
        std::vector<float> meshData;

        for (const auto& t : m)
        {
            meshData.insert(meshData.end(), std::begin(t[0]), std::end(t[0]));
            meshData.insert(meshData.end(), std::begin(t[1]), std::end(t[1]));
            meshData.insert(meshData.end(), std::begin(t[2]), std::end(t[2]));
        }

        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);

        Camera cameraGlobal(g_window);

        Cube cube(cubeEdgeLength, cornerSet, meshData);
        cube.Setup();

        while (!glfwWindowShouldClose(g_window))
        {
            int width, height;
            glfwGetFramebufferSize(g_window, &width, &height);
            glViewport(0, 0, width, height);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glEnable(GL_CULL_FACE);

            glm::mat4 projection_matrix;
            glm::mat4 view_matrix;

            cameraGlobal.rotateX(rotateX);
            cameraGlobal.rotateY(rotateY);
            cameraGlobal.computeViewProjectionMatrices(false, false);
            projection_matrix = cameraGlobal.getProjectionMatrix();
            view_matrix = cameraGlobal.getViewMatrix();
            rotateX = 0.0;
            rotateY = 0.0;

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glFrontFace(GL_CW);
            glDisable(GL_CULL_FACE);

            glUseProgram(cube.GetProgramId());

            //cube.UpdateModel(view);
            //cube.SetProjection(proj);
            cube.UpdateModel(view_matrix);
            cube.SetProjection(projection_matrix);
            cube.Draw();


            // draw geometry here
            glfwSwapBuffers(g_window);
            glfwPollEvents();

            if (nextConfiguration)
            {
                quad++;
                nextConfiguration = false;
                break;
            }
        }

        EXPECT_EQ(1, 1);
        EXPECT_TRUE(true);

        if (quad == 8)
        {
            glfwSetWindowShouldClose(g_window, GLFW_TRUE);
            break;
        }
    }
}


