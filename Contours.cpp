/*
 * Contours.cpp - Methods for Contours class.
 *
 * Author: Patrick O'Leary
 * Created: August 11, 2008
 * Copyright: 2008. All rights reserved.
 */
#include <iostream>
//#include <GL/GLColor.h>
#include <GLMotif/Button.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/Label.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/WidgetManager.h>
#include <Misc/File.h>

/* Vrui includes to use the Vrui interface */
#include <Vrui/Vrui.h>

//#include "ColorMap.h"
#include "ControlPointChangedCallbackData.h"
//#include "RGBAColor.h"
//#include "Storage.h"
#include "Contours.h"
#include "ScalarWidget.h"
#include "ScalarWidgetControlPointChangedCallbackData.h"
#include "ScalarWidgetStorage.h"
//#include "SwatchesWidget.h"

/*
 * Contours - Constructor for Contours class.
 * 		extends GLMotif::PopupWindow
 */
Contours::Contours(ExampleVTKReader * _ExampleVTKReader) :
    GLMotif::PopupWindow("ContoursPopup", Vrui::getWidgetManager(), "Contours"),
            exampleVTKReader(_ExampleVTKReader) {
    initialize();
}

/*
 * ~Contours - Destructor for Contours class.
 */
Contours::~Contours(void) {
}

///*
// * changeContoursColorMap - Change the colormap.
// *
// * parameter colormap - int
// */
//void Contours::changeContoursColorMap(int colormap) const {
//    colorMap->createColorMap(colormap);
//} // end changeContoursColorMap()
//
///*
// * changeContoursColorMapCallback
// *
// * parameter callBackData - GLMotif::RadioBox::ValueChangedCallbackData *
// */
//void Contours::changeContoursColorMapCallback(GLMotif::RadioBox::ValueChangedCallbackData * callBackData) {
//    /* Set the new Contours colormap */
//    int value = callBackData->radioBox->getToggleIndex(callBackData->newSelectedToggle);
//    changeContoursColorMap(value);
//    exportContoursColorMap(sliceColormap);
//    exampleVTKReader->updateSliceColorMap(sliceColormap);
//    Vrui::requestUpdate();
//} // end changeContoursColorMapCallback()
//
//
///*
// * colorSliderCallback - Callback of change to color slider value.
// *
// * parameter _callbackData - Misc::CallbackData*
// */
//void Contours::colorSliderCallback(Misc::CallbackData* _callbackData) {
//    RGBAColor* rgbaColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
//    for (int i = 0; i < 3; ++i) {
//        rgbaColor->setValues(i, float(colorSliders[i]->getValue()));
//    }
//    rgbaColor->setValues(3, 1.0f);
//    colorPane->setBackgroundColor(rgbaColor->getValues());
//    colorMap->setControlPointColor(*rgbaColor);
//}
//
///*
// * colorSwatchesWidgetCallback
// *
// * parameter _callbackData - Misc::CallbackData*
// */
//void Contours::colorSwatchesWidgetCallback(Misc::CallbackData* _callbackData) {
//    float* _color = swatchesWidget->getCurrentColor();
//    RGBAColor* rgbaColor = new RGBAColor(_color[0], _color[1], _color[2], 1.0f);
//    for (int i = 0; i < 3; ++i) {
//        colorSliders[i]->setValue(_color[i]);
//    }
//    colorPane->setBackgroundColor(rgbaColor->getValues());
//    colorMap->setControlPointColor(*rgbaColor);
//} // end colorSwatchesWidgetCallback()

/*
 * controlPointChangedCallback - Callback to select a control point.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Contours::controlPointChangedCallback(Misc::CallbackData* _callbackData) {
//    ControlPointChangedCallbackData* callbackData = static_cast<ControlPointChangedCallbackData*> (_callbackData);
//    if (callbackData->getCurrentControlPoint() != 0) {
//        RGBAColor* rgbaColor = colorMap->getControlPointColor();
//        colorPane->setBackgroundColor(rgbaColor->getValues());
//        for (int i = 0; i < 3; ++i)
//            colorSliders[i]->setValue(rgbaColor->getValues(i));
//    } else {
//        colorPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
//        for (int i = 0; i < 3; ++i)
//            colorSliders[i]->setValue(0.5);
//    }
}

/*
 * createButtonBox - Create a box to hold buttons.
 *
 * parameter colorMapDialog - GLMotif::RowColumn * &
 * return - GLMotif::RowColumn *
 */
GLMotif::RowColumn * Contours::createButtonBox(GLMotif::RowColumn * & colorMapDialog) {
    GLMotif::RowColumn * buttonBox = new GLMotif::RowColumn("ButtonBox", colorMapDialog, false);
    buttonBox->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    GLMotif::Button * removeControlPointButton =
            new GLMotif::Button("RemoveControlPointButton", buttonBox, "Remove RGB Control Point");
    removeControlPointButton->getSelectCallbacks().add(this, &Contours::removeControlPointCallback);
//    // Selection menu for colormap
//    GLMotif::CascadeButton * sliceColorMapSubCascade =
//            new GLMotif::CascadeButton("SliceColorMapSubCascade", buttonBox, "Color Map");
//    sliceColorMapSubCascade->setPopup(createSliceColorMapSubMenu());
    return buttonBox;
}

/*
 * createAlphaComponent - Create alpha component.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 */
void Contours::createAlphaComponent(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& contoursDialog) {
    alphaComponent = new ScalarWidget("AlphaComponent", contoursDialog, 3);
    alphaComponent->setBorderWidth(styleSheet.size * 0.5f);
    alphaComponent->setBorderType(GLMotif::Widget::LOWERED);
    alphaComponent->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
    alphaComponent->setMarginWidth(styleSheet.size);
    alphaComponent->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 20.0, styleSheet.fontHeight * 10.0, 0.0f));
    alphaComponent->setControlPointSize(styleSheet.size);
    alphaComponent->setControlPointScalar(1.0f);
    alphaComponent->getControlPointChangedCallbacks().add(this, &Contours::alphaControlPointChangedCallback);
} // end createAlphaComponent()

/*
 * exportAlpha - Export the alpha.
 *
 * parameter colormap - double*
 */
void Contours::getControlPointValues(double* values) const {
  alphaComponent->exportScalar(values);
//    values = alphaComponent->getStorage()->getScalars();
} // end exportAlpha()

/*
 * exportAlpha - Export the alpha.
 *
 * parameter colormap - double*
 */
int Contours::getNumberOfControlPoints(void) const {
    return alphaComponent->getStorage()->getNumberOfControlPoints();
} // end exportAlpha()

/*
///*
// * createColorEditor - Create color editor.
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorMapDialog - GLMotif::RowColumn*&
// * return - GLMotif::RowColumn*
// */
//GLMotif::RowColumn* Contours::createColorEditor(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
//    GLMotif::RowColumn* colorEditor = new GLMotif::RowColumn("ColorEditor", colorMapDialog, false);
//    colorEditor->setOrientation(GLMotif::RowColumn::HORIZONTAL);
//    createColorSwatchesWidget(styleSheet, colorEditor);
//    GLMotif::RowColumn* colorSlidersBox = createColorSliderBox(styleSheet, colorEditor);
//    colorSlidersBox->manageChild();
//    new GLMotif::Blind("Filler", colorEditor);
//    return colorEditor;
//}
//
///*
// * createColorMap - Create color map.
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorMapDialog - GLMotif::RowColumn*&
// */
//void Contours::createColorMap(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
//    colorMap = new ColorMap("ColorMap", colorMapDialog);
//    colorMap->setBorderWidth(styleSheet.size * 0.5f);
//    colorMap->setBorderType(GLMotif::Widget::LOWERED);
//    colorMap->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
//    colorMap->setMarginWidth(styleSheet.size);
//    colorMap->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0, styleSheet.fontHeight * 5.0, 0.0f));
//    colorMap->setControlPointSize(styleSheet.size);
//    RGBAColor* rgbaColor = new RGBAColor(1.0f, 0.0f, 0.0f, 1.0f);
//    colorMap->setControlPointColor(rgbaColor);
//    colorMap->getControlPointChangedCallbacks().add(this, &Contours::controlPointChangedCallback);
//}
//
/*
 * createContoursDialog - Create color map dialog.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Contours::createContoursDialog(const GLMotif::StyleSheet& styleSheet) {
    GLMotif::RowColumn* contoursDialog = new GLMotif::RowColumn("ContoursDialog", this, false);
//    createColorMap(styleSheet, colorMapDialog);
//    createColorPanel(styleSheet, colorMapDialog);
//    GLMotif::RowColumn* colorEditor = createColorEditor(styleSheet, colorMapDialog);
//    colorEditor->manageChild();
    createXYZContours(styleSheet, contoursDialog);
    GLMotif::RowColumn* buttonBox = createButtonBox(contoursDialog);
    createAlphaComponent(styleSheet, contoursDialog);
    buttonBox->manageChild();
    contoursDialog->manageChild();
}
//
///*
// * createColorPanel - Create color panel.
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorMapDialog - GLMotif::RowColumn*&
// */
//void Contours::createColorPanel(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
//    GLMotif::RowColumn* colorPanel = new GLMotif::RowColumn("ColorPanel", colorMapDialog, false);
//    colorPanel->setOrientation(GLMotif::RowColumn::HORIZONTAL);
//    colorPanel->setMarginWidth(styleSheet.size);
//    colorPane = new GLMotif::Blind("ColorPane", colorPanel);
//    colorPane->setBorderWidth(styleSheet.size * 0.5f);
//    colorPane->setBorderType(GLMotif::Widget::LOWERED);
//    colorPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
//    colorPane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 20.0f, styleSheet.fontHeight * 1.0f, 0.0f));
//    colorPanel->manageChild();
//}
//
///*
// * createColorSwatchesWidget
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorEditor - GLMotif::RowColumn*&
// */
//void Contours::createColorSwatchesWidget(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorEditor) {
//    swatchesWidget = new SwatchesWidget("SwatchesWidget", colorEditor);
//    swatchesWidget->setBorderWidth(styleSheet.size * 0.5f);
//    swatchesWidget->setBorderType(GLMotif::Widget::LOWERED);
//    swatchesWidget->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
//    swatchesWidget->setMarginWidth(styleSheet.size);
//    swatchesWidget->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 15.0, styleSheet.fontHeight * 5.0, 0.0f));
//    swatchesWidget->getColorChangedCallbacks().add(this, &Contours::colorSwatchesWidgetCallback);
//} // end createColorSwatchesWidget()
//
//
///*
// * createColorSlider - Create color slider.
// *
// * parameter title - const char*
// * parameter color - GLMotif::Color
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorSliderBox - GLMotif::RowColumn*
// * return - GLMotif::Slider*
// */
//GLMotif::Slider* Contours::createColorSlider(const char* title, GLMotif::Color color, const GLMotif::StyleSheet& styleSheet,
//        GLMotif::RowColumn* colorSlidersBox) {
//    GLMotif::Slider* colorSlider = new GLMotif::Slider(title, colorSlidersBox, GLMotif::Slider::VERTICAL, styleSheet.fontHeight
//            * 5.0f);
//    colorSlider->setSliderColor(color);
//    colorSlider->setValueRange(0.0f, 1.0f, 0.01f);
//    colorSlider->setValue(0.5f);
//    colorSlider->getValueChangedCallbacks().add(this, &Contours::colorSliderCallback);
//    return colorSlider;
//}
//
///*
// * createColorSliderBox - Create box to contain color slider bars.
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorEditor - GLMotif::RowColumn*
// * return - GLMotif::RowColumn*
// */
//GLMotif::RowColumn* Contours::createColorSliderBox(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorEditor) {
//    GLMotif::RowColumn* colorSlidersBox = new GLMotif::RowColumn("ColorSliders", colorEditor, false);
//    colorSlidersBox->setOrientation(GLMotif::RowColumn::HORIZONTAL);
//    colorSlidersBox->setPacking(GLMotif::RowColumn::PACK_GRID);
//    createColorSliders(styleSheet, colorSlidersBox);
//    return colorSlidersBox;
//}
//
///*
// * createColorSliders - Create RGB color sliders.
// *
// * parameter styleSheet - const GLMotif::StyleSheet&
// * parameter colorSliderBox - GLMotif::RowColumn*
// */
//void Contours::createColorSliders(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorSlidersBox) {
//    colorSliders[0] = createColorSlider("RedSlider", GLMotif::Color(1.0f, 0.0f, 0.0f), styleSheet, colorSlidersBox);
//    colorSliders[1] = createColorSlider("GreenSlider", GLMotif::Color(0.0f, 1.0f, 0.0f), styleSheet, colorSlidersBox);
//    colorSliders[2] = createColorSlider("BlueSlider", GLMotif::Color(0.0f, 0.0f, 1.0f), styleSheet, colorSlidersBox);
//}
//
///*
// * createSliceColorMapSubMenu - Creates a submenu of the available slice color maps.
// *
// * return - GLMotif::Popup *
// */
//GLMotif::Popup * Contours::createSliceColorMapSubMenu(void) {
//    GLMotif::Popup * sliceColorMapSubMenuPopup = new GLMotif::Popup("SliceColorMapSubMenuPopup", Vrui::getWidgetManager());
//    GLMotif::RadioBox * sliceColorMaps = new GLMotif::RadioBox("SliceColorMaps", sliceColorMapSubMenuPopup, false);
//    sliceColorMaps->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
//    sliceColorMaps->addToggle("Full Rainbow");
//    sliceColorMaps->addToggle("Inverse Full Rainbow");
//    sliceColorMaps->addToggle("Rainbow");
//    sliceColorMaps->addToggle("Inverse Rainbow");
//    sliceColorMaps->addToggle("Cold to Hot");
//    sliceColorMaps->addToggle("Hot to Cold");
//    sliceColorMaps->addToggle("Black to White");
//    sliceColorMaps->addToggle("White to Black");
//    sliceColorMaps->addToggle("HSB Hues");
//    sliceColorMaps->addToggle("Inverse HSB Hues");
//    sliceColorMaps->addToggle("Davinci");
//    sliceColorMaps->addToggle("Inverse Davinci");
//    sliceColorMaps->addToggle("Seismic");
//    sliceColorMaps->addToggle("Inverse Seismic");
//    sliceColorMaps->setSelectedToggle(3);
//    sliceColorMaps->getValueChangedCallbacks().add(this, &Contours::changeContoursColorMapCallback);
//    sliceColorMaps->manageChild();
//    return sliceColorMapSubMenuPopup;
//} // end createSliceColorMapSubMenu()

/*
 * createXContours - Create x slice widgets.
 *
 * parameter xyzContoursRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Contours::createXContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create x slice button and slider+textfield
    GLMotif::ToggleButton * showXSliceToggle = new GLMotif::ToggleButton("ShowXSliceToggle", xyzContoursRowColumn, "X");
    showXSliceToggle->setToggle(false);
    showXSliceToggle->getValueChangedCallbacks().add(this, &Contours::toggleSelectCallback);
    showXSliceToggle->setToggleWidth(0.1f);
    xSliceValue = new GLMotif::TextField("XSliceValue", xyzContoursRowColumn, 7);
    xSliceValue->setPrecision(3);
    xSliceValue->setValue(0.0);
    GLMotif::Slider * xSliceSlider = new GLMotif::Slider("XSliceSlider", xyzContoursRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    xSliceSlider->setValueRange(0.0, float(exampleVTKReader->getWidth() - 1), 1.0);
    xSliceSlider->setValue(0.0);
    xSliceSlider->getValueChangedCallbacks().add(this, &Contours::sliderCallback);
} // end createXContours()

/*
 * createXYZContours - Create x, y and z slice widgets.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 */
void Contours::createXYZContours(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
    GLMotif::RowColumn * xyzContoursRowColumn = new GLMotif::RowColumn("XYZContoursRowColumn", colorMapDialog, false);
    xyzContoursRowColumn->setOrientation(GLMotif::RowColumn::VERTICAL);
    xyzContoursRowColumn->setNumMinorWidgets(GLsizei(3));
    createXContours(xyzContoursRowColumn, styleSheet);
    createYContours(xyzContoursRowColumn, styleSheet);
    createZContours(xyzContoursRowColumn, styleSheet);

    xyzContoursRowColumn->manageChild();
} // end createXYZContours()createXYZContours

/*
 * createYContours - Create x slice widgets.
 *
 * parameter xyzContoursRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Contours::createYContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create y slice button and slider+textfield
    GLMotif::ToggleButton * showYSliceToggle = new GLMotif::ToggleButton("ShowYSliceToggle", xyzContoursRowColumn, "Y");
    showYSliceToggle->setToggle(false);
    showYSliceToggle->getValueChangedCallbacks().add(this, &Contours::toggleSelectCallback);
    ySliceValue = new GLMotif::TextField("YSliceValue", xyzContoursRowColumn, 7);
    ySliceValue->setPrecision(3);
    ySliceValue->setValue(0.0);
    GLMotif::Slider * ySliceSlider = new GLMotif::Slider("YSliceSlider", xyzContoursRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    ySliceSlider->setValueRange(0.0, float(exampleVTKReader->getLength() - 1), 1.0);
    ySliceSlider->setValue(0.0);
    ySliceSlider->getValueChangedCallbacks().add(this, &Contours::sliderCallback);
} // end createYContours()

/*
 * createZContours - Create x slice widgets.
 *
 * parameter xyzContoursRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Contours::createZContours(GLMotif::RowColumn * & xyzContoursRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create z slice button and slider+textfield
    GLMotif::ToggleButton * showZSliceToggle = new GLMotif::ToggleButton("ShowZSliceToggle", xyzContoursRowColumn, "Z");
    showZSliceToggle->setToggle(false);
    showZSliceToggle->getValueChangedCallbacks().add(this, &Contours::toggleSelectCallback);
    zSliceValue = new GLMotif::TextField("ZSliceValue", xyzContoursRowColumn, 7);
    zSliceValue->setPrecision(3);
    zSliceValue->setValue(0.0);
    GLMotif::Slider * zSliceSlider = new GLMotif::Slider("ZSliceSlider", xyzContoursRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    zSliceSlider->setValueRange(0.0, float(exampleVTKReader->getHeight() - 1), 1.0);
    zSliceSlider->setValue(0.0);
    zSliceSlider->getValueChangedCallbacks().add(this, &Contours::sliderCallback);
} // end createZContours()

///*
// * exportContoursColorMap - Export the color map.
// *
// * parameter colormap - unsigned char *
// */
//void Contours::exportContoursColorMap(double* colormap) const {
//    colorMap->exportColorMap(colormap);
//} // end exportContoursColorMap()
//
///*
// * getColorMap - Get the slice color map.
// *
// * return - Storage *
// */
//Storage * Contours::getColorMap(void) const {
//    return colorMap->getColorMap();
//} // end getColorMap()
//
///*
// * setColorMap - Set the slice color map.
// *
// * parameter storage - Storage*
// */
//void Contours::setColorMap(Storage* storage) {
//    colorMap->setColorMap(storage);
//} // end setColorMap()
//
///*
// * getContoursColorMap
// *
// * return const ColorMap *
// */
//const ColorMap * Contours::getContoursColorMap(void) const {
//    return colorMap;
//} // end getContoursColorMap()
//
///*
// * getContoursColorMap
// *
// * return - ColorMap *
// */
//ColorMap * Contours::getContoursColorMap(void) {
//    return colorMap;
//} // end getContoursColorMap()
//
///*
// * setContoursColorMap - Create the slice color map.
// *
// * parameter colorMapCreationType - int
// * parameter _minimum - double
// * parameter _maximum - double
// */
//void Contours::setContoursColorMap(int colorMapCreationType, double _minimum, double _maximum) {
//    colorMap->createColorMap(colorMapCreationType, _minimum, _maximum);
//} // end setContoursColorMap()
//
///*
// * getContoursColorMapChangedCallbacks
// *
// * return - Misc::CallbackList &
// */
//Misc::CallbackList& Contours::getContoursColorMapChangedCallbacks(void) {
//    return colorMap->getColorMapChangedCallbacks();
//} // end getContoursColorMapChangedCallbacks()

/*
 * initialize - Initialize the GUI for the Contours class.
 */
void Contours::initialize(void) {
    const GLMotif::StyleSheet& styleSheet = *Vrui::getWidgetManager()->getStyleSheet();
    createContoursDialog(styleSheet);
}

/*
 * removeControlPointCallback - Remove the current control point.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Contours::removeControlPointCallback(Misc::CallbackData* _callbackData) {
    alphaComponent->deleteControlPoint();
//    colorMap->deleteControlPoint();
} // end removeControlPointCallback()

///*
// * sliceColorMapChangedCallback - Change the slice color map.
// *
// * parameter callBackData - Misc::CallbackData *
// */
//void Contours::sliceColorMapChangedCallback(Misc::CallbackData * callBackData) {
//    exportContoursColorMap(sliceColormap);
//    exampleVTKReader->updateSliceColorMap(sliceColormap);
//    Vrui::requestUpdate();
//} // end sliceColorMapChangedCallback()

/*
 * sliderCallback
 *
 * parameter callBackData - GLMotif::Slider::ValueChangedCallbackData *
 */
void Contours::sliderCallback(GLMotif::Slider::ValueChangedCallbackData * callBackData) {
    if (strcmp(callBackData->slider->getName(), "XSliceSlider") == 0) {
        xSliceValue->setValue(callBackData->value);
        exampleVTKReader->setXSlice(int(callBackData->value));
        Vrui::requestUpdate();
    }
    if (strcmp(callBackData->slider->getName(), "YSliceSlider") == 0) {
        ySliceValue->setValue(callBackData->value);
        exampleVTKReader->setYSlice(int(callBackData->value));
        Vrui::requestUpdate();
    }
    if (strcmp(callBackData->slider->getName(), "ZSliceSlider") == 0) {
        zSliceValue->setValue(callBackData->value);
        exampleVTKReader->setZSlice(int(callBackData->value));
        Vrui::requestUpdate();
    }
} // end sliderCallback()

/*
 * toggleSelectCallback - Adjust the gui/program state based on which toggle button changed state.
 *
 * parameter callBackData - GLMotif::ToggleButton::ValueChangedCallbackData*
 */
void Contours::toggleSelectCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData) {
    /* Adjust gui/program state based on which toggle button changed state: */
    if (strcmp(callBackData->toggle->getName(), "ShowXSliceToggle") == 0) {
        exampleVTKReader->showXSlice(callBackData->set);
    } else if (strcmp(callBackData->toggle->getName(), "ShowYSliceToggle") == 0) {
        exampleVTKReader->showYSlice(callBackData->set);
    } else if (strcmp(callBackData->toggle->getName(), "ShowZSliceToggle") == 0) {
        exampleVTKReader->showZSlice(callBackData->set);
    }
    Vrui::requestUpdate();
} // end toggleSelectCallback()

/*
 * alphaControlPointChangedCallback - Callback to select a control point.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Contours::alphaControlPointChangedCallback(Misc::CallbackData* _callbackData) {
    ScalarWidgetControlPointChangedCallbackData* callbackData =
            static_cast<ScalarWidgetControlPointChangedCallbackData*> (_callbackData);
    if (callbackData->getCurrentControlPoint() != 0) {
        float _alpha = alphaComponent->getControlPointScalar();
        std::cout << "Number = "<< alphaComponent->getNumberOfControlPoints() << " "<<  _alpha << std::endl;
    }
} // end alphaControlPointChangedCallback()

/*
 * getAlphaChangedCallbacks
 *
 * return - Misc::CallbackList&
 */
Misc::CallbackList& Contours::getAlphaChangedCallbacks(void) {
    return alphaComponent->getChangedCallbacks();
} // end getAlphaChangedCallbacks()

