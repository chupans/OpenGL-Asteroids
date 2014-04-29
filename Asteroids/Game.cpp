#include "Game.h"

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

Game::Game(void)
{
}

Game::~Game(void)
{
}

void Game::Init(int width, int height)
{

  m_width = width;
  m_height = height;
  CompileShaders();
  pGameCamera = new Camera(width, height);

  eng = new TPhysEngine();
  eng->maxDeltaTime = 0.5f;

  vector<Vector2f> points;

  PolyFunctions::GenerateConvexPolygon(4, points);

  CPolyBody *a = new CPolyBody(&points[0], 4, Vector2f(0.0f, -0.7f), Vector2f(1.5f, -0.5f), false, 0.0f);
  player = new CPlayerObject(a);

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

void Game::Step()
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
  p.SetPerspectiveProj(60.0f, m_width, m_height, 1.0f, 100.0f);
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
    if ((*it) != player->GetBody())
      (*it)->RenderDebug(isRealLocation);
  }

  glutSwapBuffers();
  eng->physStep((float)timeDelta/1000);
}

void Game::Action( int type )
{
  switch (type) 
  {
  case 'q':
    exit(0);
    break;
  case 'w':
    player->Move(UP);
    break;;
  case 'a':
    player->Move(LEFT);
    break;
  case 'd':
    player->Move(RIGHT);
    break;
  case 's':
    player->Move(DOWN);
    break;

  }
}

void Game::CompileShaders()
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

void Game::AddShader( GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType )
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
