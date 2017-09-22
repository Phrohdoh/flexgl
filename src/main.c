#include <stdio.h>
#include <stdbool.h>

#include "ui.h"
typedef struct vertex vertex;
typedef struct rgba rgba;
typedef struct widget widget;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLchar *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in float x;"
    "layout (location = 1) in float y;"
    "layout (location = 2) in vec4 color;"
    "uniform mat4 mOrtho;"
    "out vec4 fsColor;"
    "void main() {"
    "    gl_Position = mOrtho * vec4(x, y, 0.0, 1.0);"
    "    fsColor = color;"
    "}";

const GLchar *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 fsColor;"
    "out vec4 color;"
    "void main() {"
    "    color = fsColor;"
    "}";

const uint32_t WIDTH = 1024 / 2;
const uint32_t HEIGHT = 768 / 2;

typedef struct M {
    float m11; float m12; float m13; float m14;
    float m21; float m22; float m23; float m24;
    float m31; float m32; float m33; float m34;
    float m41; float m42; float m43; float m44;
} M;

static M matrix_identity() {
    M m = {
        .m11 = 1.0, .m12 = 0.0, .m13 = 0.0, .m14 = 0.0,
        .m21 = 0.0, .m22 = 1.0, .m23 = 0.0, .m24 = 0.0,
        .m31 = 0.0, .m32 = 0.0, .m33 = 1.0, .m34 = 0.0,
        .m41 = 0.0, .m42 = 0.0, .m43 = 0.0, .m44 = 1.0,
    };

    return m;
}

static M matrix_ortho(float left, float right, float bottom, float top, float near, float far) {
    M m = matrix_identity();
    m.m11 = 2.0f / (right - left);
    m.m22 = 2.0 / (top - bottom);
    m.m33 = -2.0 / (far - near);
    m.m41 = -((right + left) / (right - left));
    m.m42 = -((top + bottom) / (top - bottom));
    m.m43 = -((far + near) / (far - near));
    return m;
}

void key_cb(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char** argv) {
    float x = 10.0f;
    float y = 10.0f;
    float w = WIDTH - x * 2.0;
    float h = HEIGHT - y * 2.0;

    rgba color1 = { 0.0f, 1.0f, 0.0f, 1.0f };
    widget *wgt1 = widget_new(x, y, w, h, color1);
    if (wgt1 == NULL) {
        printf("Failed to create wgt1\n");
        return 1;
    }

    //rgba color2 = { 1.0f, 0.0f, 1.0f, 1.0f };
    //widget *wgt2 = widget_new(w, y, h, x, color2);
    //if (wgt2 == NULL) {
    //    printf("Failed to create wgt2\n");
    //    return 1;
    //}

    widget *wgts[] = { wgt1
    //  , wgt2
    };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "FlexGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_cb);

    if (glewInit() != GLEW_OK) {
        printf("Failed to init GLEW\n");
        return 1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    GLenum err;
    glViewport(0, 0, width, height);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    {
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);

        if (infoLog[0] != 0) {
            if (!success) {
                printf("Vertex shader compilation failed!\n");
            }

            printf("Vertex shader info: %s\n", infoLog);
        }
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    {
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);

        if (infoLog[0] != 0) {
            if (!success) {
                printf("Fragment shader compilation failed!\n");
            }

            printf("Fragment shader info: %s\n", infoLog);
        }
    }

    GLuint shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

        if (infoLog[0] != 0) {
            if (!success) {
                printf("Program linking failed!\n");
            }

            printf("Program linking info: %s\n", infoLog);
        }
    }

    glUseProgram(shaderProgram);

    glDetachShader(shaderProgram, vertexShader);
    glDeleteShader(vertexShader);

    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(fragmentShader);

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    // TODO: Only put vertex data in the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(widget), wgt1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(offsetof(widget, tl.x)));
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(offsetof(widget, tl.y)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(rgba), (GLvoid*)(offsetof(widget, tl_rgba.r)));

    // Unbind VAO
    glBindVertexArray(0);

    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("glGetError(bind buffer objs): %x\n", err);
        return 1;
    }

    M ortho = matrix_ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -10.0f, 10.0f);
    GLint loc_mat_ortho = glGetUniformLocation(shaderProgram, "mOrtho");
    glUniformMatrix4fv(loc_mat_ortho, 1, false, (GLvoid const *)&ortho);

    glBindVertexArray(VAO); // Bind VAO

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, NULL);

        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glGetError(render): %x\n", err);
            return 1;
        }

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    widget_free(wgts[0]);
//  widget_free(wgts[1]);

    return 0;
}
