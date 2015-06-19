/* VolumeViewer includes */
#include "VolumeViewer.h"

#include "BaseLocator.h"
#include "FreeSliceLocator.h"

#include <iostream>

/* Vrui includes */
#include <Vrui/LocatorTool.h>
#include <Geometry/Point.h>
#include <Geometry/Vector.h>
#include <Geometry/OrthogonalTransformation.h>

/* VTK includes */
#include <vtkLight.h>

/*
 * FreeSliceLocator - Constructor for FreeSliceLocator class.
 *
 * parameter locatorTool - Vrui::LocatorTool *
 * parameter volumeViewer - VolumeViewer *
 */
FreeSliceLocator::FreeSliceLocator(Vrui::LocatorTool * locatorTool,
  VolumeViewer* _volumeViewer) :
  BaseLocator(locatorTool, _volumeViewer),
  FreeSliceVisibility(0),
  FreeSliceOrigin(0),
  FreeSliceNormal(0)
{
  this->FreeSliceVisibility = _volumeViewer->getFreeSliceVisibility();
  this->FreeSliceOrigin = _volumeViewer->getFreeSliceOrigin();
  this->FreeSliceNormal = _volumeViewer->getFreeSliceNormal();
} // end FreeSliceLocator()

/*
 * ~FreeSliceLocator - Destructor for FreeSliceLocator class.
 */
FreeSliceLocator::~FreeSliceLocator(void) {
} // end ~FreeSliceLocator()

/*
 * motionCallback
 *
 * parameter callbackData - Vrui::LocatorTool::MotionCallbackData *
 */
void FreeSliceLocator::motionCallback(
		Vrui::LocatorTool::MotionCallbackData* callbackData) {
          Vrui::Point position = callbackData->currentTransformation.getOrigin();
          Vrui::Vector planeNormal =
            callbackData->currentTransformation.transform(Vrui::Vector(0,1,0));
          this->FreeSliceOrigin[0] = position[0];
          this->FreeSliceOrigin[1] = position[1];
          this->FreeSliceOrigin[2] = position[2];
          this->FreeSliceNormal[0] = planeNormal[0];
          this->FreeSliceNormal[1] = planeNormal[1];
          this->FreeSliceNormal[2] = planeNormal[2];
} // end motionCallback()

/*
 * buttonPressCallback
 *
 * parameter callbackData - Vrui::LocatorTool::ButtonPressCallbackData *
 */
void FreeSliceLocator::buttonPressCallback(
		Vrui::LocatorTool::ButtonPressCallbackData* callbackData)
{
  this->FreeSliceVisibility[0] = 1;
} // end buttonPressCallback()

/*
 * buttonReleaseCallback
 *
 * parameter callbackData - Vrui::LocatorTool::ButtonReleaseCallbackData *
 */
void FreeSliceLocator::buttonReleaseCallback(
		Vrui::LocatorTool::ButtonReleaseCallbackData* callbackData)
{
  this->FreeSliceVisibility[0] = 0;
} // end buttonReleaseCallback()
