#ifndef _EXAMPLEVTKREADER_H
#define _EXAMPLEVTKREADER_H

// STL includes
#include <vector>

// OpenGL/Motif includes
#include <GL/gl.h>

#include <vvApplication.h>

// Vrui includes
#include <GL/GLObject.h>
#include <GLMotif/PopupWindow.h>
#include <GLMotif/RadioBox.h>
#include <GLMotif/Slider.h>
#include <GLMotif/TextField.h>
#include <GLMotif/ToggleButton.h>
#include <Vrui/Application.h>

// VTK includes
#include <vtkSmartPointer.h>

/* Forward Declarations */
namespace GLMotif
{
  class Popup;
  class PopupMenu;
}

class BaseLocator;
class ClippingPlane;
class Contours;
class ExternalVTKWidget;
class Isosurfaces;
class Slices;
class TransferFunction1D;
class volApplicationState;
class vtkActor;
class vtkColorTransferFunction;
class vtkContourFilter;
class vtkContourFilter;
class vtkCutter;
class vtkExtractVOI;
class vtkLight;
class vtkLookupTable;
class vtkPiecewiseFunction;
class vtkPlane;
class vtkPolyDataMapper;
class vtkProperty;
class vtkVolume;
class vtkVolumeProperty;

class ExampleVTKReader : public vvApplication
{
/* Embedded classes: */
  typedef std::vector<BaseLocator*> BaseLocatorList;
private:
  struct DataItem;

  volApplicationState &m_volState;

  /* Elements: */
  GLMotif::PopupMenu* mainMenu; // The program's main menu
  GLMotif::PopupMenu* createMainMenu(void);
  GLMotif::Popup* createRepresentationMenu(void);
  GLMotif::Popup* createAnalysisToolsMenu(void);
  GLMotif::Popup* createWidgetsMenu(void);
  GLMotif::Popup*  createColorMapSubMenu(void);
  GLMotif::Popup*  createAlphaSubMenu(void);
  GLMotif::PopupWindow* renderingDialog;
  GLMotif::PopupWindow* createRenderingDialog(void);
  GLMotif::TextField* opacityValue;
  GLMotif::TextField* resolutionValue;

  bool lowResolution;
  /* Name of file to load */
  char* FileName;

  float aIsosurface;
  float bIsosurface;
  float cIsosurface;

  TransferFunction1D* transferFunctionDialog;

  Slices* slicesDialog;

  bool AIsosurface;
  bool BIsosurface;
  bool CIsosurface;

  double* IsosurfaceColormap;
  Isosurfaces* isosurfacesDialog;

  /* Contours */
  Contours* ContoursDialog;
  float* Histogram;

  /* First Frame */
  bool FirstFrame;

  BaseLocatorList baseLocators;

  /* Analysis Tools */
  int analysisTool;

  /* Clipping Plane */
  ClippingPlane * ClippingPlanes;
  int NumberOfClippingPlanes;

  /* Verbose */
  bool Verbose;

  /* Free Slice visibility, origin and normal */
  int * FreeSliceVisibility;
  double * FreeSliceOrigin;
  double * FreeSliceNormal;
  vtkSmartPointer<vtkPlane> freeSlicePlane;

public:
  using Superclass = vvApplication;

  /* Constructors and destructors: */
  ExampleVTKReader(int& argc,char**& argv);
  virtual ~ExampleVTKReader(void);

  /* Methods to set/get the filename to read */
  void setFileName(const char* name);
  const char* getFileName(void);

  /* Methods to set/get the requested render mode */
  void setRequestedRenderMode(int mode);
  int getRequestedRenderMode(void) const;

  /* Clipping Planes */
  ClippingPlane * getClippingPlanes(void);
  int getNumberOfClippingPlanes(void);

  /* Methods to set/get verbosity */
  void setVerbose(bool);
  bool getVerbose(void);

  /* Get Flashlight position and direction */
  int * getFlashlightSwitch(void);
  double * getFlashlightPosition(void);
  double * getFlashlightDirection(void);

  /* Contours */
  std::vector<double> getContourValues();
  float * getHistogram();

  /* Get Free Slice visibility, origin and normal*/
  int * getFreeSliceVisibility(void);
  double * getFreeSliceOrigin(void);
  double * getFreeSliceNormal(void);

  void initialize() override;

  /* Methods to manage render context */
  void initContext(GLContextData& contextData) const override;
  void display(GLContextData& contextData) const override;
  void frame() override;

  /* Callback methods */
  void centerDisplayCallback(Misc::CallbackData* cbData);
  void opacitySliderCallback(GLMotif::Slider::ValueChangedCallbackData* cbData);
  void changeRepresentationCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void showIsosurfacesDialogCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void showSlicesDialogCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void showContoursDialogCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void showTransferFunctionDialogCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void showRenderingDialogCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void changeAnalysisToolsCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void changeColorMapCallback(GLMotif::RadioBox::ValueChangedCallbackData* callBackData);
  void changeAlphaCallback(GLMotif::RadioBox::ValueChangedCallbackData* callBackData);
  void changeResolutionCallback(GLMotif::ToggleButton::ValueChangedCallbackData* callBackData);
  void changeSamplingCallback(GLMotif::Slider::ValueChangedCallbackData* callBackData);

  virtual void toolCreationCallback(Vrui::ToolManager::ToolCreationCallbackData* cbData);
  virtual void toolDestructionCallback(Vrui::ToolManager::ToolDestructionCallbackData* cbData);

  void setIsosurfaceColorMapChanged(bool SliceColorMapChanged);
  void updateIsosurfaceColorMap(double* SliceColormap);

  void setSliceColorMapChanged(bool SliceColorMapChanged);
  void updateSliceColorMap(double* SliceColormap);

  void alphaChangedCallback(Misc::CallbackData* callBackData);
  void contourValueChangedCallback(Misc::CallbackData* callBackData);
  void volumeColorMapChangedCallback(Misc::CallbackData* callBackData);
  void updateAlpha(void);
  void updateContourValue(void);
  void updateVolumeColorMap(void);

  void setAIsosurface(float aIsosurface);
  void setBIsosurface(float bIsosurface);
  void setCIsosurface(float cIsosurface);
  void showAIsosurface(bool AIsosurface);
  void showBIsosurface(bool BIsosurface);
  void showCIsosurface(bool CIsosurface);

  void setXSlice(int xSlice);
  void setYSlice(int ySlice);
  void setZSlice(int zSlice);
  void showXSlice(bool XSlice);
  void showYSlice(bool YSlice);
  void showZSlice(bool ZSlice);

  void setContourVisible(bool visible);
  void setXContourSlice(int xSlice);
  void setYContourSlice(int ySlice);
  void setZContourSlice(int zSlice);
  void showXContourSlice(bool XSlice);
  void showYContourSlice(bool YSlice);
  void showZContourSlice(bool ZSlice);

  int getWidth(void);
  int getLength(void);
  int getHeight(void);

  float getDataMinimum(void);
  float getDataMaximum(void);
  float getDataIncrement(void);
  float getDataMidPoint(void);

protected:
  vvContextState* createContextState() const override;
};

#endif //_EXAMPLEVTKREADER_H
