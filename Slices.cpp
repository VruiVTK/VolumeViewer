/*
 * Slices.cpp - Methods for Slices class.
 *
 * Author: Patrick O'Leary
 * Created: August 11, 2008
 * Copyright: 2008. All rights reserved.
 */
#include <iostream>
#include <GL/GLColor.h>
#include <GLMotif/Button.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/Label.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/WidgetManager.h>
#include <Misc/File.h>

/* Vrui includes to use the Vrui interface */
#include <Vrui/Vrui.h>

#include "ColorMap.h"
#include "ControlPointChangedCallbackData.h"
#include "RGBAColor.h"
#include "Storage.h"
#include "Slices.h"
#include "SwatchesWidget.h"

/*
 * Slices - Constructor for Slices class.
 * 		extends GLMotif::PopupWindow
 */
Slices::Slices(double* _sliceColormap, ExampleVTKReader * _ExampleVTKReader) :
    GLMotif::PopupWindow("SlicesPopup", Vrui::getWidgetManager(), "Slices"), sliceColormap(_sliceColormap),
            exampleVTKReader(_ExampleVTKReader) {
    initialize();
}

/*
 * ~Slices - Destructor for Slices class.
 */
Slices::~Slices(void) {
}

/*
 * changeSlicesColorMap - Change the colormap.
 *
 * parameter colormap - int
 */
void Slices::changeSlicesColorMap(int colormap) const {
    colorMap->createColorMap(colormap);
} // end changeSlicesColorMap()

/*
 * changeSlicesColorMapCallback
 *
 * parameter callBackData - GLMotif::RadioBox::ValueChangedCallbackData *
 */
void Slices::changeSlicesColorMapCallback(GLMotif::RadioBox::ValueChangedCallbackData * callBackData) {
    /* Set the new slices colormap */
    int value = callBackData->radioBox->getToggleIndex(callBackData->newSelectedToggle);
    changeSlicesColorMap(value);
    exportSlicesColorMap(sliceColormap);
    exampleVTKReader->updateSliceColorMap(sliceColormap);
    Vrui::requestUpdate();
} // end changeSlicesColorMapCallback()


/*
 * colorSliderCallback - Callback of change to color slider value.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Slices::colorSliderCallback(Misc::CallbackData* _callbackData) {
    RGBAColor* rgbaColor = new RGBAColor(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 3; ++i) {
        rgbaColor->setValues(i, float(colorSliders[i]->getValue()));
    }
    rgbaColor->setValues(3, 1.0f);
    colorPane->setBackgroundColor(rgbaColor->getValues());
    colorMap->setControlPointColor(*rgbaColor);
}

/*
 * colorSwatchesWidgetCallback
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Slices::colorSwatchesWidgetCallback(Misc::CallbackData* _callbackData) {
    float* _color = swatchesWidget->getCurrentColor();
    RGBAColor* rgbaColor = new RGBAColor(_color[0], _color[1], _color[2], 1.0f);
    for (int i = 0; i < 3; ++i) {
        colorSliders[i]->setValue(_color[i]);
    }
    colorPane->setBackgroundColor(rgbaColor->getValues());
    colorMap->setControlPointColor(*rgbaColor);
} // end colorSwatchesWidgetCallback()

/*
 * controlPointChangedCallback - Callback to select a control point.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Slices::controlPointChangedCallback(Misc::CallbackData* _callbackData) {
    ControlPointChangedCallbackData* callbackData = static_cast<ControlPointChangedCallbackData*> (_callbackData);
    if (callbackData->getCurrentControlPoint() != 0) {
        RGBAColor* rgbaColor = colorMap->getControlPointColor();
        colorPane->setBackgroundColor(rgbaColor->getValues());
        for (int i = 0; i < 3; ++i)
            colorSliders[i]->setValue(rgbaColor->getValues(i));
    } else {
        colorPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
        for (int i = 0; i < 3; ++i)
            colorSliders[i]->setValue(0.5);
    }
}

/*
 * createButtonBox - Create a box to hold buttons.
 *
 * parameter colorMapDialog - GLMotif::RowColumn * &
 * return - GLMotif::RowColumn *
 */
GLMotif::RowColumn * Slices::createButtonBox(GLMotif::RowColumn * & colorMapDialog) {
    GLMotif::RowColumn * buttonBox = new GLMotif::RowColumn("ButtonBox", colorMapDialog, false);
    buttonBox->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    GLMotif::Button * removeControlPointButton =
            new GLMotif::Button("RemoveControlPointButton", buttonBox, "Remove RGB Control Point");
    removeControlPointButton->getSelectCallbacks().add(this, &Slices::removeControlPointCallback);
    // Selection menu for colormap
    GLMotif::CascadeButton * sliceColorMapSubCascade =
            new GLMotif::CascadeButton("SliceColorMapSubCascade", buttonBox, "Color Map");
    sliceColorMapSubCascade->setPopup(createSliceColorMapSubMenu());
    return buttonBox;
}

/*
 * createColorEditor - Create color editor.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 * return - GLMotif::RowColumn*
 */
GLMotif::RowColumn* Slices::createColorEditor(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
    GLMotif::RowColumn* colorEditor = new GLMotif::RowColumn("ColorEditor", colorMapDialog, false);
    colorEditor->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    createColorSwatchesWidget(styleSheet, colorEditor);
    GLMotif::RowColumn* colorSlidersBox = createColorSliderBox(styleSheet, colorEditor);
    colorSlidersBox->manageChild();
    new GLMotif::Blind("Filler", colorEditor);
    return colorEditor;
}

/*
 * createColorMap - Create color map.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 */
void Slices::createColorMap(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
    colorMap = new ColorMap("ColorMap", colorMapDialog);
    colorMap->setBorderWidth(styleSheet.size * 0.5f);
    colorMap->setBorderType(GLMotif::Widget::LOWERED);
    colorMap->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
    colorMap->setMarginWidth(styleSheet.size);
    colorMap->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 5.0, styleSheet.fontHeight * 5.0, 0.0f));
    colorMap->setControlPointSize(styleSheet.size);
    RGBAColor* rgbaColor = new RGBAColor(1.0f, 0.0f, 0.0f, 1.0f);
    colorMap->setControlPointColor(rgbaColor);
    colorMap->getControlPointChangedCallbacks().add(this, &Slices::controlPointChangedCallback);
}

/*
 * createColorMapDialog - Create color map dialog.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Slices::createColorMapDialog(const GLMotif::StyleSheet& styleSheet) {
    GLMotif::RowColumn* colorMapDialog = new GLMotif::RowColumn("ColorMapDialog", this, false);
    createColorMap(styleSheet, colorMapDialog);
    createColorPanel(styleSheet, colorMapDialog);
    GLMotif::RowColumn* colorEditor = createColorEditor(styleSheet, colorMapDialog);
    colorEditor->manageChild();
    createXYZSlices(styleSheet, colorMapDialog);
    GLMotif::RowColumn* buttonBox = createButtonBox(colorMapDialog);
    buttonBox->manageChild();
    colorMapDialog->manageChild();
}

/*
 * createColorPanel - Create color panel.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 */
void Slices::createColorPanel(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
    GLMotif::RowColumn* colorPanel = new GLMotif::RowColumn("ColorPanel", colorMapDialog, false);
    colorPanel->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    colorPanel->setMarginWidth(styleSheet.size);
    colorPane = new GLMotif::Blind("ColorPane", colorPanel);
    colorPane->setBorderWidth(styleSheet.size * 0.5f);
    colorPane->setBorderType(GLMotif::Widget::LOWERED);
    colorPane->setBackgroundColor(GLMotif::Color(0.5f, 0.5f, 0.5f));
    colorPane->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 20.0f, styleSheet.fontHeight * 1.0f, 0.0f));
    colorPanel->manageChild();
}

/*
 * createColorSwatchesWidget
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorEditor - GLMotif::RowColumn*&
 */
void Slices::createColorSwatchesWidget(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorEditor) {
    swatchesWidget = new SwatchesWidget("SwatchesWidget", colorEditor);
    swatchesWidget->setBorderWidth(styleSheet.size * 0.5f);
    swatchesWidget->setBorderType(GLMotif::Widget::LOWERED);
    swatchesWidget->setForegroundColor(GLMotif::Color(0.0f, 1.0f, 0.0f));
    swatchesWidget->setMarginWidth(styleSheet.size);
    swatchesWidget->setPreferredSize(GLMotif::Vector(styleSheet.fontHeight * 15.0, styleSheet.fontHeight * 5.0, 0.0f));
    swatchesWidget->getColorChangedCallbacks().add(this, &Slices::colorSwatchesWidgetCallback);
} // end createColorSwatchesWidget()


/*
 * createColorSlider - Create color slider.
 *
 * parameter title - const char*
 * parameter color - GLMotif::Color
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorSliderBox - GLMotif::RowColumn*
 * return - GLMotif::Slider*
 */
GLMotif::Slider* Slices::createColorSlider(const char* title, GLMotif::Color color, const GLMotif::StyleSheet& styleSheet,
        GLMotif::RowColumn* colorSlidersBox) {
    GLMotif::Slider* colorSlider = new GLMotif::Slider(title, colorSlidersBox, GLMotif::Slider::VERTICAL, styleSheet.fontHeight
            * 5.0f);
    colorSlider->setSliderColor(color);
    colorSlider->setValueRange(0.0f, 1.0f, 0.01f);
    colorSlider->setValue(0.5f);
    colorSlider->getValueChangedCallbacks().add(this, &Slices::colorSliderCallback);
    return colorSlider;
}

/*
 * createColorSliderBox - Create box to contain color slider bars.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorEditor - GLMotif::RowColumn*
 * return - GLMotif::RowColumn*
 */
GLMotif::RowColumn* Slices::createColorSliderBox(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorEditor) {
    GLMotif::RowColumn* colorSlidersBox = new GLMotif::RowColumn("ColorSliders", colorEditor, false);
    colorSlidersBox->setOrientation(GLMotif::RowColumn::HORIZONTAL);
    colorSlidersBox->setPacking(GLMotif::RowColumn::PACK_GRID);
    createColorSliders(styleSheet, colorSlidersBox);
    return colorSlidersBox;
}

/*
 * createColorSliders - Create RGB color sliders.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorSliderBox - GLMotif::RowColumn*
 */
void Slices::createColorSliders(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn* colorSlidersBox) {
    colorSliders[0] = createColorSlider("RedSlider", GLMotif::Color(1.0f, 0.0f, 0.0f), styleSheet, colorSlidersBox);
    colorSliders[1] = createColorSlider("GreenSlider", GLMotif::Color(0.0f, 1.0f, 0.0f), styleSheet, colorSlidersBox);
    colorSliders[2] = createColorSlider("BlueSlider", GLMotif::Color(0.0f, 0.0f, 1.0f), styleSheet, colorSlidersBox);
}

/*
 * createSliceColorMapSubMenu - Creates a submenu of the available slice color maps.
 *
 * return - GLMotif::Popup *
 */
GLMotif::Popup * Slices::createSliceColorMapSubMenu(void) {
    GLMotif::Popup * sliceColorMapSubMenuPopup = new GLMotif::Popup("SliceColorMapSubMenuPopup", Vrui::getWidgetManager());
    GLMotif::RadioBox * sliceColorMaps = new GLMotif::RadioBox("SliceColorMaps", sliceColorMapSubMenuPopup, false);
    sliceColorMaps->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
    sliceColorMaps->addToggle("Full Rainbow");
    sliceColorMaps->addToggle("Inverse Full Rainbow");
    sliceColorMaps->addToggle("Rainbow");
    sliceColorMaps->addToggle("Inverse Rainbow");
    sliceColorMaps->addToggle("Cold to Hot");
    sliceColorMaps->addToggle("Hot to Cold");
    sliceColorMaps->addToggle("Black to White");
    sliceColorMaps->addToggle("White to Black");
    sliceColorMaps->addToggle("HSB Hues");
    sliceColorMaps->addToggle("Inverse HSB Hues");
    sliceColorMaps->addToggle("Davinci");
    sliceColorMaps->addToggle("Inverse Davinci");
    sliceColorMaps->addToggle("Seismic");
    sliceColorMaps->addToggle("Inverse Seismic");
    sliceColorMaps->setSelectedToggle(3);
    sliceColorMaps->getValueChangedCallbacks().add(this, &Slices::changeSlicesColorMapCallback);
    sliceColorMaps->manageChild();
    return sliceColorMapSubMenuPopup;
} // end createSliceColorMapSubMenu()

/*
 * createXSlices - Create x slice widgets.
 *
 * parameter xyzSlicesRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Slices::createXSlices(GLMotif::RowColumn * & xyzSlicesRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create x slice button and slider+textfield
    GLMotif::ToggleButton * showXSliceToggle = new GLMotif::ToggleButton("ShowXSliceToggle", xyzSlicesRowColumn, "X");
    showXSliceToggle->setToggle(false);
    showXSliceToggle->getValueChangedCallbacks().add(this, &Slices::toggleSelectCallback);
    showXSliceToggle->setToggleWidth(0.1f);
    xSliceValue = new GLMotif::TextField("XSliceValue", xyzSlicesRowColumn, 7);
    xSliceValue->setPrecision(3);
    xSliceValue->setValue(0.0);
    GLMotif::Slider * xSliceSlider = new GLMotif::Slider("XSliceSlider", xyzSlicesRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    xSliceSlider->setValueRange(0.0, float(exampleVTKReader->getWidth() - 1), 1.0);
    xSliceSlider->setValue(0.0);
    xSliceSlider->getValueChangedCallbacks().add(this, &Slices::sliderCallback);
} // end createXSlices()

/*
 * createXYZSlices - Create x, y and z slice widgets.
 *
 * parameter styleSheet - const GLMotif::StyleSheet&
 * parameter colorMapDialog - GLMotif::RowColumn*&
 */
void Slices::createXYZSlices(const GLMotif::StyleSheet& styleSheet, GLMotif::RowColumn*& colorMapDialog) {
    GLMotif::RowColumn * xyzSlicesRowColumn = new GLMotif::RowColumn("XYZSlicesRowColumn", colorMapDialog, false);
    xyzSlicesRowColumn->setOrientation(GLMotif::RowColumn::VERTICAL);
    xyzSlicesRowColumn->setNumMinorWidgets(GLsizei(3));
    createXSlices(xyzSlicesRowColumn, styleSheet);
    createYSlices(xyzSlicesRowColumn, styleSheet);
    createZSlices(xyzSlicesRowColumn, styleSheet);

    xyzSlicesRowColumn->manageChild();
} // end createXYZSlices()createXYZSlices

/*
 * createYSlices - Create x slice widgets.
 *
 * parameter xyzSlicesRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Slices::createYSlices(GLMotif::RowColumn * & xyzSlicesRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create y slice button and slider+textfield
    GLMotif::ToggleButton * showYSliceToggle = new GLMotif::ToggleButton("ShowYSliceToggle", xyzSlicesRowColumn, "Y");
    showYSliceToggle->setToggle(false);
    showYSliceToggle->getValueChangedCallbacks().add(this, &Slices::toggleSelectCallback);
    ySliceValue = new GLMotif::TextField("YSliceValue", xyzSlicesRowColumn, 7);
    ySliceValue->setPrecision(3);
    ySliceValue->setValue(0.0);
    GLMotif::Slider * ySliceSlider = new GLMotif::Slider("YSliceSlider", xyzSlicesRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    ySliceSlider->setValueRange(0.0, float(exampleVTKReader->getLength() - 1), 1.0);
    ySliceSlider->setValue(0.0);
    ySliceSlider->getValueChangedCallbacks().add(this, &Slices::sliderCallback);
} // end createYSlices()

/*
 * createZSlices - Create x slice widgets.
 *
 * parameter xyzSlicesRowColumn - GLMotif::RowColumn * &
 * parameter styleSheet - const GLMotif::StyleSheet&
 */
void Slices::createZSlices(GLMotif::RowColumn * & xyzSlicesRowColumn, const GLMotif::StyleSheet & styleSheet) {
    //create z slice button and slider+textfield
    GLMotif::ToggleButton * showZSliceToggle = new GLMotif::ToggleButton("ShowZSliceToggle", xyzSlicesRowColumn, "Z");
    showZSliceToggle->setToggle(false);
    showZSliceToggle->getValueChangedCallbacks().add(this, &Slices::toggleSelectCallback);
    zSliceValue = new GLMotif::TextField("ZSliceValue", xyzSlicesRowColumn, 7);
    zSliceValue->setPrecision(3);
    zSliceValue->setValue(0.0);
    GLMotif::Slider * zSliceSlider = new GLMotif::Slider("ZSliceSlider", xyzSlicesRowColumn, GLMotif::Slider::HORIZONTAL, styleSheet.fontHeight * 10.0f);
    zSliceSlider->setValueRange(0.0, float(exampleVTKReader->getHeight() - 1), 1.0);
    zSliceSlider->setValue(0.0);
    zSliceSlider->getValueChangedCallbacks().add(this, &Slices::sliderCallback);
} // end createZSlices()

/*
 * exportSlicesColorMap - Export the color map.
 *
 * parameter colormap - unsigned char *
 */
void Slices::exportSlicesColorMap(double* colormap) const {
    colorMap->exportColorMap(colormap);
} // end exportSlicesColorMap()

/*
 * getColorMap - Get the slice color map.
 *
 * return - Storage *
 */
Storage * Slices::getColorMap(void) const {
    return colorMap->getColorMap();
} // end getColorMap()

/*
 * setColorMap - Set the slice color map.
 *
 * parameter storage - Storage*
 */
void Slices::setColorMap(Storage* storage) {
    colorMap->setColorMap(storage);
} // end setColorMap()

/*
 * getSlicesColorMap
 *
 * return const ColorMap *
 */
const ColorMap * Slices::getSlicesColorMap(void) const {
    return colorMap;
} // end getSlicesColorMap()

/*
 * getSlicesColorMap
 *
 * return - ColorMap *
 */
ColorMap * Slices::getSlicesColorMap(void) {
    return colorMap;
} // end getSlicesColorMap()

/*
 * setSlicesColorMap - Create the slice color map.
 *
 * parameter colorMapCreationType - int
 * parameter _minimum - double
 * parameter _maximum - double
 */
void Slices::setSlicesColorMap(int colorMapCreationType, double _minimum, double _maximum) {
    colorMap->createColorMap(colorMapCreationType, _minimum, _maximum);
} // end setSlicesColorMap()

/*
 * getSlicesColorMapChangedCallbacks
 *
 * return - Misc::CallbackList &
 */
Misc::CallbackList& Slices::getSlicesColorMapChangedCallbacks(void) {
    return colorMap->getColorMapChangedCallbacks();
} // end getSlicesColorMapChangedCallbacks()

/*
 * initialize - Initialize the GUI for the Slices class.
 */
void Slices::initialize(void) {
    const GLMotif::StyleSheet& styleSheet = *Vrui::getWidgetManager()->getStyleSheet();
    createColorMapDialog(styleSheet);
    getSlicesColorMapChangedCallbacks().add(this, &Slices::sliceColorMapChangedCallback);

}

/*
 * removeControlPointCallback - Remove the current control point.
 *
 * parameter _callbackData - Misc::CallbackData*
 */
void Slices::removeControlPointCallback(Misc::CallbackData* _callbackData) {
    colorMap->deleteControlPoint();
} // end removeControlPointCallback()

/*
 * sliceColorMapChangedCallback - Change the slice color map.
 *
 * parameter callBackData - Misc::CallbackData *
 */
void Slices::sliceColorMapChangedCallback(Misc::CallbackData * callBackData) {
    exportSlicesColorMap(sliceColormap);
    exampleVTKReader->updateSliceColorMap(sliceColormap);
    Vrui::requestUpdate();
} // end sliceColorMapChangedCallback()

/*
 * sliderCallback
 *
 * parameter callBackData - GLMotif::Slider::ValueChangedCallbackData *
 */
void Slices::sliderCallback(GLMotif::Slider::ValueChangedCallbackData * callBackData) {
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
void Slices::toggleSelectCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData) {
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
