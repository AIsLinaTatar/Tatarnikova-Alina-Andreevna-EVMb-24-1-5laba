#pragma once

#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>

GLuint compileShader(const char* source, GLenum type, const char* typeName);
std::string readShaderFile(const char* filePath);
GLuint createShaderProgramFromFiles(const char* vertexPath, const char* fragmentPath);
GLuint createShaderProgram(const char* vertexCode, const char* fragmentCode);
void setUniformFloat(GLuint program, const char* name, float v0);
void setUniformInt(GLuint program, const char* name, int v0);
void setUniformVec2(GLuint program, const char* name, float v0, float v1);
void setUniformVec3(GLuint program, const char* name, float v0, float v1, float v2);
void setUniformVec4(GLuint program, const char* name, float v0, float v1, float v2, float v3);

GLuint compileShader(const char* source, GLenum type, const char* typeName) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << typeName << "\n" << infoLog << "\n";
    }

    return shader;
}

std::string readShaderFile(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Не удалось открыть файл шейдера: " << filePath << std::endl;
        return "";
    }

    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    content = stringStream.str();
    fileStream.close();

    return content;
}

GLuint createShaderProgramFromFiles(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Ошибка: не удалось прочитать файлы шейдеров!" << std::endl;
        return 0;
    }

    GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER, "VERTEX");
    GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

GLuint createShaderProgram(const char* vertexCode, const char* fragmentCode) {
    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER, "VERTEX");
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER, "FRAGMENT");

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void setUniformFloat(GLuint program, const char* name, float v0) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1f(location, v0);
}

void setUniformInt(GLuint program, const char* name, int v0) {
    GLint location = glGetUniformLocation(program, name);
    glUniform1i(location, v0);
}

void setUniformVec2(GLuint program, const char* name, float v0, float v1) {
    GLint location = glGetUniformLocation(program, name);
    glUniform2f(location, v0, v1);
}

void setUniformVec3(GLuint program, const char* name, float v0, float v1, float v2) {
    GLint location = glGetUniformLocation(program, name);
    glUniform3f(location, v0, v1, v2);
}

void setUniformVec4(GLuint program, const char* name, float v0, float v1, float v2, float v3) {
    GLint location = glGetUniformLocation(program, name);
    glUniform4f(location, v0, v1, v2, v3);
}