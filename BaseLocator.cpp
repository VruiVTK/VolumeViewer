// VolumeViewer includes
#include "VolumeViewer.h"
#include "BaseLocator.h"

#include <Vrui/LocatorTool.h>

/*
 * BaseLocator
 *
 * parameter _locatorTool - Vrui::LocatorTool*
 * parameter _volumeViewer - VolumeViewer*
 */
BaseLocator::BaseLocator(Vrui::LocatorTool* _locatorTool, VolumeViewer* _volumeViewer) :
	Vrui::LocatorToolAdapter(_locatorTool) {
	volumeViewer = _volumeViewer;
} // end BaseLocator()

/*
 * ~BaseLocator - Destructor
 */
BaseLocator::~BaseLocator(void) {
	volumeViewer = 0;
} // end ~BaseLocator()

/*
 * highlightLocator - Render actual locator
 *
 * parameter glContextData - GLContextData&
 */
void BaseLocator::highlightLocator(GLContextData& glContextData) const {
} // end highlightLocator()

/*
 * glRenderAction - Render opaque elements of locator
 *
 * parameter glContextData - GLContextData&
 */
void BaseLocator::glRenderAction(GLContextData& glContextData) const {
} // end glRenderAction()

/*
 * glRenderActionTransparent - Render transparent elements of locator
 *
 * parameter glContextData - GLContextData&
 */
void BaseLocator::glRenderActionTransparent(GLContextData& glContextData) const {
} // end glRenderActionTransparent()

void BaseLocator::getName(std::string& name) const {
}
