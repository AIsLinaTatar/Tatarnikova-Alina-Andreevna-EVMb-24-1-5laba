#define GLEW_DLL
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5, 0.2, 0.7, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
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
        glEnd();
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