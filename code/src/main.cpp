// This example is heavily based on the tutorial at https://open.gl

// Application Libraries
#include "lib/geometry/GeometryClass.h"
#include "lib/features/MacroClass.h"
#include "helper/CallbackClass.h"
#include "view/ViewControl.h"
#include "lib/features/Skybox.h"

// GLFW Library
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// Timer
#include <chrono>
#include <iostream>

using namespace SceneEditor;

/* [GLFW Window Guide]
*  Window Creation Reference: https://www.glfw.org/docs/3.3/window_guide.html#window_creation
*/
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
static const char WINDOW_TITLE[] = "[Assignment-4] Environment Mapping and Shadow Mapping";

static Skybox skybox;
static Geometry geometry;
static ViewControl viewcontrol;
static Callbacks callbacks(geometry, viewcontrol);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    callbacks.windowSizeCallback(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get the size of the window
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Convert screen position to world coordinates
    double screen_x = ((xpos/double(width))*2)-1;
    double screen_y = (((height-1-ypos)/double(height))*2)-1; // NOTE: y axis is flipped in glfw

    // Update the position of the first vertex if the left button is pressed
    callbacks.mouseClickCallback(button, action, screen_x, screen_y);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    callbacks.keyboardCallback(key, action);
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    // Window Reference: https://www.glfw.org/docs/latest/group__window.html
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    // Window Reference: https://www.glfw.org/docs/latest/group__window.html
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    // Window Reference: https://www.glfw.org/docs/latest/group__window.html
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    // Context Reference: https://www.glfw.org/docs/latest/group__context.html
    glfwMakeContextCurrent(window);

    printf("[Assignment-4] [ENVIRONMENT MAPPING AND SHADOW MAPPING]\n\n");

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "[WARNING] ERROR: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "[SYSTEM INFO] STATUS: USING GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("[SYSTEM INFO] OPENGL VERSION RECIEVED: %d.%d.%d\n", major, minor, rev);
    printf("[SYSTEM INFO] SUPPORTED OPENGL IS %s\n", (const char*)glGetString(GL_VERSION));
    printf("[SYSTEM INFO] SUPPORTED GLSL IS %s\n\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    geometry.init();
    geometry.configShadowMap();
    geometry.bind();
    geometry.addPlane();

    skybox.init();
    skybox.configCubeMap();
    skybox.bind();
    skybox.update();

    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    GLuint uniDepthMap, uniSkybox;
    std::vector<Program> programs(N_SHADER);
    programs[WIREFRAME] = ProgramFactory::createWireframeShader("outColor");
    programs[FLAT] = ProgramFactory::createFlatShader("outColor");
    programs[FLAT].bind();
    uniDepthMap = programs[FLAT].uniform("depthMap");
    glUniform1i(uniDepthMap, 0);
    uniSkybox = programs[FLAT].uniform("skybox");
    glUniform1i(uniSkybox, 1);

    programs[PHONG] = ProgramFactory::createPhongShader("outColor");
    programs[PHONG].bind();
    uniDepthMap = programs[PHONG].uniform("depthMap");
    glUniform1i(uniDepthMap, 0);
    uniSkybox = programs[PHONG].uniform("skybox");
    glUniform1i(uniSkybox, 1);

    programs[SHADOW] = ProgramFactory::createShadowShader("");

    programs[SKYBOX] = ProgramFactory::createSkyboxShader("outColor");
    programs[SKYBOX].bind();
    uniSkybox = programs[SHADOW].uniform("skybox");
    glUniform1i(uniSkybox, 0);


    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)


    // The vertex shader wants the position of the vertices as an input.
    // The following line connects the VBO we defined above with the position "slot"
    // in the vertex shader
    // program.bindVertexAttribArray("position",VBO);

    // Save the current time --- it will be used to dynamically change the triangle color
    auto t_start = std::chrono::high_resolution_clock::now();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    long long int nbFrames = 0;
    long long int newFrames = 0;

    double lastTime = glfwGetTime();
    double newLastTime = glfwGetTime();

    const double maxFPS = 60.0;
    const double maxPeriod = 1.0 / maxFPS;
    int counter = 0;

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_GREATER);
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {

        // Set the uniform value depending on the time difference
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        double newCurrentTime = glfwGetTime();

        double deltaTime = newCurrentTime - newLastTime;
        if (deltaTime >= maxPeriod) {
            newLastTime = newCurrentTime;
            double currentTime = glfwGetTime();
            nbFrames++;
            if (currentTime - lastTime >= 1.0) {
                // printf and reset timer
                ++counter;
                if (counter % 8 == 0) {
                    counter = 0;
                    printf("\n[SYSTEM INFO] STATUS: %f ms/frame, %lld frames/s\n", 1000.0 / double(nbFrames), nbFrames);
                }
                nbFrames = 0;
                lastTime += 1.0;
            }
        }

        // Clear the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        // glClearDepth(0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        geometry.bind();
        geometry.draw(programs, viewcontrol, skybox);

        glDepthFunc(GL_LEQUAL);
        //glDepthMask(GL_FALSE);
        skybox.bind();
        skybox.draw(programs[SKYBOX], viewcontrol);
        // glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        //glDepthMask(GL_TRUE);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    for (auto&& program : programs) {
        program.free();
    }
    geometry.free();
    skybox.free();

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
