#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include <iostream>
#include <format>

#include "guidialog.h"


bool rotateflag = false;


const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;
    uniform float angle;
    void main()
    {
        float x = position.x * cos(angle) - position.y * sin(angle);
        float y = position.x * sin(angle) + position.y * cos(angle);
        gl_Position = vec4(x, y, 0.0, 1.0);

    }
)";


const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";

//Размеры окна
const GLuint WIDTH = 1920, HEIGHT = 1080;
GLuint shaderProgram, VBO, VAO;



// Создание буфера вершин и массива атрибутов вершин
// Определение вершин отрезка
float vertices1[] = {
    -0.5f, -0.5f,
    0.5f,  0.8f
};

bool CreateData(GLuint &shaderProgram, GLuint &VBO, GLuint &VAO)
{
    // Компиляция и связывание шейдеров
    // Создание вершинного шейдера
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Создание фрагментного шейдера
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Создание шейдерной программы
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Удаление шейдеров
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}



double normalize(double value, double min, double max) {
    return (2 * (value - min) / (max - min)) - 1;
}

void mouseKey (GLFWwindow * window, int button, int action, int mode)
{
    double x, y;
    glfwGetCursorPos ( window, &x, &y );

    double x_norm = normalize(x, 0, WIDTH); // Normalizing x to range [-1, 1]
    double y_norm = -1 * normalize(y, 0, HEIGHT);

    bool flagUpdateData = false;
    std::string MouseCoord = std::format("Mouse coord is X:{} Y:{}", x_norm, y_norm);
    if ( button == GLFW_MOUSE_BUTTON_MIDDLE  && action == GLFW_PRESS )
    {
        vertices1[0] = x_norm;
        vertices1[1] = y_norm;
        std::cout << MouseCoord <<std::endl;
        flagUpdateData = true;
    }
    if ( button == GLFW_MOUSE_BUTTON_RIGHT  && action == GLFW_PRESS )
    {
        vertices1[2] = x_norm;
        vertices1[3] = y_norm;
        std::cout << MouseCoord <<std::endl;
        flagUpdateData = true;
    }
    if (flagUpdateData == true)
    {
        CreateData(shaderProgram,VBO,VAO);
    }
}


int main()
{

    //Инициализация GLFW
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);                // 4x Сглаживание
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //Версия OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Не используем старый OpenGL


    //Создание окна приложения
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW Create Line and Rotate", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback (window, mouseKey);

    //Инициализация окна ImGui
    guiDialog objGui (window);


    // Инициализация GLEW
    glewExperimental = true; // Флаг необходим в Core-режиме OpenGL
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    //Режим отслеживания нажатия клавиш в приложении
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    //Основной цикл, при нажатии на ECS или закрытии приложения работа завершается
    float angle = 0;


    CreateData(shaderProgram, VBO, VAO);

    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           !glfwWindowShouldClose(window))
    {
        //Очиста буфера цвета с одновременным определением цвета фона
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_LINES, 0, 2);

        if (rotateflag == true)
        {
            objGui.SetBufferCoord(vertices1,4);
           // Установка угла вращения
           angle += 0.005;
           glUniform1f(glGetUniformLocation(shaderProgram, "angle"), angle);
        }
        //Создание окна ImGui
        objGui.CreateGuiDialog(rotateflag);
        //Отображение ImGui
        objGui.ShowGui();

        //Обмен данными между передним и задним буфером
        glfwSwapBuffers(window);
        //Опрос и обработка событий
        glfwPollEvents();
    }


    //Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //Уничтожение контекста GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
