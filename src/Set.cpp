#include <iostream>

#include "SensorUtils.h"

using namespace std;

int main() {
  // Calculate the phase angle
  std::vector<double> lookDir = {0.0, 0.0, 0.0};
  std::vector<double> sunPosition = {0.0, 0.0, 0.0};
  
  double phaseAngle = PhaseAngle(lookDir, sunPosition); 
  std::cout << "Phase Angle: " << phaseAngle << std::endl; 

  return 0;
}
