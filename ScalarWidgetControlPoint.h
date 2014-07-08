/*
 * ScalarWidgetControlPoint.h - Class for editable scalar widget control points for one-dimensional
 * component transfer functions.
 *
 * Author: Patrick O'Leary
 * Created: January 19, 2008
 * Copyright 2008. All rights reserved.
 */
#ifndef SCALARWIDGETCONTROLPOINT_H_
#define SCALARWIDGETCONTROLPOINT_H_

#include <GL/gl.h>

class ScalarWidgetControlPoint {
public:
	ScalarWidgetControlPoint* left;
	ScalarWidgetControlPoint* right;
	ScalarWidgetControlPoint(void);
	ScalarWidgetControlPoint(double _value, float _scalar);
	~ScalarWidgetControlPoint(void);
	float getScalar(void);
	void setScalar(float _scalar);
	double getValue(void);
	void setValue(double _value);
	GLfloat getX(void);
	void setX(GLfloat _x);
	GLfloat getY(void);
	void setY(GLfloat _y);
private:
	double value;
	float scalar;
	GLfloat x, y;
};

#endif /*SCALARWIDGETCONTROLPOINT_H_*/
