#include <iostream>


#include "vgl.h"
#include "LoadShaders.h"
#include "vmath.h"

using namespace std;


struct Vertex
{
    GLubyte color[4];
    GLfloat position[3];
};

Vertex Verts[9] = { { { 255,255,255,255 },{ 0.0f, 0.9f, 0.0f } },
{ { 255,255,255,255 },{ -0.9f, -0.9f, 0.0f } },
{ { 255,255,255,255 },{ 0.9f, -0.9f, 0.0f } } ,
{ { 255,0,0,255 },{ 0.4f, 0.4f, -0.2f } } ,
{ { 255,0,0,255 },{ 0.0f, -0.4f, -0.2f } } ,
{ { 255,0,0,255 },{ 0.8f, -0.4f, -0.2f } } };

GLuint Program = 0;

GLuint vert;
GLuint vbo;

void Init()
{
    ShaderInfo si[] = { { GL_VERTEX_SHADER, "Chpater04.vert" },{ GL_FRAGMENT_SHADER, "Chapter04.frag" },{ GL_NONE, NULL } };
    Program = LoadShaders(si);

    glGenVertexArrays(1, &vert);
    glBindVertexArray(vert);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(Verts->color)));
	glEnableVertexAttribArray(1);

    glClearColor(0.2, 0.1, 0.3, 1.0);
    glUseProgram(Program);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vert);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_INVERT);//反转颜色，将黑色三角形反转为白色
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glLogicOp(GL_COPY);
    glDisable(GL_COLOR_LOGIC_OP);

    glutSwapBuffers();
    glutPostRedisplay();
	Sleep(30);
}


int main(int argc, char*argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(400, 400);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Test OpenGL Chapter 04");

    if (glewInit())
    {
        cout << "Unable to Initialize GLEW." << endl;
        return 1;
    }
    Init();
    glutDisplayFunc(Display);
    glutMainLoop();

    return 0;
}



