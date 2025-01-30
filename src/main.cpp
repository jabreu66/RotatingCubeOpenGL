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


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    float positions[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    // Generates a buffer and gives us back an ID
    glGenBuffers(1, &buffer);

    // Select that buffer using bindBuffer
    //GL_ARRAY_BUFFER tells us that this is an array
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Creates and initializes a buffer object's data store
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // Loop until we decide to close the window
    while(!glfwWindowShouldClose(window))
    {
        // render screen
        glClear(GL_COLOR_BUFFER_BIT); 

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate(); // Destroys remaining windows and releases resources alloc by GLFW



    // Initialize all OpenGL pointers required for this program to interact w/ OpenGL API
    // We need this line or we wouldn't be able to call OpenGL functions beyond the basic ones, such as funcs for creating shaders, buffers, etc...
   
    return 0;
}