#include <iostream>
#include <fstream>

#include "UsgsAstroFramePlugin.h"
#include "UsgsAstroFrameSensorModel.h"
#include "SensorUtils.h"

#include <json/json.hpp>
using json = nlohmann::json;

using namespace std;

int main() {
  // Load ISD file
  csm::Isd imageSupportData("simpleFramerISD.json");
  imageSupportData.clearAllParams();

  std::ifstream isdFile("simpleFramerISD.json");
  json jsonIsd = json::parse(isdFile);

  for (json::iterator it = jsonIsd.begin(); it != jsonIsd.end(); ++it) {
    imageSupportData.addParam(it.key(), it.value().dump());
  }
  isdFile.close(); 
  
  // Check that csm::Isd was populated correctly: 
  std::cout << "Focal length value: " << imageSupportData.param("focal_length") << std::endl;

  // Calculate the phase angle
  std::vector<double> lookDir = {0.0, 0.0, 0.0};
  std::vector<double> sunPosition = {0.0, 0.0, 0.0};
  
  double phaseAngle = PhaseAngle(lookDir, sunPosition); 
  std::cout << "Phase Angle: " << phaseAngle << std::endl; 

  return 0;
}
