#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "escenas.h"

namespace {

constexpr int anchoVentana = 960;
constexpr int altoVentana = 720;

std::string leerArchivo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    std::ostringstream contenido;
    contenido << archivo.rdbuf();
    return contenido.str();
}

GLuint compilarShader(GLenum tipo, const std::string& fuente) {
    GLuint shader = glCreateShader(tipo);
    const char* codigo = fuente.c_str();
    glShaderSource(shader, 1, &codigo, nullptr);
    glCompileShader(shader);

    GLint exito = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &exito);
    if (exito == GL_FALSE) {
        GLint longitud = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &longitud);
        std::string registro(static_cast<std::size_t>(longitud), '\0');
        glGetShaderInfoLog(shader, longitud, nullptr, registro.data());
        glDeleteShader(shader);
        throw std::runtime_error("Error al compilar shader: " + registro);
    }

    return shader;
}

GLuint crearPrograma(const std::string& rutaVertice, const std::string& rutaFragmento) {
    const std::string shaderVertice = leerArchivo(rutaVertice);
    const std::string shaderFragmento = leerArchivo(rutaFragmento);

    const GLuint vertice = compilarShader(GL_VERTEX_SHADER, shaderVertice);
    const GLuint fragmento = compilarShader(GL_FRAGMENT_SHADER, shaderFragmento);

    GLuint programa = glCreateProgram();
    glAttachShader(programa, vertice);
    glAttachShader(programa, fragmento);
    glLinkProgram(programa);

    GLint exito = GL_FALSE;
    glGetProgramiv(programa, GL_LINK_STATUS, &exito);
    if (exito == GL_FALSE) {
        GLint longitud = 0;
        glGetProgramiv(programa, GL_INFO_LOG_LENGTH, &longitud);
        std::string registro(static_cast<std::size_t>(longitud), '\0');
        glGetProgramInfoLog(programa, longitud, nullptr, registro.data());
        glDeleteProgram(programa);
        glDeleteShader(vertice);
        glDeleteShader(fragmento);
        throw std::runtime_error("Error al enlazar programa: " + registro);
    }

    glDeleteShader(vertice);
    glDeleteShader(fragmento);
    return programa;
}

void cargarEscena(GLuint vbo, const BufferVertices& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertice)),
        vertices.data(),
        GL_STATIC_DRAW);
}

}

int main() {
    if (!glfwInit()) {
        std::cerr << "No se pudo inicializar GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(
        anchoVentana,
        altoVentana,
        "Practica 3: lineas, circulos e intersecciones",
        nullptr,
        nullptr);
    if (ventana == nullptr) {
        std::cerr << "No se pudo crear la ventana.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(ventana);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "No se pudo inicializar GLEW.\n";
        glfwDestroyWindow(ventana);
        glfwTerminate();
        return -1;
    }

    GLuint programa = 0;
    GLuint vao = 0;
    GLuint vbo = 0;

    try {
        programa = crearPrograma("shader.vert", "shader.frag");
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        glfwDestroyWindow(ventana);
        glfwTerminate();
        return -1;
    }

    std::vector<BufferVertices> escenas = {
        crearEscenaFigura(),
        crearEscenaIntersecciones()
    };

    int escenaActual = 0;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertice), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertice),
        reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    cargarEscena(vbo, escenas[escenaActual]);

    const glm::mat4 proyeccion = glm::ortho(
        0.0f,
        static_cast<float>(anchoVentana),
        0.0f,
        static_cast<float>(altoVentana));

    std::cout << "Tecla 1: figura compuesta con lineas y circulos\n";
    std::cout << "Tecla 2: cinco circulos aleatorios con intersecciones\n";
    std::cout << "Tecla R: regenerar la escena aleatoria\n";

    while (!glfwWindowShouldClose(ventana)) {
        if (glfwGetKey(ventana, GLFW_KEY_1) == GLFW_PRESS && escenaActual != 0) {
            escenaActual = 0;
            cargarEscena(vbo, escenas[escenaActual]);
        }

        if (glfwGetKey(ventana, GLFW_KEY_2) == GLFW_PRESS && escenaActual != 1) {
            escenaActual = 1;
            escenas[1] = crearEscenaIntersecciones();
            cargarEscena(vbo, escenas[escenaActual]);
        }

        if (glfwGetKey(ventana, GLFW_KEY_R) == GLFW_PRESS && escenaActual == 1) {
            escenas[1] = crearEscenaIntersecciones();
            cargarEscena(vbo, escenas[escenaActual]);
        }

        glViewport(0, 0, anchoVentana, altoVentana);
        glClearColor(0.96f, 0.97f, 0.99f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programa);
        glUniformMatrix4fv(
            glGetUniformLocation(programa, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(proyeccion));

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(escenas[escenaActual].size()));

        glfwSwapBuffers(ventana);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programa);
    glfwDestroyWindow(ventana);
    glfwTerminate();
    return 0;
}
