#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <time.h>

#include "Game.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

GLuint VBO;
GLuint IBO;

Game *pGame = NULL;

using namespace std;


static void RenderSceneCB()
{
  pGame->Step();
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
  pGame->Action(Key);
}


static void KeyboardCB(unsigned char Key, int x, int y)
{
  pGame->Action(Key);
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

int main(int argc, char** argv)
{
  srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Tutorial 15");
  glutGameModeString("1280x1024@32");
  glutEnterGameMode();

  InitializeGlutCallbacks();


  // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
    fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
    return 1;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  pGame = new Game();
  pGame->Init(WINDOW_WIDTH, WINDOW_HEIGHT);


  glutMainLoop();

  return 0;
}
