#include <iostream>
#include "SIMPLE_PARSER.h"
#include "SUBSPACE_FLUID_3D_EIGEN.h"
#include <string>

using std::string;

SUBSPACE_FLUID_3D_EIGEN* fluid = NULL;
BOX* box = NULL;
// box parameters
const VEC3F boxCenter(0.5, 0.5, 0.5);
const VEC3F boxLengths(0.4, 0.05, 0.15);

// period over which the box revolves
const double period = 4.0;

// period over which the box translates
const double translationPeriod = 10.0;


int main(int argc, char* argv[]) {
  // read in the cfg file
  if (argc != 2)
  {
    cout << " Usage: " << argv[0] << " *.cfg" << endl;
    return 0;
  }
  SIMPLE_PARSER parser(argv[1]);

  int xRes = parser.getInt("xRes", 48);
  int yRes = parser.getInt("yRes", 64);
  int zRes = parser.getInt("zRes", 48);
  string reducedPath = parser.getString("reduced path", "./data/reduced.dummy/");
  string snapshotPath = parser.getString("snapshot path", "./data/dummy/");
  int simulationSnapshots = parser.getInt("simulation snapshots", 20);
  Real vorticity = parser.getFloat("vorticity", 0);

  cout << " Using vorticity: " << vorticity << endl;

  unsigned int boundaries[6];
  boundaries[0] = parser.getInt("front", 1);
  boundaries[1] = parser.getInt("back", 1);
  boundaries[2] = parser.getInt("left", 1);
  boundaries[3] = parser.getInt("right", 1);
  boundaries[4] = parser.getInt("top", 0);
  boundaries[5] = parser.getInt("bottom", 0);

  string names[] = {"front", "back", "left", "right", "top", "bottom"};
  for (int x = 0; x < 6; x++)
  {
    cout << " Boundary on " << names[x].c_str() << "\tis set to " << flush;
    if (boundaries[x] == 0)
      cout << "Neumann " << endl;
    else
      cout << "Dirichlet " << endl;
  }

  double discardThreshold = parser.getFloat("discard threshold", 1e-9);
  cout << " Using discard threshold: " << discardThreshold << endl;
  
  bool usingIOP = parser.getBool("iop", 0);
  cout << " Using iop: " << usingIOP << endl;

	fluid = new SUBSPACE_FLUID_3D_EIGEN(xRes, yRes, zRes, reducedPath, &boundaries[0], usingIOP);
  fluid->setTotalReducedSteps(simulationSnapshots);

  box = new BOX(boxCenter, boxLengths, period, translationPeriod);
  box->set_dt(fluid->dt());
  // linke the fluid with the obstacle
  fluid->setObstacle(*box);

  int start = 150;
  // fluid->buildRemainingObstacleMatrices(start);

  delete fluid;
  delete box;

  return 0;
}
