// viewModes: Interactive viewer for subspace eigenvector modes
//
// Loads the U basis matrix and singular values, then displays individual
// modes as velocity-magnitude density fields. Arrow keys cycle through modes.
//
// Usage: ./bin/viewModes cfg/stam.64.cfg

#include "EIGEN.h"
#include "VECTOR.h"
#include <cmath>

#include <glvu.h>
#include <VEC3.h>
#include <iostream>
#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#include <GL/glu.h>
#endif
#include "FIELD_3D.h"
#include "VECTOR3_FIELD_3D.h"
#include "SIMPLE_PARSER.h"

using namespace std;

GLVU glvu;

// The basis matrix and singular values
MatrixXd U;
vector<double> singularValues;
int numModes = 0;
int currentMode = 0;

// Grid dimensions (peeled)
int xPeeled = 0, yPeeled = 0, zPeeled = 0;

// The density field we render (velocity magnitude of current mode)
FIELD_3D displayField;

// Sonification parameters (from dissertation)
double fundamentalFreq = 64.0;   // Hz
double octaveScaling = 1.75;

// Brightness scaling (modes have different magnitudes)
double brightnessScale = 1.0;

///////////////////////////////////////////////////////////////////////
// Compute the mapped audible frequency for mode i using the dissertation formula:
//   f_i = f * (sigma_max / sigma_i)^(1/s)
///////////////////////////////////////////////////////////////////////
double mappedFrequency(int modeIndex)
{
  if (modeIndex < 0 || modeIndex >= (int)singularValues.size())
    return 0.0;
  double sigmaMax = singularValues[0];
  double sigmaI = singularValues[modeIndex];
  if (sigmaI < 1e-15) return 0.0;
  return fundamentalFreq * pow(sigmaMax / sigmaI, 1.0 / octaveScaling);
}

///////////////////////////////////////////////////////////////////////
// Unproject mode i: set q = e_i, compute v = U * q (just column i of U),
// then fill displayField with velocity magnitude at each cell.
///////////////////////////////////////////////////////////////////////
void setMode(int mode)
{
  if (mode < 0 || mode >= numModes) return;
  currentMode = mode;

  // Column i of U is the velocity field for mode i
  // U has 3 * xPeeled * yPeeled * zPeeled rows (interleaved x,y,z)
  const double* col = U.col(mode).data();

  // Compute velocity magnitude at each cell
  double maxMag = 0.0;
  int totalCells = xPeeled * yPeeled * zPeeled;
  for (int i = 0; i < totalCells; i++) {
    double vx = col[3 * i + 0];
    double vy = col[3 * i + 1];
    double vz = col[3 * i + 2];
    double mag = sqrt(vx * vx + vy * vy + vz * vz);
    displayField[i] = mag;
    if (mag > maxMag) maxMag = mag;
  }

  // Normalize to [0, 1] for rendering
  if (maxMag > 1e-15) {
    double scale = brightnessScale / maxMag;
    for (int i = 0; i < totalCells; i++)
      displayField[i] *= scale;
  }

  double freq = mappedFrequency(mode);
  double sigma = (mode < (int)singularValues.size()) ? singularValues[mode] : 0.0;

  cout << "Mode " << mode << "/" << numModes - 1
       << "  sigma=" << sigma
       << "  freq=" << freq << " Hz"
       << "  maxVelMag=" << maxMag
       << endl;
}

///////////////////////////////////////////////////////////////////////
// GL callbacks
///////////////////////////////////////////////////////////////////////
void glutDisplay()
{
  glvu.BeginFrame();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
      glTranslatef(0.5, 0.5, 0.5);
      displayField.draw();
      displayField.drawBoundingBox();
    glPopMatrix();

  glvu.EndFrame();
}

void glutIdle()
{
  glutPostRedisplay();
}

void glutKeyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'q':
      exit(0);
      break;
    case '+':
    case '=':
      brightnessScale *= 1.5;
      cout << "Brightness: " << brightnessScale << endl;
      setMode(currentMode);
      break;
    case '-':
    case '_':
      brightnessScale /= 1.5;
      cout << "Brightness: " << brightnessScale << endl;
      setMode(currentMode);
      break;
    default:
      break;
  }
  glvu.Keyboard(key, x, y);
}

void glutSpecial(int key, int x, int y)
{
  switch (key)
  {
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_UP:
      if (currentMode < numModes - 1)
        setMode(currentMode + 1);
      break;
    case GLUT_KEY_LEFT:
    case GLUT_KEY_DOWN:
      if (currentMode > 0)
        setMode(currentMode - 1);
      break;
    case GLUT_KEY_HOME:
      setMode(0);
      break;
    case GLUT_KEY_END:
      setMode(numModes - 1);
      break;
  }
}

void glutMouseClick(int button, int state, int x, int y)
{
  glvu.Mouse(button, state, x, y);
}

void glutMouseMotion(int x, int y)
{
  glvu.Motion(x, y);
}

///////////////////////////////////////////////////////////////////////
// Open the GLVU window
///////////////////////////////////////////////////////////////////////
int glvuWindow()
{
  char title[] = "Mode Viewer";
  glvu.Init(title,
            GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH,
            0, 0, 800, 800);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  GLfloat lightZeroPosition[] = {10.0, 4.0, 10.0, 1.0};
  GLfloat lightZeroColor[] = {1.0, 1.0, 1.0, 1.0};
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
  glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0, 0, 0, 0);

  glutDisplayFunc(&glutDisplay);
  glutIdleFunc(&glutIdle);
  glutKeyboardFunc(&glutKeyboard);
  glutSpecialFunc(&glutSpecial);
  glutMouseFunc(&glutMouseClick);
  glutMotionFunc(&glutMouseMotion);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glvuVec3f ModelMin(-10, -10, -10), ModelMax(10, 10, 10),
        Eye(1.5, 0.5, 2), LookAtCntr(0.5, 0.5, 0.5), Up(0, 1, 0);

  float Yfov = 45;
  float Aspect = 1;
  float Near = 0.001f;
  float Far = 10.0f;
  glvu.SetAllCams(ModelMin, ModelMax, Eye, LookAtCntr, Up, Yfov, Aspect, Near, Far);

  glvuVec3f center(0.5, 0.5, 0.5);
  glvu.SetWorldCenter(center);

  glutMainLoop();
  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " *.cfg" << endl;
    return 0;
  }
  SIMPLE_PARSER parser(argv[1]);

  int xRes = parser.getInt("xRes", 48);
  int yRes = parser.getInt("yRes", 64);
  int zRes = parser.getInt("zRes", 48);
  string reducedPath = parser.getString("reduced path", "./data/reduced.dummy/");

  // Peeled grid (remove 1-cell boundary on each side)
  xPeeled = xRes - 2;
  yPeeled = yRes - 2;
  zPeeled = zRes - 2;

  cout << "Grid: " << xRes << "x" << yRes << "x" << zRes
       << "  Peeled: " << xPeeled << "x" << yPeeled << "x" << zPeeled << endl;

  // Load the basis matrix
  string basisFile = reducedPath + string("U.preadvect.matrix");
  cout << "Loading basis: " << basisFile << endl;
  bool ok = EIGEN::read(basisFile, U);
  if (!ok) {
    cerr << "Failed to load " << basisFile << endl;
    return 1;
  }

  numModes = U.cols();
  int expectedRows = 3 * xPeeled * yPeeled * zPeeled;
  cout << "Basis: " << U.rows() << " x " << numModes << " (expected " << expectedRows << " rows)" << endl;
  if (U.rows() != expectedRows) {
    cerr << "Row count mismatch! Expected " << expectedRows << endl;
    return 1;
  }

  // Load singular values (stored as squared values by BIG_MATRIX::outOfCoreSVD)
  string svFile = "./data/scratch/velocity.preadvect.matrix.singularValues.vector";
  VECTOR svRaw(svFile.c_str());
  cout << "Loaded " << svRaw.size() << " singular values from " << svFile << endl;

  // Take sqrt to get actual singular values, keep only the modes we have
  singularValues.resize(min(svRaw.size(), numModes));
  for (int i = 0; i < (int)singularValues.size(); i++)
    singularValues[i] = sqrt(fabs(svRaw[i]));

  // Print frequency mapping summary
  cout << endl;
  cout << "Sonification mapping (f=" << fundamentalFreq << " Hz, s=" << octaveScaling << "):" << endl;
  for (int i = 0; i < min(numModes, 10); i++)
    cout << "  Mode " << i << ": sigma=" << singularValues[i]
         << "  -> " << mappedFrequency(i) << " Hz" << endl;
  if (numModes > 10)
    cout << "  ... (" << numModes << " modes total)" << endl;
  cout << endl;

  // Initialize the display field
  displayField = FIELD_3D(xPeeled, yPeeled, zPeeled);

  // Set initial mode
  setMode(0);

  cout << "Controls:" << endl;
  cout << "  Left/Right arrows: previous/next mode" << endl;
  cout << "  Home/End: first/last mode" << endl;
  cout << "  +/-: adjust brightness" << endl;
  cout << "  Mouse: rotate/zoom (GLVU trackball)" << endl;
  cout << "  q: quit" << endl;
  cout << endl;

  glutInit(&argc, argv);
  glvuWindow();

  return 0;
}
