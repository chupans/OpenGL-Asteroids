#pragma once
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "pipeline.h"
#include "camera.h"
#include "TPhysEngine.h"
#include "CBoundaryBody.h"
#include "CPolyBody.h"
#include "CCompositeBody.h"
#include "Functions.h"
#include "CPlayerObject.h"

#include <vector>

class Game
{
public:
  Game(void);
  ~Game(void);
  void Init(int width, int height);
  void Step();
  void Action(int type);

private:

  void CompileShaders();
  void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

  GLuint gWVPLocation;
  GLuint gRotLocation;
  GLuint isRealLocation;

  int m_width, m_height;

  TPhysEngine *eng;
  CPlayerObject *player;
  Camera* pGameCamera;
};
