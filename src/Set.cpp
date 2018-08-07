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
  // TODO: Do we want to do a dynamic cast here or is there a better solution? It is needed to call methods below...
  UsgsAstroFrameSensorModel *frameModel = dynamic_cast<UsgsAstroFrameSensorModel *>(cameraModel);

  // Grab the Camera Model model state to get the sun location
  std::string modelState = cameraModel->getModelState(); 
  json parsedIsd = json::parse(modelState);

  // Calculate the phase angle
  // TODO: actually use the values from the CSM camera model for the phase angle calculation
  // TODO: let user pass in image point/ground point 
  csm::ImageCoord imagePt(0.0, 0.0);
  csm::EcefVector lookDirUnit = frameModel->imageToRemoteImagingLocus(imagePt).direction;
  std::vector<double> lookDir = {lookDirUnit.x, lookDirUnit.y, lookDirUnit.z};

  // sun pos (center body to center sun)
  // is body fixed ground coordinated (center body to ground point)
  // minus the illumination direction (center sun to ground point)
  csm::EcefCoord groundPoint = frameModel->imageToGround(imagePt, 0.0); // TODO: Change height from 0.0?
  csm::EcefVector illumDir = frameModel->getIlluminationDirection(groundPoint);
  std::vector<double> sunPosition = {groundPoint.x - illumDir.x, 
	                             groundPoint.y - illumDir.y, 
				     groundPoint.z - illumDir.z};
  
  double phaseAngle = PhaseAngle(lookDir, sunPosition);

  // TODO: all of these may not be needed. keep for debugging purposes right now
  std::cout << "Image Coord: " << imagePt.samp << ", " << imagePt.line << std::endl; 
  std::cout << "Look Dir: " << lookDir[0] << ", " << lookDir[1] << ", " << lookDir[2] << std::endl; 
  std::cout << "Groud Point: " << groundPoint.x << ", " << groundPoint.y << ", " << groundPoint.z << std::endl; 
  std::cout << "Illum Dir: " << illumDir.x << ", " << illumDir.y << ", " << illumDir.z << std::endl; 
  std::cout << "Sun Pos: " << sunPosition[0] << ", " << sunPosition[1] << ", " << sunPosition[2] << std::endl; 
  std::cout << "Phase Angle: " << phaseAngle << std::endl; 

  return 0;
}
