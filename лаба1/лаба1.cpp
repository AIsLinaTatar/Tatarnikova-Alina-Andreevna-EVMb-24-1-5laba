#define GLEW_DLL
#define GLFW_DLL

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stdio.h"
#include "stdlib.h"
#include "cmath"
#include "LIB.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;

float lastX = 256.0f;
float lastY = 256.0f;
bool firstMouse = true;

float cameraSpeed = 2.5f;
float sensitivity = 0.1f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float rotationAngle = 0.0f;
float rotationSpeed = 50.0f;
glm::vec3 diagonalAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));

/*float vertex[] = {
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

};*/

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewExperimental = GL_TRUE;

    glEnable(GL_DEPTH_TEST);

    GLenum ret = glewInit();
    if (GLEW_OK != ret) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(ret));
        return 1;
    }
    
    /*GLuint VBO, VAO, EBO;
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

    GLuint shader_Program = createShaderProgramFromFiles(
        "vertex_shader.glsl",
        "fragment_shader.glsl"
    );

    if (shader_Program == 0) {
        fprintf(stderr, "Ошибка: не удалось создать программу шейдера!\n");
        return -1;
    }
    Model ourRTC("Lab_3_VAR_1.obj");
    GLint modelLoc = glGetUniformLocation(shader_Program, "model");
    GLint viewLoc = glGetUniformLocation(shader_Program, "view");
    GLint projLoc = glGetUniformLocation(shader_Program, "projection");
    GLint colorLoc = glGetUniformLocation(shader_Program, "ourColor");

    glm::mat4 model = glm::mat4(1.0f);

    /*glAttachShader(shader_Program, vert_Shader);
    glAttachShader(shader_Program, frag_Shader);

    glLinkProgram(shader_Program);*/

    while (!glfwWindowShouldClose(window)) {

        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.5, 0.2, 0.7, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

        rotationAngle += rotationSpeed * deltaTime;
        if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotationAngle), diagonalAxis);

        glUseProgram(shader_Program);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        float timeValue = (float)glfwGetTime();
        float r = (sinf(timeValue) + 1.0f) / 2.0f;
        float g = (sinf(timeValue + 2.0f) + 1.0f) / 2.0f;
        float b = (sinf(timeValue + 4.0f) + 1.0f) / 2.0f;
        glUniform4f(colorLoc, r, g, b, 1.0);

        ourRTC.Draw();
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

void processInput(GLFWwindow* window) {
    float speed = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;   
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
