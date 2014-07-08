/*
 * ColorMapCallbackData.h - Class for callback data.
 *
 * Author: Patrick O'Leary
 * Created: November 9, 2007
 * Copyright 2007. All rights reserved.
 */
#ifndef COLORMAPCALLBACKDATA_H_
#define COLORMAPCALLBACKDATA_H_

#include <Misc/CallbackData.h>

// begin Forward Declarations
class ColorMap;
// end Forward Declarations

class ColorMapCallbackData : public Misc::CallbackData {
public:
	ColorMap* colorMap;
	ColorMapCallbackData();
	ColorMapCallbackData(ColorMap* _colorMap);
	~ColorMapCallbackData();
};

#endif /*COLORMAPCALLBACKDATA_H_*/
