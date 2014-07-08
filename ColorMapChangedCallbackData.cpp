/*
 * CallbackData.cpp - Methods for ColorMapChangedCallbackData class.
 *
 * Author: Patrick O'Leary
 * Created: November 9, 2007
 * Copyright 2007. All rights reserved.
 */
#include "ColorMap.h"
#include "ColorMapChangedCallbackData.h"

/*
 * ColorMapChangedCallbackData - Constructor for ColorMapChangedCallbackData.
 */
ColorMapChangedCallbackData::ColorMapChangedCallbackData() {
} // end ColorMapChangedCallbackData()

/*
 * ColorMapChangedCallbackData - Constructor for ColorMapChangedCallbackData.
 *
 * parameter _colorMap - ColorMap*
 */
ColorMapChangedCallbackData::ColorMapChangedCallbackData(ColorMap* colorMap) :
	ColorMapCallbackData(colorMap) {
} // end ColorMapChangedCallbackData()

/*
 * ~ColorMapChangedCallbackData - Destructor for ColorMapChangedCallbackData.
 */
ColorMapChangedCallbackData::~ColorMapChangedCallbackData() {
} // end ~ColorMapChangedCallbackData()
