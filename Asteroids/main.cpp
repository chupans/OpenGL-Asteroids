#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>

#include "pipeline.h"
#include "camera.h"
#include "TPhysEngine.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;

TPhysEngine *eng;

Camera* pGameCamera = NULL;

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec2 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 0.0, 1.0);                                  \n\
    Color = vec4(clamp(Position*5, 0.0, 1.0), 0.5, 1.0);                            \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}";

static void RenderSceneCB()
{
  int start, end;
    long timeDelta, curTime;
    static long lastTime;
    curTime = GetTickCount();
    timeDelta = curTime - lastTime;
    lastTime = curTime;
    timeDelta = 10;

    eng->physStep((float)timeDelta/1000);
    pGameCamera->OnRender();

    glClear(GL_COLOR_BUFFER_BIT);

    Pipeline p;
    //p.Rotate(0.0f, Scale, 0.0f);
    vector<TPhysObject>::iterator it;
    for (it = eng->m_existingObjects.begin(), start = 0, end = 6; it < eng->m_existingObjects.end(); it++, start += 6, end += 6)
    {
      p.WorldPos(it->center.getX(), it->center.getY(), 3.0f);
      p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
      p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

      glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());

      it->m_collider.Render();
    }

    glutSwapBuffers();
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
    pGameCamera->OnKeyboard(Key);
}


static void KeyboardCB(unsigned char Key, int x, int y)
{
    switch (Key) {
        case 'q':
            exit(0);
    }
}

static void PassiveMouseCB(int x, int y)
{
    //pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(RenderSceneCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
    AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    assert(gWVPLocation != 0xFFFFFFFF);
}
void initPhysEngine()
{
  eng = new TPhysEngine();
  eng->maxDeltaTime = 0.5f;
  TPhysObject a(Vector2f(0.0f, -0.7f), Vector2f(0.2f, 0.0f));
  a._mass = 1.0f;

  Vector2f Vertices[4];
  Vertices[0] = Vector2f(-0.1f, -0.1f);
  Vertices[1] = Vector2f(0.1f, -0.1f);
  Vertices[2] = Vector2f(0.1f, 0.1f);
  Vertices[3] = Vector2f(-0.1f, 0.1f);
  TMeshCollider mesh(Vertices, 4, Vector2f(0.0f, 0.0f));
  a.m_collider = mesh;
  a.m_collider.UpdatePosition(a.center);

  TPhysObject b(Vector2f(0.0f, 0.0f), Vector2f(0.3f, -0.5f));
  b._mass = 1.0f;
  //Vertices[1] = Vector2f(0.2f, -0.3f);
  TMeshCollider mesh2(Vertices, 4, Vector2f(0.0f, 0.0f));
  b.m_collider = mesh2;
  b.m_collider.UpdatePosition(b.center);

  eng->m_existingObjects.push_back(a);
  eng->m_existingObjects.push_back(b);
  
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 15");
    glutGameModeString("1280x1024@32");
    glutEnterGameMode();

    InitializeGlutCallbacks();

    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    initPhysEngine();

    CompileShaders();

    glutMainLoop();

    return 0;
}
