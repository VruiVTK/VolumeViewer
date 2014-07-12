/*
 * Contours.h - Class for adding x, y and z and editing slice colormap.
 *
 * Author: Patrick O'Leary
 * Created: August 11, 2008
 * Copyright: 2008. All rights reserved.
 */
#ifndef CONTOURS_INCLUDED
#define CONTOURS_INCLUDED

/* Vrui includes */
#include <GL/GLColorMap.h>
#include <GLMotif/Blind.h>
#include <GLMotif/Popup.h>
#include <GLMotif/PopupWindow.h>
#include <GLMotif/RadioBox.h>
#include <GLMotif/RowColumn.h>
#include <GLMotif/Slider.h>
#include <GLMotif/StyleSheet.h>
#include <GLMotif/TextField.h>
#include <Misc/CallbackData.h>
#include <Misc/CallbackList.h>

#include "ExampleVTKReader.h"

// begin Forward Declarations
//class ColorMap;
//class Storage;
class ScalarWidget;
//class SwatchesWidget;
// end Forward Declarations

class Contours: public GLMotif::PopupWindow {
public:
    ExampleVTKReader * exampleVTKReader;

    Contours(ExampleVTKReader * _ExampleVTKReader);
    virtual ~Contours(void);
//    void changeContoursColorMap(int colormap) const;
//    void changeContoursColorMapCallback(GLMotif::RadioBox::ValueChangedCallbackData * callBackData);
//    void exportContoursColorMap(double* colormap) const;
//    Storage * getColorMap(void) const;
//    void setColorMap(Storage* storage);
//    const ColorMap* getContoursColorMap(void) const;
//    ColorMap* getContoursColorMap(void);
//    void setContoursColorMap(int colorMapCreationType, double _minimum, double _maximum);
//    Misc::CallbackList& getContoursColorMapChangedCallbacks(void);
//    void sliceColorMapChangedCallback(Misc::CallbackData * callBackData);
    void sliderCallback(GLMotif::Slider::ValueChangedCallbackData * callBackData);
    void toggleSelectCallback(GLMotif::ToggleButton::ValueChangedCallbackData * callBackData);
    Misc::CallbackList& getAlphaChangedCallbacks(void);
    void getControlPointValues(double* colormap) const;
    int getNumberOfControlPoints(void) const;
private:
    ScalarWidget* alphaComponent;
//    ColorMap * colorMap;
    GLMotif::Blind * colorPane;
    GLMotif::Slider * colorSliders[3];
//    double* sliceColormap;
//    SwatchesWidget * swatchesWidget;
    GLMotif::TextField* xSliceValue;
    GLMotif::TextField* ySliceValue;
    GLMotif::TextField* zSliceValue;
//    void colorMapChangedCallback(Misc::CallbackData * callbackData);
//    void colorSliderCallback(Misc::CallbackData * callbackData);
//    void colorSwatchesWidgetCallback(Misc::CallbackData * callbackData);
    void controlPointChangedCallback(Misc::CallbackData * callbackData);
    GLMotif::RowColumn * createButtonBox(GLMotif::RowColumn * & colorMapDialog);
//    void removeAlphaControlPointCallback(Misc::CallbackData* _callbackData);
    void alphaControlPointChangedCallback(Misc::CallbackData* _callbackData);
//    GLMotif::RowColumn * createColorEditor(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & colorMapDialog);
//    void createColorMap(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & colorMapDialog);
    void createContoursDialog(const GLMotif::StyleSheet & styleSheet);
//    void createColorPanel(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & colorMapDialog);
//    GLMotif::Slider * createColorSlider(const char * title, GLMotif::Color color, const GLMotif::StyleSheet & styleSheet,
//            GLMotif::RowColumn * colorSlidersBox);
//    GLMotif::RowColumn * createColorSliderBox(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * colorEditor);
//    void createColorSliders(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * colorEditor);
//    void createColorSwatchesWidget(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & colorEditor);
//    GLMotif::Popup * createSliceColorMapSubMenu(void);
    void createAlphaComponent(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog);
    void createXContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet);
    void createXYZContours(const GLMotif::StyleSheet & styleSheet, GLMotif::RowColumn * & colorMapDialog);
    void createYContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet);
    void createZContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet);
    void initialize(void);
    void removeControlPointCallback(Misc::CallbackData * callbackData);
};

#endif
