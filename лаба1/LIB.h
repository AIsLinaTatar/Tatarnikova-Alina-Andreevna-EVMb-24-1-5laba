#pragma once

#pragma once

#include <iostream>
#include <GL/glew.h>

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