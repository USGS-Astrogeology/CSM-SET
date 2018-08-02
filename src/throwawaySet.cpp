// useage: throwawaySet (no arguments) 
//

#include "UsgsAstroFramePlugin.h"
#include "UsgsAstroFrameSensorModel.h"
#include "SensorUtils.h"

#include <iostream>
#include <fstream>
#include <json>

int main (int argc, char **argv) {
  
  // Load ISD file;  
  csm::Isd imageSupportData("EN1007907102M.json");
    
  std::ifstream isdFile("EN1007907102M.json");
  json jsonIsd = json::parse(isdFile);
  imageSupportData.clearAllParams();
  for (json::iterator it = jsonIsd.begin(); it != jsonIsd.end(); ++it) {
    isd.addParam(it.key(), it.value().dump());
  }
  isdFile.close; 

  // Instantiate sensor model from CSM with the ISD
  csm::WarningList *warnings;
  csm::Model *cameraModel = constructModelFromISD(imageSupportData,
                                                "USGS_ASTRO_FRAME_SENSOR_MODEL",
                                                 csm::WarningList *warnings)

  // (3) Loop over pixels (TODO: what pixels? where do we get them from with no input)  

  // a. Use sensor model to get look direction and sun position

  // There isn't a function in our CSM API to get the sun position, so get the model state
  // and pull it out. 
  std::string modelState = cameraModel.getModelState(); 
  json isd = json::parse(modelState)

  std::vector<double> lookDir = {isd["m_boresight"][0], isd["m_boresight"][1], 
                                 isd["m_boresight"][2]};

  std::vector<double> sunPosition = {isd["m_sunPosition"][0], isd["m_sunPosition"][1], 
                                     isd["m_sunPosition"][2]};

  // b. Pass look direction and sun position to Sensor Utils to compute phase angle
  double phaseAngle = SensorUtils::PhaseAngle(lookDir, sunPosition); 

  // c. Print phase angle to screen
  std::cout << "Phase Angle: " << phaseAngle << std::endl; 

  return 0; 
}
