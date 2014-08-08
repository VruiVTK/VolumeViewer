#ifndef _EXAMPLEVTKREADER_H
#define _EXAMPLEVTKREADER_H

// STL includes
#include <vector>

// OpenGL/Motif includes
#include <GL/gl.h>

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
class vtkActor;
class vtkColorTransferFunction;
class vtkContourFilter;
class vtkContourFilter;
class vtkCutter;
class vtkLight;
class vtkLookupTable;
class vtkPiecewiseFunction;
class vtkPlane;
class vtkPolyDataMapper;
class vtkVolume;
class vtkVolumeProperty;

class ExampleVTKReader:public Vrui::Application,public GLObject
{
/* Embedded classes: */
  typedef std::vector<BaseLocator*> BaseLocatorList;
private:
  struct DataItem;
//  struct DataItem : public GLObject::DataItem
//  {
//  /* Elements */
//  public:
//    /* VTK components */
//    vtkSmartPointer<ExternalVTKWidget> externalVTKWidget;
//    vtkSmartPointer<vtkActor> actor;
//    vtkSmartPointer<vtkActor> actorOutline;
//    vtkSmartPointer<vtkLight> flashlight;
//    vtkSmartPointer<vtkVolume> actorVolume;
//    vtkSmartPointer<vtkVolumeProperty> propertyVolume;
//    vtkSmartPointer<vtkCutter> xCutter;
//    vtkSmartPointer<vtkPolyDataMapper> xCutterMapper;
//    vtkSmartPointer<vtkActor> actorXCutter;
//    vtkSmartPointer<vtkCutter> yCutter;
//    vtkSmartPointer<vtkPolyDataMapper> yCutterMapper;
//    vtkSmartPointer<vtkActor> actorYCutter;
//    vtkSmartPointer<vtkCutter> zCutter;
//    vtkSmartPointer<vtkPolyDataMapper> zCutterMapper;
//    vtkSmartPointer<vtkActor> actorZCutter;
//    vtkSmartPointer<vtkContourFilter> aContour;
//    vtkSmartPointer<vtkPolyDataMapper> aContourMapper;
//    vtkSmartPointer<vtkActor> actorAContour;
//    vtkSmartPointer<vtkContourFilter> bContour;
//    vtkSmartPointer<vtkPolyDataMapper> bContourMapper;
//    vtkSmartPointer<vtkActor> actorBContour;
//    vtkSmartPointer<vtkContourFilter> cContour;
//    vtkSmartPointer<vtkPolyDataMapper> cContourMapper;
//    vtkSmartPointer<vtkActor> actorCContour;
//    vtkSmartPointer<vtkContourFilter> contourFilter;
//    vtkSmartPointer<vtkActor> contourActor;
//    vtkSmartPointer<vtkCutter> xContourCutter;
//    vtkSmartPointer<vtkCutter> yContourCutter;
//    vtkSmartPointer<vtkCutter> zContourCutter;
//    vtkSmartPointer<vtkActor> actorXContourCutter;
//    vtkSmartPointer<vtkActor> actorYContourCutter;
//    vtkSmartPointer<vtkActor> actorZContourCutter;
//    vtkSmartPointer<vtkCutter> freeSliceCutter;
//    vtkSmartPointer<vtkPolyDataMapper> freeSliceMapper;
//    vtkSmartPointer<vtkActor> freeSliceActor;
//
//    /* Constructor and destructor*/
//    DataItem(void);
//    virtual ~DataItem(void);
//  };
//
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

  /* Name of file to load */
  char* FileName;

  /* SmartVolumeMapper Requested RenderMode */
  int RequestedRenderMode;

  /* Opacity value */
  double Opacity;

  /* Representation Type */
  int RepresentationType;

  vtkSmartPointer<vtkLookupTable> modelLUT;

  int* DataDimensions;
  double* DataBounds;
  int* DataExtent;
  double* DataOrigin;
  double* DataSpacing;
  double* DataScalarRange;

  double xCenter;
  double yCenter;
  double zCenter;
  double xOrigin;
  double yOrigin;
  double zOrigin;

  vtkSmartPointer<vtkPlane> xPlane;
  vtkSmartPointer<vtkPlane> yPlane;
  vtkSmartPointer<vtkPlane> zPlane;

  int xSlice;
  int ySlice;
  int zSlice;

  float aIsosurface;
  float bIsosurface;
  float cIsosurface;

  vtkSmartPointer<vtkPlane> xContourPlane;
  vtkSmartPointer<vtkPlane> yContourPlane;
  vtkSmartPointer<vtkPlane> zContourPlane;

  int xContourSlice;
  int yContourSlice;
  int zContourSlice;

  bool Outline;
  bool Volume;

  double* VolumeColormap;
  vtkSmartPointer<vtkColorTransferFunction> colorFunction;
  vtkSmartPointer<vtkPiecewiseFunction> opacityFunction;
  TransferFunction1D* transferFunctionDialog;

  bool XSlice;
  bool YSlice;
  bool ZSlice;

  bool XContourSlice;
  bool YContourSlice;
  bool ZContourSlice;

  double* SliceColormap;
  vtkSmartPointer<vtkLookupTable> sliceLUT;
  Slices* slicesDialog;

  bool AIsosurface;
  bool BIsosurface;
  bool CIsosurface;

  double* IsosurfaceColormap;
  vtkSmartPointer<vtkLookupTable> isosurfaceLUT;
  Isosurfaces* isosurfacesDialog;

  /* Contours */
  Contours* ContoursDialog;
  bool ContourVisible;
  std::vector<double> ContourValues;
  float* Histogram;

  /* First Frame */
  bool FirstFrame;

  /* Data Center */
  Vrui::Point Center;

  /* Data Radius  */
  Vrui::Scalar Radius;

  BaseLocatorList baseLocators;

  /* Analysis Tools */
  int analysisTool;

  /* Clipping Plane */
  ClippingPlane * ClippingPlanes;
  int NumberOfClippingPlanes;

  /* Verbose */
  bool Verbose;

  /* Flashlight position and direction */
  int * FlashlightSwitch;
  double * FlashlightPosition;
  double * FlashlightDirection;

  /* Free Slice visibility, origin and normal */
  int * FreeSliceVisibility;
  double * FreeSliceOrigin;
  double * FreeSliceNormal;
  vtkSmartPointer<vtkPlane> freeSlicePlane;

public:
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

  void initialize(void);

  /* Methods to manage render context */
  virtual void initContext(GLContextData& contextData) const;
  virtual void display(GLContextData& contextData) const;
  virtual void frame(void);

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
  void updateModelColorMap(void);

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
};

#endif //_EXAMPLEVTKREADER_H
