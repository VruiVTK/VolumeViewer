#ifndef CLIPPINGPLANELOCATOR_H_
#define CLIPPINGPLANELOCATOR_H_

#include <Vrui/LocatorTool.h>

// Begin forward declarations
class BaseLocator;
class ClippingPlane;
class VolumeViewer;
// End forward declarations
class ClippingPlaneLocator : public BaseLocator {
public:
	ClippingPlaneLocator(Vrui::LocatorTool* locatorTool,
			VolumeViewer * volumeViewer);
	~ClippingPlaneLocator(void);
	virtual void buttonPressCallback(
			Vrui::LocatorTool::ButtonPressCallbackData* callbackData);
	virtual void buttonReleaseCallback(
			Vrui::LocatorTool::ButtonReleaseCallbackData* callbackData);
	virtual void motionCallback(
			Vrui::LocatorTool::MotionCallbackData* callbackData);
private:
	ClippingPlane * clippingPlane;
};

#endif /*CLIPPINGPLANELOCATOR_H_*/
