#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


std::string loadShaderSource(const char* filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        std::cout << "No se pudo abrir papu  :U.  el :" << filepath << "\n";
        return "";
    }


    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}


unsigned int createShaderProgram(
    const char* vertexPath,
    const char* fragmentPath
)
{
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmenSource = loadShaderSource(fragmentPath);

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmenSource.c_str();


    // Vertex Shader

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexCode,
        nullptr
    );

    glCompileShader(vertexShader);


    //
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "ERROR VERTEX SHADER:\n"
            << infoLog << "\n";
    }


    // Fragment Shader

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentCode,
        nullptr
    );

    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            512,
            nullptr,
            infoLog
        );

        std::cerr << "ERROR FRAGMENT SHADER:\n"
            << infoLog << "\n";
    }


    // Shader Program

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);


    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    
        std::cerr << "Error al enlazar shaders:\n" << infoLog << "\n";
    }




    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}




int main()
{

    if (!glfwInit())
    {
        std::cerr << "No se pudo inicializar GLFW\n";
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "OpenGL Playground",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "No se pudo crear la ventana\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "No se pudo inicializar GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::cout << "OpenGL cargado: "
        << glGetString(GL_VERSION)
        << "\n";

    // Vertices del triangulo

    float vertices[] =
    {
         0.0f,  0.5f, 0.0f,   // v�rtice 0
        -0.5f, -0.5f, 0.0f,   // v�rtice 1
         0.5f, -0.5f, 0.0f    // v�rtice 2
    };


    // VBO

    unsigned int VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // VAO

    unsigned int VAO;

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    unsigned int shaderProgram =
        createShaderProgram(
            "shaders/basic.vert",
            "shaders/basic.frag"
        );




    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            3
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}