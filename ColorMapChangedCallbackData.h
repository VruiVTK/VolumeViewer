/*
 * ColorMapChangedCallbackData.h - Class for color map changed callback data.
 *
 * Author: Patrick O'Leary
 * Created: November 9, 2007
 * Copyright 2007. All rights reserved.
 */
#ifndef COLORMAPCHANGEDCALLBACKDATA_H_
#define COLORMAPCHANGEDCALLBACKDATA_H_

#include "ColorMapCallbackData.h"

// begin Forward Declarations
class ColorMap;
// end Forward Declarations

class ColorMapChangedCallbackData : public ColorMapCallbackData {
public:
	ColorMapChangedCallbackData();
	ColorMapChangedCallbackData(ColorMap* colorMap);
	~ColorMapChangedCallbackData();
};

#endif /*COLORMAPCHANGEDCALLBACKDATA_H_*/
