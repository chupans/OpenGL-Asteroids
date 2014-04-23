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
#include "CBoundaryBody.h"
#include "CPolyBody.h"
#include "CCompositeBody.h"
#include "Functions.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

GLuint VBO;
GLuint IBO;
GLuint gWVPLocation;
GLuint gRotLocation;
GLuint isRealLocation;

TPhysEngine *eng;

Camera* pGameCamera = NULL;

using namespace std;

static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec2 Position;                                             \n\
                                                                                    \n\
uniform mat2 gRot;                                                                  \n\
uniform mat4 gWVP;                                                                  \n\
uniform int isReal;                                                                 \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
  if (isReal == 0)                                                                  \n\
  {                                                                                 \n\
    gl_Position = gWVP * vec4( gRot * Position, 0.0, 1.0);                          \n\
    Color = vec4(clamp(Position*5, 0.0, 1.0), 0.5, 1.0);                            \n\
  }                                                                                 \n\
  else                                                                              \n\
  {                                                                                 \n\
    gl_Position = gWVP * vec4 ( Position, 0.0, 1.0);                                \n\
    Color = vec4(1.0, 1.0, 1.0, 1.0);                                               \n\
  }                                                                                 \n\
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
  FragColor = Color;                                                                \n\
}";

static void RenderSceneCB()
{
  long timeDelta, curTime;
  static long lastTime;
  curTime = GetTickCount();
  timeDelta = curTime - lastTime;
  lastTime = curTime;
  timeDelta = 10;

  pGameCamera->OnRender();

  glClear(GL_COLOR_BUFFER_BIT);

  Pipeline p;
  //p.Rotate(0.0f, Scale, 0.0f);
  //
  p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
  vector<IBody*>::iterator it;
  for (it = eng->m_existingBodies.begin(); it != eng->m_existingBodies.end(); it++)
  {
    p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
    p.WorldPos((*it)->GetPosition().getX(), (*it)->GetPosition().getY(), 3.0f);

    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
    glUniformMatrix2fv(gRotLocation, 1, GL_FALSE, (const GLfloat*)&((*it)->GetRotMatrix()));

    (*it)->Render();

    p.WorldPos(0.0f, 0.0f, 3.0f);
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
    (*it)->RenderDebug(isRealLocation);
  }

  glutSwapBuffers();
    eng->physStep((float)timeDelta/1000);
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

  gRotLocation = glGetUniformLocation(ShaderProgram, "gRot");
  assert(gRotLocation != 0xFFFFFFFF);

  isRealLocation = glGetUniformLocation(ShaderProgram, "isReal");
}
void initPhysEngine()
{
  eng = new TPhysEngine();
  eng->maxDeltaTime = 0.5f;

    srand(time(NULL));
  vector<Vector2f> points;

  PolyFunctions::GenerateConvexPolygon(4, points);

  CPolyBody *a = new CPolyBody(&points[0], 4, Vector2f(0.0f, -0.7f), Vector2f(1.5f, -0.5f), false, 0.0f);

  points.clear();
  PolyFunctions::GenerateConvexPolygon(4, points);
  CPolyBody *b = new CPolyBody(&points[0], 4, Vector2f(1.0f, 1.0f), Vector2f(1.3f, 0.7f), false, 0.0f);

  CPolyBody *c;
  for (int i = 0; i < 15; i++)
  {
    points.clear();
    PolyFunctions::GenerateConvexPolygon(4, points);
    c = new CPolyBody(&points[0], 4, Vector2f((float)(rand()%100 - 50)/50.0f, (float)(rand()%100 - 50)/50.0f), Vector2f((float)(rand()%100 - 50)/50.0f, (float)(rand()%100 - 50)/50.0f), false, 0.0f);
    eng->m_existingBodies.push_back((IBody*)c);
  }

  CBoundaryBody *bottom = new CBoundaryBody(Vector2f(-1.8f, -1.55f), Vector2f(+1.8f, -1.55f), Vector2f(0.0f, -2.0f));
  CBoundaryBody *top = new CBoundaryBody(Vector2f(1.8f, 1.55f), Vector2f(-1.8f, 1.55f), Vector2f(0.0f, 2.0f));
  CBoundaryBody *left = new CBoundaryBody(Vector2f(-1.55f, 1.8f), Vector2f(-1.55f, -1.8f), Vector2f(-2.0f, 0.0f));
  CBoundaryBody *right = new CBoundaryBody(Vector2f(1.55f, -1.8f), Vector2f(1.55f, +1.8f), Vector2f(+2.0f, 0.0f));

  eng->m_existingBodies.push_back((IBody*)a);
  eng->m_existingBodies.push_back((IBody*)b);
  eng->m_existingBodies.push_back((IBody*)bottom);
  eng->m_existingBodies.push_back((IBody*)top);
  eng->m_existingBodies.push_back((IBody*)left);
  eng->m_existingBodies.push_back((IBody*)right);

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
