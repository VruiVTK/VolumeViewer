#ifndef __FREESLICELOCATOR_H_
#define __FREESLICELOCATOR_H_

#include "BaseLocator.h"
#include <vtkSmartPointer.h>

#include <Vrui/LocatorTool.h>
#include "VolumeViewer.h"

// Begin forward declarations
class vtkLight;
// End forward declarations

class FreeSliceLocator : public BaseLocator
{
public:
  FreeSliceLocator(Vrui::LocatorTool* locatorTool,
    VolumeViewer * _volumeViewer);
  ~FreeSliceLocator(void);

  virtual void buttonPressCallback(
    Vrui::LocatorTool::ButtonPressCallbackData* callbackData);
  virtual void buttonReleaseCallback(
    Vrui::LocatorTool::ButtonReleaseCallbackData* callbackData);
  virtual void motionCallback(
    Vrui::LocatorTool::MotionCallbackData* callbackData);
private:
  int * FreeSliceVisibility;
  double * FreeSliceOrigin;
  double * FreeSliceNormal;
};
#endif //__FREESLICELOCATOR_H_
