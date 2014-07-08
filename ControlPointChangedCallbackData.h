/*
 * ControlPointChangedCallbackData.h - Class for control point changed callback data.
 *
 * Author: Patrick O'Leary
 * Created: November 9, 2007
 * Copyright 2007. All rights reserved.
 */
#ifndef CONTROLPOINTCHANGEDCALLBACKDATA_H_
#define CONTROLPOINTCHANGEDCALLBACKDATA_H_

#include "ColorMapCallbackData.h"

// begin Forward Declarations
class ColorMap;
class ControlPoint;
// end Forward Declarations

class ControlPointChangedCallbackData : public ColorMapCallbackData {
public:
	ControlPointChangedCallbackData();
	ControlPointChangedCallbackData(ColorMap* _colorMap, ControlPoint* _previousControlPoint, ControlPoint* _currentControlPoint);
	~ControlPointChangedCallbackData();
	ControlPoint* getCurrentControlPoint(void);
private:
	ControlPoint* previousControlPoint;
	ControlPoint* currentControlPoint;
};

#endif /*CONTROLPOINTCHANGEDCALLBACKDATA_H_*/
