#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        1300,
        900,
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


    // Vertices de dos triangulos para cuadrado
    //float vertices[] =
    //{
    //    // Posición          // Color RGB
    //     0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 
    //     1.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 
    //     0.0f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f,

    //     //segundo triangulo
    //     0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
    //     1.0f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
    //     1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f
    //};
 
    // Cuadrado utilizando el solapamiento
    float cuadrado[] = {
        // Posición          // Color
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 0: arriba izquierda
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 1: arriba derecha
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 2: abajo derecha
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // 3: abajo izquierda
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };



    // Configuración Buffers

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(cuadrado),
        cuadrado,
        GL_STATIC_DRAW
    );

    // Asociación del EBO al VAO

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    unsigned int shaderProgram =
    createShaderProgram(
        "shaders/basic.vert",
        "shaders/basic.frag"
    );

    // Operaciones en el triangulo


    
    int modelLoc = glGetUniformLocation(shaderProgram, "model");


    // Variable para cambiar de estado fill/line

    bool wireframe = false;
    bool wWasPressed = false;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);



        glm::mat4 model = glm::mat4(1.0f);

        float time = static_cast<float>(glfwGetTime());

        // Traslación
        model = glm::translate(
            model,
            glm::vec3(0.0f, 0.0f, 0.0f)
        );

        // Rotación sobre el eje x
        model = glm::rotate(
            model,
            time,
            glm::vec3(1.0f, 0.0f, 1.0f)
        );

        glUniformMatrix4fv(
            modelLoc,
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );



        glBindVertexArray(VAO);


        // Detección de presionado de tecla "w"
        bool wIsPressed = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;

        if (wIsPressed && !wWasPressed)
        {
            wireframe = !wireframe;
        }

        wWasPressed = wIsPressed;

        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glDrawArrays(
        //    GL_TRIANGLES,
        //    0,
        //    4 //Numero de vertices a dibujar 
        //);

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