#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Include GLM for matrix transformations
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>

// This forces the program to run on the integrated GPU (if available)
extern "C" 
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    // create a shader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    // Check if there is an error in getting the shader

    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        
        std::cout << "Failed to compile " 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
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

    if(!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Physics_Sim", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // Enable depth testing for proper 3D rendering
    glEnable(GL_DEPTH_TEST);

    // Vertex data for a cube (8 vertices, each with x, y, z)
    float positions[24] = {
        // Front face
        -0.5f, -0.5f, -0.5f, // 0: bottom front left
        -0.5f,  0.5f, -0.5f, // 1: top front left
         0.5f, -0.5f, -0.5f, // 2: bottom front right
         0.5f,  0.5f, -0.5f, // 3: top front right

        // Back face
        -0.5f, -0.5f,  0.5f, // 4: bottom back left
        -0.5f,  0.5f,  0.5f, // 5: top back left
         0.5f, -0.5f,  0.5f, // 6: bottom back right
         0.5f,  0.5f,  0.5f  // 7: top back right
    };

    // Corrected indices for 12 triangles (36 indices total)
    unsigned int indices[36] = {
        // Front face
        0, 1, 2,
        2, 1, 3,

        // Left face
        0, 1, 4,
        1, 5, 4,

        // Right face
        2, 3, 7,
        2, 7, 6,

        // Top face
        1, 3, 7,
        1, 7, 5,

        // Bottom face
        0, 2, 6,
        0, 6, 4,

        // Back face
        4, 5, 7,
        4, 7, 6
    };

    // Create and bind the vertex buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // Create and bind the index buffer
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set up vertex attribute pointer (location 0, 3 floats per vertex)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    // Vertex Shader
    std::string vertexShader = 
        "#version 330 core\n"
        "layout(location = 0) in vec3 position;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "out vec2 uv;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "    // For demonstration, derive uv from the vertex position\n"
        "    uv = position.xy + vec2(0.5);\n"
        "}\n";

    // Fragment Shader
    std::string fragmentShader = 
        "#version 330 core\n"
        "in vec2 uv;\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    float pixelSize = 128.0;\n"
        "    vec2 uvPixelated = floor(uv * pixelSize) / pixelSize;\n"
        "    float gradient = uvPixelated.y;\n"
        "    float levels = 32.0;\n"
        "    gradient = floor(gradient * levels) / levels;\n"
        "    color = vec4(1.0, 0.0, gradient, 1.0);\n"
        "}\n";

    // Create and use the shader program
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Set up a static view matrix and projection matrix using GLM.
    // View: simple camera positioned at (1.5, 1.5, 2.0) looking at the origin.
    glm::mat4 view = glm::lookAt(
        glm::vec3(1.5f, 1.5f, 2.0f), // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f), // Target
        glm::vec3(0.0f, 1.0f, 0.0f)  // Up direction
    );
    // Projection: perspective projection
    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),         // Field of view
        640.0f / 480.0f,             // Aspect ratio
        0.1f, 100.0f                // Near and far planes
    );
    // Upload the static matrices to the shader uniforms
    int viewLoc = glGetUniformLocation(shader, "view");
    int projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Get the location of the "model" uniform so we can update it each frame.
    int modelLoc = glGetUniformLocation(shader, "model");

    // Main loop
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the model matrix with rotation around the Y-axis.
        float time = static_cast<float>(glfwGetTime());
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.7f, 1.0f, 0.1f));
        // Optionally, you can add more rotations or translations here.
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Draw the cube using all 36 indices
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
