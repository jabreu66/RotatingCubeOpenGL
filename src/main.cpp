#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    GLFWwindow* window;

    // Before we can use GLFW functions, the library has to be initialized
    // If we are successful, GFLW_TRUE is returned, else false

    

    if(!glfwInit()) // if we fail to init our library, return -1
    {
        return -1;
    }

   /* // If a GLFW function fails, we report an error to the error callback

    void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }*/

    // Function to set the error callback is one of the few GLFW functions that can be called BEFORE initialization

   // glfwSetErrorCallback(error_callback);

    //Create the window using a single call to glfwCreateWindow (width, height, title, monitor(for fullscreen or NULL for windowed), share (window whose context to share resorurces with, NULL to not share))
    // Note that GLFWwindow is the TYPE here.
    window = glfwCreateWindow(640, 480, "Physics_Sim", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    // Before can use the OpenGL API, we must have a current OpenGL context
    // This context will REMAIN current until we make another context current or the window owning the context is destroyed
    glfwMakeContextCurrent(window);

    // Loop until we decide to close the window
    while(!glfwWindowShouldClose(window))
    {
        // render screen
       // glClear(GL_COLOR_BUFFER_BIT); --> This line was causing issues

        // swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate(); // Destroys remaining windows and releases resources alloc by GLFW



   /* // Initialize all OpenGL pointers required for this program to interact w/ OpenGL API
    // We need this line or we wouldn't be able to call OpenGL functions beyond the basic ones, such as funcs for creating shaders, buffers, etc...
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);*/

    return 0;
}