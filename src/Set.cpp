#include <iostream>
#include <fstream>

#include "UsgsAstroFramePlugin.h"
#include "UsgsAstroFrameSensorModel.h"
#include "SensorUtils.h"

#include <sstream>
#include <string>
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
    if (it.value().size() >1 ) {
      vector<double> v = it.value();
      for (int j=0;j < v.size(); j++) {
        ostringstream val;
        val << setprecision(15) << v[j];
        imageSupportData.addParam(it.key(),val.str());
      }
    }
    else {
      imageSupportData.addParam(it.key(), it.value().dump());
    }
  }
  isdFile.close(); 

  // Instantiate a Camera Model 
  csm::WarningList *warnings;
  UsgsAstroFramePlugin framePlugin;
  csm::Model *cameraModel = framePlugin.constructModelFromISD(imageSupportData,
                                                             "USGS_ASTRO_FRAME_SENSOR_MODEL",
                                                              warnings);
  // Grab the Camera Model model state to get the sun location
  std::string modelState = cameraModel->getModelState(); 
  json parsedIsd = json::parse(modelState);

  // TODO: actually use the values from the CSM camera model for the phase angle calculation

  // Calculate the phase angle
  std::vector<double> lookDir = {0.0, 0.0, 0.0};
  std::vector<double> sunPosition = {0.0, 0.0, 0.0};
  
  double phaseAngle = PhaseAngle(lookDir, sunPosition);
  std::cout << "Phase Angle: " << phaseAngle << std::endl; 

  return 0;
}
