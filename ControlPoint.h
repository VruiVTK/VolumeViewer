/*
 * ControlPoint.h - Class for editable control points for colormaps and transfer functions.
 * 
 * Author: Patrick O'Leary
 * Created: November 9, 2007
 * Copyright 2007. All rights reserved.
 */
#ifndef CONTROLPOINT_H_
#define CONTROLPOINT_H_

#include <GL/gl.h>

// begin Forward Declarations
class RGBAColor;
// end Forward Declarations

class ControlPoint {
public:
	double value;
	RGBAColor* rgbaColor;
	GLfloat x, y;
	ControlPoint* left;
	ControlPoint* right;
	ControlPoint();
	ControlPoint(double _value, RGBAColor* _rgbaColor);
	~ControlPoint();
};

#endif /*CONTROLPOINT_H_*/
