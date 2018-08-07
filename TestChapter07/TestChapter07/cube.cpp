#include "vgl.h"
#include "vmath.h"
#include "LoadShaders.h"
#include <iostream>
using namespace std;

char *ShaderVert =
" #version 430 core \n"
" uniform mat4 matModel;\n"
" uniform mat4 matPorjectView;\n"
" \n"
" layout(location = 0) in vec4 pos;\n"
" layout(location = 1) in vec4 vVertColor;\n"
" \n"
" out vec4 color;\n"
" \n"
" void main()\n"
" {\n"
"     gl_Position = matPorjectView * (matModel * pos);\n"
"     color = vVertColor;\n"
" \n"
" }\n"
" \n"
;

char *ShaderFrag =
"#version 430 core \n"
" \n"
" in vec4 color;\n"

" out vec4 FragColor;\n"
" \n"
" void main()\n"
" {\n"
"     FragColor = color;\n"
" \n"
" }\n"
" \n"
" \n"
" \n"
;


float Vertices[] = {
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
};

float color[] = {
     1.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  1.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  1.0f,
     0.0f,  1.0f,  1.0f,
     0.5f,  0.0f,  0.5f,
     0.0f,  0.5f,  1.0f,
};

unsigned int VertIndex[]
{
    1, 2, 0, 3, 4, 7, 5, 6,
    0, 4, 1, 5, 2, 6, 3, 7
};

GLuint Program;

GLuint verts;
GLuint vIo;
GLuint vBo;
GLuint vCo;

GLuint CreateShader(GLenum Type, const char* sorce)
{
    GLuint shader;
    shader = glCreateShader(Type);

    glShaderSource(shader, 1, &sorce, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (0 == status)
    {
        char log[1000] = { 0 };
        GLint len = 0;
        glGetShaderInfoLog(shader, sizeof(log), &len, log);
        cout<< Type<<": " << log << endl;
        return -1;
    }
    return shader;
}

GLuint ULocModel;
GLuint ULocPorjView;

void init()
{
    Program = glCreateProgram();

    GLuint Shaders;
    Shaders = CreateShader(GL_VERTEX_SHADER, ShaderVert);
    glAttachShader(Program, Shaders);
    glDeleteShader(Shaders);
    Shaders = CreateShader(GL_FRAGMENT_SHADER, ShaderFrag);
    glAttachShader(Program, Shaders);
    glDeleteShader(Shaders);
    glLinkProgram(Program);

    glGenBuffers(1, &vIo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(VertIndex), VertIndex, GL_STATIC_DRAW);

    glGenVertexArrays(1, &verts);
    glBindVertexArray(verts);
    glGenBuffers(1, &vBo);
    glBindBuffer(GL_ARRAY_BUFFER, vBo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vCo);
    glBindBuffer(GL_ARRAY_BUFFER, vCo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);


    ULocModel = glGetUniformLocation(Program, "matModel");
    ULocPorjView = glGetUniformLocation(Program, "matPorjectView");



    glClearColor(0.1, 0.2, 0.1, 1.0);
}

void display()
{
    static unsigned int Time = GetTickCount();

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    vmath::mat4 modle = vmath::mat4::identity();
    vmath::mat4 ProjView = vmath::mat4::identity();

    modle *= vmath::scale(0.2f, 0.2f, 0.2f);

    unsigned int timeNow = GetTickCount();
    float dt = float((timeNow - Time)%8000) / 1000.0f;
    modle *= vmath::rotate(45.0f*dt, vmath::vec3(1.0f, 0.0f, 0.0f));
    modle *= vmath::rotate(45.0f*dt, vmath::vec3(0.0f, 1.0f, 0.0f));
    modle *= vmath::rotate(45.0f*dt, vmath::vec3(0.0f, 0.0f, 1.0f));


    glUseProgram(Program);
    glUniformMatrix4fv(ULocModel, 1, GL_FALSE, modle);
    glUniformMatrix4fv(ULocPorjView, 1, GL_FALSE, ProjView);


    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(verts);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vIo);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, BUFFER_OFFSET(sizeof(unsigned int)*8));


    glutSwapBuffers();
    glutPostRedisplay();
}




int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextVersion(4, 3);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Test chapter07");

    if (glewInit() != 0)
    {
        cout << "Unable to initilizate GLEW." << endl;
        return 0;
    }

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}





