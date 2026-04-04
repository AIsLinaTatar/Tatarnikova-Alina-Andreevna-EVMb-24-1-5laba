#define GLEW_DLL
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include "stdlib.h"
#include "cmath"
#include "LIB.h"
 
float vertex[] = { 
    0.0, 0.5, 0.0,
    0.25, 0.0, 0.0,
    -0.25, 0, 0.0,
    0.5, 0, 0.0,
    0, 0.25, 0.0,
    0, -0.25, 0.0,
    0, -0.5, 0.0,
    0.25, 0, 0.0,
    -0.25, 0, 0.0,
    -0.5, 0, 0.0,
    0, 0.25, 0.0,
    0.0, -0.25, 0.0 };

GLuint indices[] = {
    0, 1, 2,
    3, 4, 5, 
    6, 7, 8,
    9, 10, 11

};

int main() {

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(512, 512, "Mainwindow", NULL, NULL);
   
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(ret));
        return 1;
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   
    glBindVertexArray(0);    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   
    const char* vert_shader =
        "#version 410 core\n"
        "layout (location=0) in vec3 vp;"
        "void main(){"
        "gl_Position = vec4 (vp, 1.0);"
        "}";

    const char* frag_shader =
        "#version 410 core\n"
        "out vec4 frag_colour;"
        "uniform vec4 ourColor;"
        "void main(){"
        "frag_colour = ourColor;"
        "}";

    /*GLuint vert_Shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag_Shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert_Shader, 1, &vert_shader, NULL);
    glShaderSource(frag_Shader, 1, &frag_shader, NULL);

    glCompileShader(vert_Shader);
    glCompileShader(frag_Shader);*/

    GLuint shader_Program = createShaderProgram(vert_shader, frag_shader);

    /*glAttachShader(shader_Program, vert_Shader);
    glAttachShader(shader_Program, frag_Shader);

    glLinkProgram(shader_Program);*/

    float vertexColorLocation = glGetUniformLocation(shader_Program, "ourColor");

        while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5, 0.2, 0.7, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        /*glBegin(GL_TRIANGLES);
        glVertex2f(0, 0.5);
        glVertex2f(0.25, 0);
        glVertex2f(-0.25, 0);
        glVertex2f(0.5, 0);
        glVertex2f(0, 0.25);
        glVertex2f(0, -0.25);
        glVertex2f(0, -0.5);
        glVertex2f(0.25, 0);
        glVertex2f(-0.25, 0);
        glVertex2f(-0.5, 0);
        glVertex2f(0, 0.25);
        glVertex2f(0, -0.25);
        glColor3f(1.0, 1.0, 1.0);
        glEnd();*/

        float timeValue = (float)glfwGetTime();
        float r = (sinf(timeValue) + 1.0f) / 2.0f;      
        float g = (sinf(timeValue + 2.0f) + 1.0f) / 2.0f;
        float b = (sinf(timeValue + 4.0f) + 1.0f) / 2.0f;

        glUseProgram(shader_Program);
        glUniform4f(vertexColorLocation, r, g, b, 1.0);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    const GLubyte* version_str = glGetString(GL_VERSION);
    const GLubyte* device_str = glGetString(GL_RENDER);

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("This version OpenGL running in %s\n", version_str);
    printf("This device OpenGL running is %s\n", device_str);
        glfwTerminate();
        return 0;
}