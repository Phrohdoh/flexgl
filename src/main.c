#include <stdio.h>
#include <stdbool.h>

#include "ui.h"
typedef struct widget widget;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLchar *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos;"
    "layout (location = 1) in float r;"
    "layout (location = 2) in float g;"
    "layout (location = 3) in float b;"
    "layout (location = 4) in float a;"
    "uniform mat4 mOrtho;"
    "out vec4 fsColor;"
    "void main() {"
    "    gl_Position = mOrtho * vec4(pos, 0.0, 1.0);"
    "    fsColor = vec4(r, g, b, a);"
    "}";

const GLchar *fragmentShaderSource =
    "#version 330 core\n"
    "in vec4 fsColor;"
    "out vec4 color;"
    "void main() {"
    "    color = fsColor;"
    "}";

const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 768;

typedef struct Vertex Vertex;
struct Vertex {
    float pos[2];
    float r;
    float g;
    float b;
    float a;
};

static Vertex _create_vert(float x, float y, float r, float g, float b, float a) {
    Vertex vv = { { x, y }, r, g, b, a };
    return vv;
}

static void create_4_verts(Vertex *verts, float x, float y, float w, float h, float r, float g, float b, float a) {
    verts[0] = _create_vert(x + 0.0f, y + 0.0f, r, g, b, a);
    verts[1] = _create_vert(x + 0.0f, y +    h, r, g, b, a);
    verts[2] = _create_vert(x +    w, y +    h, r, g, b, a);
    verts[3] = _create_vert(x +    w, y + 0.0f, r, g, b, a);
}

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
    float y = 25.0f;
    float w = 80.0f;
    float h = 120.0f;

    // These values must be normalized between 0.0 and 1.0
    float r = 0.2f;
    float g = 0.7f;
    float b = 0.1f;
    float a = 1.0f;

    widget *wgt = ui_widget_new(x, y, w, h);

    // TODO: Create a Quad type and impl quad_init(quads, x, y, w, h)

    Vertex verts[4];
    create_4_verts(verts, x, y, w, h, r, g, b, a);

    ui_widget_free(wgt);

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
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_cb);

    if (glewInit() != GLEW_OK) {
        printf("Failed to init GLEW\n");
        return -1;
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

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Store the pos in attrib 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, pos)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, r)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, g)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, b)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, a)));

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

        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        if ((err = glGetError()) != GL_NO_ERROR) {
            printf("glGetError(render): %x\n", err);
            return 1;
        }

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
