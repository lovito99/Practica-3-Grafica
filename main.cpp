#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
#include <iostream>

// Estructura para puntos y contenedor global
struct Point { float x, y; };
std::vector<Point> allPoints;

// Función de trazado (plot) de la guía
void plot(int x, int y) {
    allPoints.push_back({(float)x, (float)y});
}

// --- IMPLEMENTACIÓN DE ALGORITMOS DE LA GUÍA ---

void circulo2vias(int x0, int y0, int r) {
    plot(x0 + r, y0); plot(x0 - r, y0);
    for (int x = -r + 1; x < r; x++) {
        int y = (int)floor(sqrt(r * r - x * x) + 0.5);
        plot(x0 + x, y0 + y); plot(x0 + x, y0 - y);
    }
}

void circulo4vias(int x0, int y0, int r) {

//COMPLETAR CóDIGO


}

void circulo8vias(int x0, int y0, int r) {
//COMPLETAR CóDIGO









}

void circuloPtoMedio(int x0, int y0, int r) {

//COMPLETAR CóDIGO




}

// --- UTILIDADES DE SHADER Y RENDER ---

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(600, 600, "Practica 3: Circunferencias", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    // Generar puntos con los algoritmos
    circulo2vias(150, 450, 40);
    circulo4vias(300, 450, 40);
    circulo8vias(450, 450, 40);
    circuloPtoMedio(300, 250, 100);

    // Configurar Buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allPoints.size() * sizeof(Point), allPoints.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    // Shaders simples embebidos para facilitar ejecución
    const char* vShaderSrc = "#version 330 core\nlayout(location=0) in vec2 p; uniform mat4 pr; void main(){gl_Position=pr*vec4(p,0,1);}";
    const char* fShaderSrc = "#version 330 core\nout vec4 fc; uniform vec3 c; void main(){fc=vec4(c,1);}";
    
    GLuint vs = compileShader(GL_VERTEX_SHADER, vShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fShaderSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs); glAttachShader(program, fs);
    glLinkProgram(program);

    glm::mat4 projection = glm::ortho(0.0f, 600.0f, 0.0f, 600.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "pr"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(program, "c"), 0.0f, 0.4f, 0.8f); // Azul

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, allPoints.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
