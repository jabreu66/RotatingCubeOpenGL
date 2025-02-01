#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// This forces the program to run on the integrated GPU
// This makes sure that it doesn't keep closing by itself.
extern "C" 
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type); // creates a shader
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // replaces the source code in a shader object
    glCompileShader(id); // compiles a shader

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // queries the compile status and returns it in result

    // If result is false, the shader did not compile successfully
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length * sizeof(char)); // alloca -- allocates memory on the stack
        glGetShaderInfoLog(id, length, &length, message);
        
        std::cout << "Failed to compile " << 
        (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
         " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }


    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{ 
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(fs);
    glDeleteShader(vs);

    return program;
}

void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);

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

    float positions[8] = {
       -0.5f, -0.5f,
        -0.5f,  0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f

    };

    unsigned int indices[6] = {
        0,1,2, // Triangle ABC
        2,1,3
    };

    unsigned int buffer;
    // Generates a buffer and gives us back an ID
    glGenBuffers(1, &buffer);

    // Select that buffer using bindBuffer
    //GL_ARRAY_BUFFER tells us that this is an array
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Creates and initializes a buffer object's data store
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    unsigned indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;"
    "void main()"
    "{\n"
    " gl_Position = position;\n"
    "}\n";
    std::string fragmentShader = 
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;"
    "void main()"
    "{\n"
    " color = vec4(0.0, 0.0, 1.0, 1.0);\n" //r,g,b, and a (alpha)
    "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    // Loop until we decide to close the window
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        // render screen
        glClear(GL_COLOR_BUFFER_BIT); 

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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