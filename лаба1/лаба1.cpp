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
#include <windows.h>

glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos; lastY = ypos; firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos; lastY = ypos;
    xoffset *= 0.1f; yoffset *= 0.1f;
    yaw += xoffset; pitch += yoffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(dir);
}

void processInput(GLFWwindow* window, float deltaTime) {
    float speed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= speed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

glm::vec3 centerBase = glm::vec3(0.000000f, -0.486263f, 1.326860f);
glm::vec3 centerArm = glm::vec3(0.2279f, 0.130384f, 0.51433f);
glm::vec3 centerTube = glm::vec3(0.000000f, 0.536150f, 2.096110f);

float baseRotate = 0.0f;
float armPitch = 0.0f;
float tubeShiftZ = 0.0f;

int main() {
    SetConsoleOutputCP(1251);

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3.\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Lab7 Variant 19", NULL, NULL);

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

    GLuint shader_Program = createShaderProgramFromFiles(
        "vertex_shader.glsl",
        "fragment_shader.glsl"
    );

    if (shader_Program == 0) {
        fprintf(stderr, "Ошибка: не удалось создать программу шейдера!\n");
        return -1;
    }

    Model ourRTC("Lab_3_VAR_1.obj");

    glm::vec3 lightPos = glm::vec3(2.0f, 3.0f, 4.0f);
    glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 lightDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        deltaTime = now - lastFrame;
        lastFrame = now;
        float angleSpeed = 50.0f * deltaTime;

        processInput(window, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) baseRotate += angleSpeed;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) baseRotate -= angleSpeed;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) armPitch += angleSpeed;
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) armPitch -= angleSpeed;
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) tubeShiftZ += 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) tubeShiftZ -= 2.5f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        if (armPitch > 30.0f) armPitch = 30.0f;
        if (armPitch < -10.0f) armPitch = -10.0f;
        if (tubeShiftZ < -0.5f) tubeShiftZ = -0.5f;
        if (tubeShiftZ > 0.0f) tubeShiftZ = 0.0f;

        glm::mat4 baseMatrix = glm::mat4(1.0f);
        baseMatrix = glm::translate(baseMatrix, centerBase);
        baseMatrix = glm::rotate(baseMatrix, glm::radians(baseRotate), glm::vec3(0, 1, 0));
        baseMatrix = glm::translate(baseMatrix, -centerBase);

        glm::mat4 armMatrix = baseMatrix;
        armMatrix = glm::translate(armMatrix, centerArm);
        armMatrix = glm::rotate(armMatrix, glm::radians(armPitch), glm::vec3(0, 1, 0));
        armMatrix = glm::translate(armMatrix, -centerArm);

        glm::mat4 tubeMatrix = armMatrix;
        tubeMatrix = glm::translate(tubeMatrix, centerTube);
        tubeMatrix = glm::translate(tubeMatrix, glm::vec3(0, 0, tubeShiftZ));
        tubeMatrix = glm::translate(tubeMatrix, -centerTube);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glClearColor(0.5f, 0.2f, 0.7f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_Program);

        glUniformMatrix4fv(glGetUniformLocation(shader_Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader_Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glUniform3fv(glGetUniformLocation(shader_Program, "light.position"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shader_Program, "light.ambient"), 1, glm::value_ptr(lightAmbient));
        glUniform3fv(glGetUniformLocation(shader_Program, "light.diffuse"), 1, glm::value_ptr(lightDiffuse));
        glUniform3fv(glGetUniformLocation(shader_Program, "light.specular"), 1, glm::value_ptr(lightSpecular));

        glUniform3fv(glGetUniformLocation(shader_Program, "viewPos"), 1, glm::value_ptr(cameraPos));

        glUniform3fv(glGetUniformLocation(shader_Program, "material.specular"), 1, glm::value_ptr(materialSpecular));
        glUniform1f(glGetUniformLocation(shader_Program, "material.shininess"), shininess);

        float timeValue = now;
        float r = (sinf(timeValue) + 1.0f) / 2.0f;
        float g = (sinf(timeValue + 2.0f) + 1.0f) / 2.0f;
        float b = (sinf(timeValue + 4.0f) + 1.0f) / 2.0f;

        glm::vec3 materialAmbient = glm::vec3(r * 0.3f, g * 0.3f, b * 0.3f);
        glm::vec3 materialDiffuse = glm::vec3(r, g, b);

        glUniform3fv(glGetUniformLocation(shader_Program, "material.ambient"), 1, glm::value_ptr(materialAmbient));
        glUniform3fv(glGetUniformLocation(shader_Program, "material.diffuse"), 1, glm::value_ptr(materialDiffuse));

        ourRTC.DrawPart(0, shader_Program, baseMatrix);
        ourRTC.DrawPart(1, shader_Program, armMatrix);
        ourRTC.DrawPart(2, shader_Program, tubeMatrix);

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

