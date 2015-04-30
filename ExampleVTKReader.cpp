// STD includes
#include <iostream>
#include <string>
#include <math.h>

// Must come before any gl.h include
#include <GL/glew.h>

// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkContourFilter.h>
#include <vtkPlaneCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractVOI.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkImageData.h>
#include <vtkLight.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlane.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkTimerLog.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkXMLImageDataReader.h>

// OpenGL/Motif includes
#include <GL/GLContextData.h>
#include <GL/gl.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/Label.h>
#include <GLMotif/Menu.h>
#include <GLMotif/Popup.h>
#include <GLMotif/PopupMenu.h>
#include <GLMotif/RadioBox.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/StyleSheet.h>
#include <GLMotif/SubMenu.h>
#include <GLMotif/WidgetManager.h>

// VRUI includes
#include <Vrui/Application.h>
#include <Vrui/Tool.h>
#include <Vrui/ToolManager.h>
#include <Vrui/Vrui.h>
#include <Vrui/VRWindow.h>
#include <Vrui/WindowProperties.h>

// ExampleVTKReader includes
#include "BaseLocator.h"
#include "ClippingPlane.h"
#include "ClippingPlaneLocator.h"
#include "ColorMap.h"
#include "Contours.h"
#include "DataItem.h"
#include "ExampleVTKReader.h"
#include "FlashlightLocator.h"
#include "Isosurfaces.h"
#include "FreeSliceLocator.h"
#include "ScalarWidget.h"
#include "Slices.h"
#include "TransferFunction1D.h"

#define ROTATIONS_MAX 1000

//----------------------------------------------------------------------------
ExampleVTKReader::ExampleVTKReader(int& argc,char**& argv)
  :Vrui::Application(argc,argv),
  aIsosurface(0),
  AIsosurface(false),
  analysisTool(0),
  bIsosurface(0),
  BIsosurface(false),
  cIsosurface(0),
  CIsosurface(false),
  ClippingPlanes(NULL),
  ContoursDialog(NULL),
  ContourVisible(true),
  FileName(0),
  FirstFrame(true),
  FlashlightDirection(0),
  FlashlightPosition(0),
  FlashlightSwitch(0),
  FreeSliceNormal(0),
  FreeSliceOrigin(0),
  FreeSliceVisibility(0),
  lowResolution(true),
  mainMenu(NULL),
  NumberOfClippingPlanes(6),
  Opacity(1.0),
  opacityValue(NULL),
  Outline(true),
  renderingDialog(NULL),
  RepresentationType(2),
  RequestedRenderMode(3),
  resolutionValue(NULL),
  sampling(4),
  Rotations(0),
  slicesDialog(NULL),
  transferFunctionDialog(NULL),
  Verbose(false),
  Volume(true),
  xCenter(0),
  xContourSlice(0),
  XContourSlice(false),
  xOrigin(0),
  xSlice(0),
  XSlice(false),
  yCenter(0),
  yContourSlice(0),
  YContourSlice(false),
  yOrigin(0),
  ySlice(0),
  YSlice(false),
  zCenter(0),
  zContourSlice(0),
  ZContourSlice(false),
  zOrigin(0),
  zSlice(0),
  ZSlice(false)
{
  /* Set Window properties:
   * Since the application requires translucency, GLX_ALPHA_SIZE is set to 1 at
   * context (VRWindow) creation time. To do this, we set the 4th component of
   * ColorBufferSize in WindowProperties to 1. This should be done in the
   * constructor to make sure it is set before the main loop is called.
   */
  Vrui::WindowProperties properties;
  properties.setColorBufferSize(0,1);
  Vrui::requestWindowProperties(properties);

  this->DataDimensions = new int[3];
  this->DataBounds = new double[6];
  this->DataExtent = new int[6];
  this->DataOrigin = new double[6];
  this->DataSpacing = new double[3];
  this->DataScalarRange = new double[2];

  this->FlashlightSwitch = new int[1];
  this->FlashlightSwitch[0] = 0;
  this->FlashlightPosition = new double[3];
  this->FlashlightDirection = new double[3];

  this->FreeSliceVisibility = new int[1];
  this->FreeSliceVisibility[0] = 0;
  this->FreeSliceOrigin = new double[3];
  this->FreeSliceNormal = new double[3];

  this->VolumeColormap = new double[4*256];

  this->IsosurfaceColormap = new double[4*256];

  this->SliceColormap = new double[4*256];

  this->xPlane = vtkSmartPointer<vtkPlane>::New();
  this->xPlane->SetOrigin(0.0, 0.0, 0.0);
  this->xPlane->SetNormal(1.0, 0.0, 0.0);

  this->yPlane = vtkSmartPointer<vtkPlane>::New();
  this->yPlane->SetOrigin(0.0, 0.0, 0.0);
  this->yPlane->SetNormal(0.0, 1.0, 0.0);

  this->zPlane = vtkSmartPointer<vtkPlane>::New();
  this->zPlane->SetOrigin(0.0, 0.0, 0.0);
  this->zPlane->SetNormal(0.0, 0.0, 1.0);

  this->xContourPlane = vtkSmartPointer<vtkPlane>::New();
  this->xContourPlane->SetOrigin(0.0, 0.0, 0.0);
  this->xContourPlane->SetNormal(1.0, 0.0, 0.0);

  this->yContourPlane = vtkSmartPointer<vtkPlane>::New();
  this->yContourPlane->SetOrigin(0.0, 0.0, 0.0);
  this->yContourPlane->SetNormal(0.0, 1.0, 0.0);

  this->zContourPlane = vtkSmartPointer<vtkPlane>::New();
  this->zContourPlane->SetOrigin(0.0, 0.0, 0.0);
  this->zContourPlane->SetNormal(0.0, 0.0, 1.0);

  this->Histogram = new float[256];
  for(int j = 0; j < 256; ++j)
    {
    this->Histogram[j] = 0.0;
    }

  this->freeSlicePlane = vtkSmartPointer<vtkPlane>::New();

  this->Timer = vtkSmartPointer<vtkTimerLog>::New();

  /* Initialize the clipping planes */
  ClippingPlanes = new ClippingPlane[NumberOfClippingPlanes];
  for(int i = 0; i < NumberOfClippingPlanes; ++i)
    {
    ClippingPlanes[i].setAllocated(false);
    ClippingPlanes[i].setActive(false);
    }
  initialize();
}

//----------------------------------------------------------------------------
ExampleVTKReader::~ExampleVTKReader(void)
{
  if(this->DataDimensions)
    {
    delete[] this->DataDimensions;
    }
  if(this->DataBounds)
    {
    delete[] this->DataBounds;
    }
  if(this->DataExtent)
    {
    delete[] this->DataExtent;
    }
  if(this->DataOrigin)
    {
    delete[] this->DataOrigin;
    }
  if(this->DataSpacing)
    {
    delete[] this->DataSpacing;
    }
  if(this->DataScalarRange)
    {
    delete[] this->DataScalarRange;
    }
  if(this->FlashlightSwitch)
    {
    delete[] this->FlashlightSwitch;
    }
  if(this->FlashlightPosition)
    {
    delete[] this->FlashlightPosition;
    }
  if(this->FlashlightDirection)
    {
    delete[] this->FlashlightDirection;
    }
  if(this->VolumeColormap)
    {
    delete[] this->VolumeColormap;
    }
  if(this->Histogram)
    {
    delete[] this->Histogram;
    }
  if(this->FreeSliceVisibility)
    {
    delete[] this->FreeSliceVisibility;
    }
  if(this->FreeSliceOrigin)
    {
    delete[] this->FreeSliceOrigin;
    }
  if(this->FreeSliceNormal)
    {
    delete[] this->FreeSliceNormal;
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::initialize(void)
{
    /* Create the user interface: */
  renderingDialog = createRenderingDialog();
  mainMenu=createMainMenu();
  Vrui::setMainMenu(mainMenu);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setFileName(const char* name)
{
  if(this->FileName && name && (!strcmp(this->FileName, name)))
    {
    return;
    }
  if(this->FileName && name)
    {
    delete [] this->FileName;
    }
  this->FileName = new char[strlen(name) + 1];
  strcpy(this->FileName, name);
}

//----------------------------------------------------------------------------
const char* ExampleVTKReader::getFileName(void)
{
  return this->FileName;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setVerbose(bool verbose)
{
  this->Verbose = verbose;
}

//----------------------------------------------------------------------------
bool ExampleVTKReader::getVerbose(void)
{
  return this->Verbose;
}

//----------------------------------------------------------------------------
GLMotif::PopupMenu* ExampleVTKReader::createMainMenu(void)
{
  GLMotif::PopupMenu* mainMenuPopup = new GLMotif::PopupMenu(
    "MainMenuPopup",Vrui::getWidgetManager());
  mainMenuPopup->setTitle("Main Menu");
  GLMotif::Menu* mainMenu = new GLMotif::Menu(
    "MainMenu",mainMenuPopup,false);

  const GLMotif::StyleSheet& styleSheet = *Vrui::getWidgetManager()->getStyleSheet();

  GLMotif::ToggleButton * showLowResolution =
  new GLMotif::ToggleButton("ShowLowResolution", mainMenu,
    "Low Resolution");
  showLowResolution->setToggle(lowResolution);
  showLowResolution->getValueChangedCallbacks().add(this, &ExampleVTKReader::changeResolutionCallback);

  GLMotif::CascadeButton* representationCascade =
    new GLMotif::CascadeButton("RepresentationCascade", mainMenu,
    "Representation");
  representationCascade->setPopup(createRepresentationMenu());

  GLMotif::CascadeButton* analysisToolsCascade =
    new GLMotif::CascadeButton("AnalysisToolsCascade", mainMenu,
      "Analysis Tools");
  analysisToolsCascade->setPopup(createAnalysisToolsMenu());

  GLMotif::CascadeButton * colorMapSubCascade =
    new GLMotif::CascadeButton("ColorMapSubCascade", mainMenu, "Color Map");
  colorMapSubCascade->setPopup(createColorMapSubMenu());

  GLMotif::CascadeButton * alphaSubCascade =
    new GLMotif::CascadeButton("AlphaSubCascade", mainMenu, "Opacity Ramp");
  alphaSubCascade->setPopup(createAlphaSubMenu());

  GLMotif::CascadeButton* widgetsCascade =
    new GLMotif::CascadeButton("WidgetsCascade", mainMenu,
      "Widgets");
  widgetsCascade->setPopup(createWidgetsMenu());

  GLMotif::Button* centerDisplayButton = new GLMotif::Button(
    "CenterDisplayButton",mainMenu,"Center Display");
  centerDisplayButton->getSelectCallbacks().add(
    this,&ExampleVTKReader::centerDisplayCallback);

   GLMotif::ToggleButton * showTransferFunctionDialog =
     new GLMotif::ToggleButton("ShowTransferFunctionDialog", mainMenu,
    "Transfer Function");
  showTransferFunctionDialog->setToggle(false);
  showTransferFunctionDialog->getValueChangedCallbacks().add(
    this, &ExampleVTKReader::showTransferFunctionDialogCallback);

  GLMotif::ToggleButton * showRenderingDialog = new GLMotif::ToggleButton(
    "ShowRenderingDialog", mainMenu,
    "Rendering");
  showRenderingDialog->setToggle(false);
  showRenderingDialog->getValueChangedCallbacks().add(
    this, &ExampleVTKReader::showRenderingDialogCallback);

  mainMenu->manageChild();
  return mainMenuPopup;
}

//----------------------------------------------------------------------------
GLMotif::Popup* ExampleVTKReader::createRepresentationMenu(void)
{
  const GLMotif::StyleSheet* ss = Vrui::getWidgetManager()->getStyleSheet();

  GLMotif::Popup* representationMenuPopup = new GLMotif::Popup(
    "representationMenuPopup", Vrui::getWidgetManager());
  GLMotif::SubMenu* representationMenu = new GLMotif::SubMenu(
    "representationMenu", representationMenuPopup, false);

  GLMotif::ToggleButton* showOutline=new GLMotif::ToggleButton(
    "ShowOutline",representationMenu,"Outline");
  showOutline->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  showOutline->setToggle(true);

  GLMotif::Label* representation_Label = new GLMotif::Label(
    "Representations", representationMenu,"Representations:");

  GLMotif::RadioBox* representation_RadioBox = new GLMotif::RadioBox(
    "Representation RadioBox",representationMenu,true);

  GLMotif::ToggleButton* showNone=new GLMotif::ToggleButton(
    "ShowNone",representation_RadioBox,"None");
  showNone->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  GLMotif::ToggleButton* showPoints=new GLMotif::ToggleButton(
    "ShowPoints",representation_RadioBox,"Points");
  showPoints->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  GLMotif::ToggleButton* showWireframe=new GLMotif::ToggleButton(
    "ShowWireframe",representation_RadioBox,"Wireframe");
  showWireframe->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  GLMotif::ToggleButton* showSurface=new GLMotif::ToggleButton(
    "ShowSurface",representation_RadioBox,"Surface");
  showSurface->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  GLMotif::ToggleButton* showSurfaceWithEdges=new GLMotif::ToggleButton(
    "ShowSurfaceWithEdges",representation_RadioBox,"Surface with Edges");
  showSurfaceWithEdges->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);
  GLMotif::ToggleButton* showVolume=new GLMotif::ToggleButton(
    "ShowVolume",representation_RadioBox,"Volume");
  showVolume->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeRepresentationCallback);

  representation_RadioBox->setSelectionMode(GLMotif::RadioBox::ATMOST_ONE);
  representation_RadioBox->setSelectedToggle(showSurface);

  representationMenu->manageChild();
  return representationMenuPopup;
}

//----------------------------------------------------------------------------
GLMotif::Popup * ExampleVTKReader::createAnalysisToolsMenu(void)
{
  const GLMotif::StyleSheet* ss = Vrui::getWidgetManager()->getStyleSheet();

  GLMotif::Popup * analysisToolsMenuPopup = new GLMotif::Popup(
    "analysisToolsMenuPopup", Vrui::getWidgetManager());
  GLMotif::SubMenu* analysisToolsMenu = new GLMotif::SubMenu(
    "representationMenu", analysisToolsMenuPopup, false);

  GLMotif::RadioBox * analysisTools_RadioBox = new GLMotif::RadioBox(
    "analysisTools", analysisToolsMenu, true);

  GLMotif::ToggleButton* showClippingPlane=new GLMotif::ToggleButton(
    "ClippingPlane",analysisTools_RadioBox,"Clipping Plane");
  showClippingPlane->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeAnalysisToolsCallback);
  GLMotif::ToggleButton* showFreeSlice=new GLMotif::ToggleButton(
    "FreeSlice",analysisTools_RadioBox,"Free Slice");
  showFreeSlice->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeAnalysisToolsCallback);
  GLMotif::ToggleButton* showFlashlight=new GLMotif::ToggleButton(
    "Flashlight",analysisTools_RadioBox,"Flashlight");
  showFlashlight->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeAnalysisToolsCallback);

  analysisTools_RadioBox->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
  analysisTools_RadioBox->setSelectedToggle(showClippingPlane);

  analysisToolsMenu->manageChild();
  return analysisToolsMenuPopup;
}

//----------------------------------------------------------------------------
GLMotif::Popup * ExampleVTKReader::createWidgetsMenu(void)
{
  const GLMotif::StyleSheet* ss = Vrui::getWidgetManager()->getStyleSheet();

  GLMotif::Popup * widgetsMenuPopup = new GLMotif::Popup(
    "widgetsMenuPopup", Vrui::getWidgetManager());
  GLMotif::SubMenu* widgetsMenu = new GLMotif::SubMenu(
    "widgetsMenu", widgetsMenuPopup, false);

  GLMotif::ToggleButton * showSlicesDialog = new GLMotif::ToggleButton(
    "ShowSlicesDialog", widgetsMenu, "Slices");
  showSlicesDialog->setToggle(false);
  showSlicesDialog->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::showSlicesDialogCallback);

  GLMotif::ToggleButton * showIsosurfacesDialog =
    new GLMotif::ToggleButton("ShowIsosurfacesDialog", widgetsMenu,
    "Isosurfaces");
  showIsosurfacesDialog->setToggle(false);
  showIsosurfacesDialog->getValueChangedCallbacks().add(
    this, &ExampleVTKReader::showIsosurfacesDialogCallback);

  GLMotif::ToggleButton * showContoursDialog = new GLMotif::ToggleButton(
    "ShowContoursDialog", widgetsMenu, "Contours");
  showContoursDialog->setToggle(false);
  showContoursDialog->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::showContoursDialogCallback);

  widgetsMenu->manageChild();
  return widgetsMenuPopup;
}

//----------------------------------------------------------------------------
GLMotif::Popup* ExampleVTKReader::createColorMapSubMenu(void)
{
  GLMotif::Popup * colorMapSubMenuPopup = new GLMotif::Popup(
    "ColorMapSubMenuPopup", Vrui::getWidgetManager());
  GLMotif::RadioBox* colorMaps = new GLMotif::RadioBox(
    "ColorMaps", colorMapSubMenuPopup, false);
  colorMaps->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
  colorMaps->addToggle("Full Rainbow");
  colorMaps->addToggle("Inverse Full Rainbow");
  colorMaps->addToggle("Rainbow");
  colorMaps->addToggle("Inverse Rainbow");
  colorMaps->addToggle("Cold to Hot");
  colorMaps->addToggle("Hot to Cold");
  colorMaps->addToggle("Black to White");
  colorMaps->addToggle("White to Black");
  colorMaps->addToggle("HSB Hues");
  colorMaps->addToggle("Inverse HSB Hues");
  colorMaps->addToggle("Davinci");
  colorMaps->addToggle("Inverse Davinci");
  colorMaps->addToggle("Seismic");
  colorMaps->addToggle("Inverse Seismic");
  colorMaps->setSelectedToggle(3);
  colorMaps->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::changeColorMapCallback);
  colorMaps->manageChild();
  return colorMapSubMenuPopup;
} // end createColorMapSubMenu()

//----------------------------------------------------------------------------
GLMotif::Popup*  ExampleVTKReader::createAlphaSubMenu(void)
{
  GLMotif::Popup * alphaSubMenuPopup = new GLMotif::Popup(
    "AlphaSubMenuPopup", Vrui::getWidgetManager());
  GLMotif::RadioBox* alphas = new GLMotif::RadioBox(
    "Alphas", alphaSubMenuPopup, false);
  alphas->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
  alphas->addToggle("Up");
  alphas->addToggle("Down");
  alphas->addToggle("Constant");
  alphas->addToggle("Seismic");
  alphas->setSelectedToggle(0);
  alphas->getValueChangedCallbacks().add(this, &ExampleVTKReader::changeAlphaCallback);
  alphas->manageChild();
  return alphaSubMenuPopup;
} // end createAlphaSubMenu()

//----------------------------------------------------------------------------
GLMotif::PopupWindow* ExampleVTKReader::createRenderingDialog(void)
{
  const GLMotif::StyleSheet& ss = *Vrui::getWidgetManager()->getStyleSheet();
  GLMotif::PopupWindow * dialogPopup = new GLMotif::PopupWindow(
    "RenderingDialogPopup", Vrui::getWidgetManager(),
    "Rendering Dialog");
  GLMotif::RowColumn * dialog = new GLMotif::RowColumn(
    "RenderingDialog", dialogPopup, false);
  dialog->setOrientation(GLMotif::RowColumn::VERTICAL);
  dialog->setNumMinorWidgets(GLsizei(3));


  /* Create opacity slider */
  GLMotif::Label* opacity_Label = new GLMotif::Label("Opacity", dialog,"Opacity:");
  GLMotif::Slider* opacitySlider = new GLMotif::Slider(
    "OpacitySlider", dialog, GLMotif::Slider::HORIZONTAL,
    ss.fontHeight*10.0f);
  opacitySlider->setValue(Opacity);
  opacitySlider->setValueRange(0.0, 1.0, 0.1);
  opacitySlider->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::opacitySliderCallback);
  opacityValue = new GLMotif::TextField("OpacityValue", dialog, 6);
  opacityValue->setFieldWidth(6);
  opacityValue->setPrecision(3);
  opacityValue->setValue(Opacity);

  /* Create sampling slider */
  GLMotif::Label* sampling_Label = new GLMotif::Label("Sampling", dialog,"Sampling:");
  GLMotif::Slider * resolutionSlider = new GLMotif::Slider("ResolutionSlider", dialog, GLMotif::Slider::HORIZONTAL, ss.fontHeight * 10.0f);
  resolutionSlider->setValueRange(1.0, 8.0, 1.0);
  resolutionSlider->setValue(float(this->sampling));
  resolutionSlider->getValueChangedCallbacks().add(this, &ExampleVTKReader::changeSamplingCallback);
  resolutionValue = new GLMotif::TextField("ResolutionValue", dialog, 6);
  resolutionValue->setFieldWidth(6);
  resolutionValue->setPrecision(1);
  resolutionValue->setValue(float(this->sampling));
  dialog->manageChild();

  return dialogPopup;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::frame(void)
{
  if(this->FirstFrame)
    {
    transferFunctionDialog = new TransferFunction1D(this);
    transferFunctionDialog->createTransferFunction1D(CINVERSE_RAINBOW,
      UP_RAMP, 0.0, 1.0);
    transferFunctionDialog->getColorMapChangedCallbacks().add(
      this, &ExampleVTKReader::volumeColorMapChangedCallback);
    transferFunctionDialog->getAlphaChangedCallbacks().add(this,
      &ExampleVTKReader::alphaChangedCallback);
    updateVolumeColorMap();
    updateAlpha();

    this->slicesDialog = new Slices(this->SliceColormap, this);
    this->slicesDialog->setSlicesColorMap(CINVERSE_RAINBOW, 0.0, 1.0);
    this->slicesDialog->exportSlicesColorMap(this->SliceColormap);
    updateSliceColorMap(this->SliceColormap);

    this->isosurfacesDialog = new Isosurfaces(this->IsosurfaceColormap, this);
    this->isosurfacesDialog->setIsosurfacesColorMap(CINVERSE_RAINBOW, 0.0, 1.0);
    this->isosurfacesDialog->exportIsosurfacesColorMap(this->IsosurfaceColormap);
    updateIsosurfaceColorMap(this->IsosurfaceColormap);

    this->ContoursDialog = new Contours(this);
    this->ContoursDialog->getAlphaChangedCallbacks().add(this,
      &ExampleVTKReader::contourValueChangedCallback);

    /* Compute the data center and Radius once */
    this->xCenter = (this->DataBounds[0] + this->DataBounds[1])/2.0;
    this->yCenter = (this->DataBounds[2] + this->DataBounds[3])/2.0;
    this->zCenter = (this->DataBounds[4] + this->DataBounds[5])/2.0;
    this->xOrigin = this->DataBounds[0];
    this->yOrigin = this->DataBounds[2];
    this->zOrigin = this->DataBounds[4];
    this->Center[0] = this->xCenter;
    this->Center[1] = this->yCenter;
    this->Center[2] = this->zCenter;

    this->Radius = sqrt((this->DataBounds[1] - this->DataBounds[0])*
                        (this->DataBounds[1] - this->DataBounds[0]) +
                        (this->DataBounds[3] - this->DataBounds[2])*
                        (this->DataBounds[3] - this->DataBounds[2]) +
                        (this->DataBounds[5] - this->DataBounds[4])*
                        (this->DataBounds[5] - this->DataBounds[4]));
    /* Scale the Radius */
    this->Radius *= 0.75;
    /* Initialize Vrui navigation transformation: */
    centerDisplayCallback(0);

    this->xPlane->SetOrigin(this->xOrigin + (
        this->xSlice * this->DataSpacing[0]), this->yCenter, this->zCenter);
    this->yPlane->SetOrigin(this->xCenter, this->yOrigin + (
        this->ySlice * this->DataSpacing[1]), this->zCenter);
    this->zPlane->SetOrigin(this->xCenter, this->yCenter,
      this->zOrigin + (this->zSlice * this->DataSpacing[2]));
    this->xContourPlane->SetOrigin(this->xOrigin + (
        this->xContourSlice * this->DataSpacing[0]), this->yCenter, this->zCenter);
    this->yContourPlane->SetOrigin(this->xCenter, this->yOrigin + (
        this->yContourSlice * this->DataSpacing[1]), this->zCenter);
    this->zContourPlane->SetOrigin(this->xCenter, this->yCenter,
      this->zOrigin + (this->zContourSlice * this->DataSpacing[2]));
    this->aIsosurface = this->getDataMidPoint();
    this->bIsosurface = this->getDataMidPoint();
    this->cIsosurface = this->getDataMidPoint();
    this->FirstFrame = false;
    this->Timer->StartTimer();
    }
  if (this->Rotations < ROTATIONS_MAX)
    {
    this->Rotations++;
    Vrui::scheduleUpdate(Vrui::getApplicationTime());
    }
  else
    {
    this->Timer->StopTimer();
    std::cout << "Time: " << this->Timer->GetElapsedTime() << std::endl;
    Vrui::shutdown();
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::initContext(GLContextData& contextData) const
{
  // The VTK OpenGL2 backend seems to require this:
  GLenum glewInitResult = glewInit();
  if (glewInitResult != GLEW_OK)
    {
    std::cerr << "Error: Could not initialize GLEW (glewInit() returned: "
      << glewInitResult << ")." << std::endl;
    }

  /* Create a new context data item */
  DataItem* dataItem = new DataItem();
  contextData.addDataItem(this, dataItem);

  vtkNew<vtkDataSetMapper> mapper;
  vtkNew<vtkDataSetMapper> lowMapper;
  dataItem->actor->SetMapper(mapper.GetPointer());
  dataItem->lowActor->SetMapper(lowMapper.GetPointer());

  vtkNew<vtkOutlineFilter> dataOutline;
  vtkNew<vtkOutlineFilter> lowDataOutline;

  vtkNew<vtkSmartVolumeMapper> mapperVolume;
  vtkNew<vtkSmartVolumeMapper> lowMapperVolume;

  if(this->FileName)
    {
    vtkNew<vtkXMLImageDataReader> reader;
    reader->SetFileName(this->FileName);
    reader->Update();

    reader->GetOutput()->GetDimensions(this->DataDimensions);
    reader->GetOutput()->GetBounds(this->DataBounds);
    reader->GetOutput()->GetOrigin(this->DataOrigin);
    reader->GetOutput()->GetExtent(this->DataExtent);
    reader->GetOutput()->GetSpacing(this->DataSpacing);
    reader->GetOutput()->GetScalarRange(this->DataScalarRange);

    dataItem->extract->SetInputConnection(reader->GetOutputPort());
    dataItem->extract->SetVOI(0, this->DataDimensions[0], 0, this->DataDimensions[1], 0, this->DataDimensions[2]);
    dataItem->extract->SetSampleRate(this->sampling, this->sampling, this->sampling);

    mapper->SetInputConnection(reader->GetOutputPort());
    lowMapper->SetInputConnection(dataItem->extract->GetOutputPort());

    dataOutline->SetInputConnection(reader->GetOutputPort());
    lowDataOutline->SetInputConnection(dataItem->extract->GetOutputPort());

    mapperVolume->SetInputConnection(reader->GetOutputPort());
    lowMapperVolume->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->xCutter->SetInputConnection(reader->GetOutputPort());
    dataItem->lowXCutter->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->yCutter->SetInputConnection(reader->GetOutputPort());
    dataItem->lowYCutter->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->zCutter->SetInputConnection(reader->GetOutputPort());
    dataItem->lowZCutter->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->aContour->SetInputConnection(reader->GetOutputPort());
    dataItem->lowAContour->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->bContour->SetInputConnection(reader->GetOutputPort());
    dataItem->lowBContour->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->cContour->SetInputConnection(reader->GetOutputPort());
    dataItem->lowCContour->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->contourFilter->SetInputConnection(reader->GetOutputPort());
    dataItem->lowContourFilter->SetInputConnection(dataItem->extract->GetOutputPort());

    dataItem->freeSliceCutter->SetInputConnection(reader->GetOutputPort());
    dataItem->lowFreeSliceCutter->SetInputConnection(dataItem->extract->GetOutputPort());

    for (int i = 0; i < this->DataDimensions[0]; ++i)
      {
      for (int j = 0; j < this->DataDimensions[1]; ++j)
        {
        for (int k = 0; k < this->DataDimensions[2]; ++k)
          {
          unsigned char * pixel = static_cast<unsigned char *>(
            reader->GetOutput()->GetScalarPointer(i,j,k));
          this->Histogram[static_cast<int>(pixel[0])] += 1;
          }
        }
      }

    }
  else
    {
    vtkNew<vtkImageData> imageData;
    this->DataDimensions[0] = 3;
    this->DataDimensions[1] = 3;
    this->DataDimensions[2] = 3;
    this->DataOrigin[0] = -1;
    this->DataOrigin[1] = -1;
    this->DataOrigin[2] = -1;
    this->DataSpacing[0] = 1;
    this->DataSpacing[1] = 1;
    this->DataSpacing[2] = 1;
    imageData->SetDimensions(this->DataDimensions);
    imageData->SetOrigin(this->DataOrigin);
    imageData->SetSpacing(this->DataSpacing);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
    imageData->GetBounds(this->DataBounds);
    imageData->GetScalarRange(this->DataScalarRange);
    imageData->GetExtent(this->DataExtent);

    mapper->SetInputData(imageData.GetPointer());

    dataOutline->SetInputData(imageData.GetPointer());

    mapperVolume->SetInputData(imageData.GetPointer());

    dataItem->xCutter->SetInputData(imageData.GetPointer());

    dataItem->yCutter->SetInputData(imageData.GetPointer());

    dataItem->zCutter->SetInputData(imageData.GetPointer());

    dataItem->aContour->SetInputData(imageData.GetPointer());

    dataItem->bContour->SetInputData(imageData.GetPointer());

    dataItem->cContour->SetInputData(imageData.GetPointer());

    dataItem->contourFilter->SetInputData(imageData.GetPointer());

    dataItem->freeSliceCutter->SetInputData(imageData.GetPointer());

    for (int i = 0; i < this->DataDimensions[0]; ++i)
      {
      for (int j = 0; j < this->DataDimensions[1]; ++j)
        {
        for (int k = 0; k < this->DataDimensions[2]; ++k)
          {
          unsigned char * pixel = static_cast<unsigned char *>(
            imageData->GetScalarPointer(i,j,k));
          this->Histogram[static_cast<int>(pixel[0])] += 1;
          }
        }
      }
    }

  mapper->SetScalarRange(this->DataScalarRange);
  mapper->SetLookupTable(dataItem->modelLUT);
  mapper->SetColorModeToMapScalars();
  lowMapper->SetScalarRange(this->DataScalarRange);
  lowMapper->SetLookupTable(dataItem->modelLUT);
  lowMapper->SetColorModeToMapScalars();

  vtkNew<vtkPolyDataMapper> mapperOutline;
  mapperOutline->SetInputConnection(dataOutline->GetOutputPort());
  dataItem->actorOutline->SetMapper(mapperOutline.GetPointer());
  dataItem->actorOutline->GetProperty()->SetColor(1,1,1);
  vtkNew<vtkPolyDataMapper> lowMapperOutline;
  lowMapperOutline->SetInputConnection(lowDataOutline->GetOutputPort());
  dataItem->lowActorOutline->SetMapper(lowMapperOutline.GetPointer());
  dataItem->lowActorOutline->GetProperty()->SetColor(1,1,1);

  mapperVolume->SetRequestedRenderMode(this->RequestedRenderMode);

  dataItem->colorFunction->AddRGBPoint(this->DataScalarRange[0], 1.0, 1.0, 1.0);
  dataItem->colorFunction->AddRGBPoint(this->DataScalarRange[1], 0.0, 0.0, 0.0);
  dataItem->opacityFunction->AddPoint(this->DataScalarRange[0], 0.0);
  dataItem->opacityFunction->AddPoint(this->DataScalarRange[1], 1.0);

  dataItem->propertyVolume->ShadeOff();
  dataItem->propertyVolume->SetScalarOpacityUnitDistance(1.0);
  dataItem->propertyVolume->SetColor(dataItem->colorFunction);
  dataItem->propertyVolume->SetScalarOpacity(dataItem->opacityFunction);
  dataItem->propertyVolume->SetInterpolationTypeToLinear();
  dataItem->actorVolume->SetProperty(dataItem->propertyVolume);
  dataItem->actorVolume->SetMapper(mapperVolume.GetPointer());
  dataItem->lowPropertyVolume->ShadeOff();
  dataItem->lowPropertyVolume->SetScalarOpacityUnitDistance(1.0);
  dataItem->lowPropertyVolume->SetColor(dataItem->colorFunction);
  dataItem->lowPropertyVolume->SetScalarOpacity(dataItem->opacityFunction);
  dataItem->lowPropertyVolume->SetInterpolationTypeToLinear();
  dataItem->lowActorVolume->SetProperty(dataItem->lowPropertyVolume);
  dataItem->lowActorVolume->SetMapper(lowMapperVolume.GetPointer());

  dataItem->xCutter->SetCutFunction(this->xPlane);
  dataItem->xCutterMapper->SetInputConnection(dataItem->xCutter->GetOutputPort());
  dataItem->xCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->xCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->xCutterMapper->SetColorModeToMapScalars();
  dataItem->actorXCutter->SetMapper(dataItem->xCutterMapper);
  dataItem->lowXCutter->SetCutFunction(this->xPlane);
  dataItem->lowXCutterMapper->SetInputConnection(dataItem->lowXCutter->GetOutputPort());
  dataItem->lowXCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowXCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->lowXCutterMapper->SetColorModeToMapScalars();
  dataItem->lowActorXCutter->SetMapper(dataItem->lowXCutterMapper);
  dataItem->yCutter->SetCutFunction(this->yPlane);
  dataItem->yCutterMapper->SetInputConnection(dataItem->yCutter->GetOutputPort());
  dataItem->yCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->yCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->yCutterMapper->SetColorModeToMapScalars();
  dataItem->actorYCutter->SetMapper(dataItem->yCutterMapper);
  dataItem->lowYCutter->SetCutFunction(this->yPlane);
  dataItem->lowYCutterMapper->SetInputConnection(dataItem->lowYCutter->GetOutputPort());
  dataItem->lowYCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowYCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->lowYCutterMapper->SetColorModeToMapScalars();
  dataItem->lowActorYCutter->SetMapper(dataItem->lowYCutterMapper);
  dataItem->zCutter->SetCutFunction(this->zPlane);
  dataItem->zCutterMapper->SetInputConnection(dataItem->zCutter->GetOutputPort());
  dataItem->zCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->zCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->zCutterMapper->SetColorModeToMapScalars();
  dataItem->actorZCutter->SetMapper(dataItem->zCutterMapper);
  dataItem->lowZCutter->SetCutFunction(this->zPlane);
  dataItem->lowZCutterMapper->SetInputConnection(dataItem->lowZCutter->GetOutputPort());
  dataItem->lowZCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowZCutterMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->lowZCutterMapper->SetColorModeToMapScalars();
  dataItem->lowActorZCutter->SetMapper(dataItem->lowZCutterMapper);

  dataItem->aContour->SetValue(0, this->aIsosurface);
  dataItem->aContourMapper->SetInputConnection(dataItem->aContour->GetOutputPort());
  dataItem->aContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->aContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->aContourMapper->SetColorModeToMapScalars();
  dataItem->actorAContour->SetMapper(dataItem->aContourMapper);
  dataItem->lowAContour->SetValue(0, this->aIsosurface);
  dataItem->lowAContourMapper->SetInputConnection(dataItem->lowAContour->GetOutputPort());
  dataItem->lowAContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowAContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->lowAContourMapper->SetColorModeToMapScalars();
  dataItem->lowActorAContour->SetMapper(dataItem->lowAContourMapper);
  dataItem->bContour->SetValue(0, this->bIsosurface);
  dataItem->bContourMapper->SetInputConnection(dataItem->bContour->GetOutputPort());
  dataItem->bContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->bContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->bContourMapper->SetColorModeToMapScalars();
  dataItem->actorBContour->SetMapper(dataItem->bContourMapper);
  dataItem->lowBContour->SetValue(0, this->bIsosurface);
  dataItem->lowBContourMapper->SetInputConnection(dataItem->lowBContour->GetOutputPort());
  dataItem->lowBContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowBContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->lowBContourMapper->SetColorModeToMapScalars();
  dataItem->lowActorBContour->SetMapper(dataItem->lowBContourMapper);
  dataItem->cContour->SetValue(0, this->cIsosurface);
  dataItem->cContourMapper->SetInputConnection(dataItem->cContour->GetOutputPort());
  dataItem->cContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->cContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->cContourMapper->SetColorModeToMapScalars();
  dataItem->actorCContour->SetMapper(dataItem->cContourMapper);
  dataItem->lowCContour->SetValue(0, this->cIsosurface);
  dataItem->lowCContourMapper->SetInputConnection(dataItem->lowCContour->GetOutputPort());
  dataItem->lowCContourMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowCContourMapper->SetLookupTable(dataItem->isosurfaceLUT);
  dataItem->lowCContourMapper->SetColorModeToMapScalars();
  dataItem->lowActorCContour->SetMapper(dataItem->lowCContourMapper);

  vtkNew<vtkPolyDataMapper> contourMapper;
  contourMapper->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  contourMapper->ScalarVisibilityOff();
  dataItem->contourActor->SetMapper(contourMapper.GetPointer());
  vtkNew<vtkPolyDataMapper> lowContourMapper;
  lowContourMapper->SetInputConnection(dataItem->lowContourFilter->GetOutputPort());
  lowContourMapper->ScalarVisibilityOff();
  dataItem->lowContourActor->SetMapper(lowContourMapper.GetPointer());

  vtkNew<vtkPlaneCutter> xContourCutter;
  xContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  xContourCutter->SetCutFunction(this->xContourPlane);
  vtkNew<vtkCompositePolyDataMapper> mapperXContourCutter;
  mapperXContourCutter->SetInputConnection(xContourCutter->GetOutputPort());
  mapperXContourCutter->ScalarVisibilityOff();
  dataItem->actorXContourCutter->SetMapper(mapperXContourCutter.GetPointer());
  vtkNew<vtkPlaneCutter> lowXContourCutter;
  lowXContourCutter->SetInputConnection(dataItem->lowContourFilter->GetOutputPort());
  lowXContourCutter->SetCutFunction(this->xContourPlane);
  vtkNew<vtkCompositePolyDataMapper> lowMapperXContourCutter;
  lowMapperXContourCutter->SetInputConnection(lowXContourCutter->GetOutputPort());
  lowMapperXContourCutter->ScalarVisibilityOff();
  dataItem->lowActorXContourCutter->SetMapper(lowMapperXContourCutter.GetPointer());
  vtkNew<vtkPlaneCutter> yContourCutter;
  yContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  yContourCutter->SetCutFunction(this->yContourPlane);
  vtkNew<vtkCompositePolyDataMapper> mapperYContourCutter;
  mapperYContourCutter->SetInputConnection(yContourCutter->GetOutputPort());
  mapperYContourCutter->ScalarVisibilityOff();
  dataItem->actorYContourCutter->SetMapper(mapperYContourCutter.GetPointer());
  vtkNew<vtkPlaneCutter> lowYContourCutter;
  lowYContourCutter->SetInputConnection(dataItem->lowContourFilter->GetOutputPort());
  lowYContourCutter->SetCutFunction(this->yContourPlane);
  vtkNew<vtkCompositePolyDataMapper> lowMapperYContourCutter;
  lowMapperYContourCutter->SetInputConnection(lowYContourCutter->GetOutputPort());
  lowMapperYContourCutter->ScalarVisibilityOff();
  dataItem->lowActorYContourCutter->SetMapper(lowMapperYContourCutter.GetPointer());
  vtkNew<vtkPlaneCutter> zContourCutter;
  zContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  zContourCutter->SetCutFunction(this->zContourPlane);
  vtkNew<vtkCompositePolyDataMapper> mapperZContourCutter;
  mapperZContourCutter->SetInputConnection(zContourCutter->GetOutputPort());
  mapperZContourCutter->ScalarVisibilityOff();
  dataItem->actorZContourCutter->SetMapper(mapperZContourCutter.GetPointer());
  vtkNew<vtkPlaneCutter> lowZContourCutter;
  lowZContourCutter->SetInputConnection(dataItem->lowContourFilter->GetOutputPort());
  lowZContourCutter->SetCutFunction(this->zContourPlane);
  vtkNew<vtkCompositePolyDataMapper> lowMapperZContourCutter;
  lowMapperZContourCutter->SetInputConnection(lowZContourCutter->GetOutputPort());
  lowMapperZContourCutter->ScalarVisibilityOff();
  dataItem->lowActorZContourCutter->SetMapper(lowMapperZContourCutter.GetPointer());
  dataItem->flashlight->SwitchOff();
  dataItem->flashlight->SetLightTypeToHeadlight();
  dataItem->flashlight->SetColor(0.0, 1.0, 1.0);
  dataItem->flashlight->SetConeAngle(15);
  dataItem->flashlight->SetPositional(true);

  dataItem->freeSliceCutter->SetCutFunction(this->freeSlicePlane);
  dataItem->freeSliceMapper->SetScalarRange(this->DataScalarRange);
  dataItem->freeSliceMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->freeSliceMapper->SetColorModeToMapScalars();
  dataItem->lowFreeSliceCutter->SetCutFunction(this->freeSlicePlane);
  dataItem->lowFreeSliceMapper->SetScalarRange(this->DataScalarRange);
  dataItem->lowFreeSliceMapper->SetLookupTable(dataItem->sliceLUT);
  dataItem->lowFreeSliceMapper->SetColorModeToMapScalars();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::display(GLContextData& contextData) const
{
  int numberOfSupportedClippingPlanes;
  glGetIntegerv(GL_MAX_CLIP_PLANES, &numberOfSupportedClippingPlanes);
  int clippingPlaneIndex = 0;
  for (int i = 0; i < NumberOfClippingPlanes &&
    clippingPlaneIndex < numberOfSupportedClippingPlanes; ++i)
    {
    if (ClippingPlanes[i].isActive())
      {
      /* Enable the clipping plane: */
      glEnable(GL_CLIP_PLANE0 + clippingPlaneIndex);
      GLdouble clippingPlane[4];
      for (int j = 0; j < 3; ++j)
          clippingPlane[j] = ClippingPlanes[i].getPlane().getNormal()[j];
      clippingPlane[3] = -ClippingPlanes[i].getPlane().getOffset();
      glClipPlane(GL_CLIP_PLANE0 + clippingPlaneIndex, clippingPlane);
      /* Go to the next clipping plane: */
      ++clippingPlaneIndex;
      }
    }

  /* Get context data item */
  DataItem* dataItem = contextData.retrieveDataItem<DataItem>(this);

  /* Update all lookup tables */
  dataItem->colorFunction->RemoveAllPoints();
  dataItem->opacityFunction->RemoveAllPoints();
  double step = (this->DataScalarRange[1] - this->DataScalarRange[0])/255.0;
  for (int i = 0; i < 256; ++i)
    {
    dataItem->modelLUT->SetTableValue(i,
      this->VolumeColormap[4*i + 0],
      this->VolumeColormap[4*i + 1],
      this->VolumeColormap[4*i + 2], 1.0);

    dataItem->sliceLUT->SetTableValue(i,
      this->SliceColormap[4*i + 0],
      this->SliceColormap[4*i + 1],
      this->SliceColormap[4*i + 2], 1.0);

    dataItem->isosurfaceLUT->SetTableValue(i,
      this->IsosurfaceColormap[4*i + 0],
      this->IsosurfaceColormap[4*i + 1],
      this->IsosurfaceColormap[4*i + 2], 1.0);

    dataItem->colorFunction->AddRGBPoint(
      this->DataScalarRange[0] + (double)(i*step),
      this->VolumeColormap[4*i + 0],
      this->VolumeColormap[4*i + 1],
      this->VolumeColormap[4*i + 2]);
    dataItem->opacityFunction->AddPoint(
      this->DataScalarRange[0] + (double)(i*step),
      this->VolumeColormap[4*i + 3]);
    }

  /* Turn off visibility of all actors in the scene */
  dataItem->actor->VisibilityOff();
  dataItem->lowActor->VisibilityOff();
  dataItem->actorOutline->VisibilityOff();
  dataItem->lowActorOutline->VisibilityOff();
  dataItem->actorVolume->VisibilityOff();
  dataItem->lowActorVolume->VisibilityOff();
  dataItem->actorXCutter->VisibilityOff();
  dataItem->lowActorXCutter->VisibilityOff();
  dataItem->actorYCutter->VisibilityOff();
  dataItem->lowActorYCutter->VisibilityOff();
  dataItem->actorZCutter->VisibilityOff();
  dataItem->lowActorZCutter->VisibilityOff();
  dataItem->actorAContour->VisibilityOff();
  dataItem->lowActorAContour->VisibilityOff();
  dataItem->actorBContour->VisibilityOff();
  dataItem->lowActorBContour->VisibilityOff();
  dataItem->actorCContour->VisibilityOff();
  dataItem->lowActorCContour->VisibilityOff();
  dataItem->actorXContourCutter->VisibilityOff();
  dataItem->lowActorXContourCutter->VisibilityOff();
  dataItem->actorYContourCutter->VisibilityOff();
  dataItem->lowActorYContourCutter->VisibilityOff();
  dataItem->actorZContourCutter->VisibilityOff();
  dataItem->lowActorZContourCutter->VisibilityOff();
  dataItem->contourActor->VisibilityOff();
  dataItem->lowContourActor->VisibilityOff();
  dataItem->freeSliceActor->VisibilityOff();
  dataItem->lowFreeSliceActor->VisibilityOff();
  if (lowResolution)
    {
    dataItem->extract->SetSampleRate(
      this->sampling, this->sampling, this->sampling);
    }

  if(this->FlashlightSwitch[0])
    {
    dataItem->flashlight->SetPosition(this->FlashlightPosition);
    dataItem->flashlight->SetFocalPoint(this->FlashlightDirection);
    dataItem->flashlight->SwitchOn();
    }
  else
    {
    dataItem->flashlight->SwitchOff();
    }

  if(this->FreeSliceVisibility[0])
    {
    this->freeSlicePlane->SetOrigin(this->FreeSliceOrigin);
    this->freeSlicePlane->SetNormal(this->FreeSliceNormal);
    if (!lowResolution)
      {
      dataItem->freeSliceActor->VisibilityOn();
      }
    else
      {
      dataItem->lowFreeSliceActor->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->freeSliceActor->VisibilityOff();
      }
    else
      {
      dataItem->lowFreeSliceActor->VisibilityOff();
      }
    }

  if (this->Outline)
    {
    if (!lowResolution)
      {
      dataItem->actorOutline->VisibilityOn();
      }
    else
      {
      dataItem->lowActorOutline->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorOutline->VisibilityOff();
      }
    else
      {
      dataItem->lowActorOutline->VisibilityOff();
      }
    }

  if (this->Volume)
    {
    if (!lowResolution)
      {
      dataItem->actorVolume->VisibilityOn();
      }
    else
      {
      dataItem->lowActorVolume->VisibilityOn();
      }
    if (!lowResolution)
      {
      dataItem->actor->VisibilityOff();
      }
    else
      {
      dataItem->lowActor->VisibilityOff();
      }
    if (this->Rotations < ROTATIONS_MAX)
      {
      dataItem->actorVolume->RotateY(1);
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorVolume->VisibilityOff();
      }
    else
      {
      dataItem->lowActorVolume->VisibilityOff();
      }
    if (!lowResolution)
      {
      dataItem->actor->GetProperty()->SetOpacity(this->Opacity);
      }
    else
      {
      dataItem->lowActor->GetProperty()->SetOpacity(this->Opacity);
      }
    if (this->RepresentationType != -1)
      {
      if (!lowResolution)
        {
        dataItem->actor->VisibilityOn();
        }
      else
        {
        dataItem->lowActor->VisibilityOn();
        }
      if (this->RepresentationType == 3)
        {
          if (!lowResolution) {
            dataItem->actor->GetProperty()->SetRepresentationToSurface();
            dataItem->actor->GetProperty()->EdgeVisibilityOn();
          }
          else {
            dataItem->lowActor->GetProperty()->SetRepresentationToSurface();
            dataItem->lowActor->GetProperty()->EdgeVisibilityOn();
          }
        }
      else
        {
          if (!lowResolution) {
            dataItem->actor->GetProperty()->SetRepresentation(this->RepresentationType);
            dataItem->actor->GetProperty()->EdgeVisibilityOff();
          }
          else {
            dataItem->lowActor->GetProperty()->SetRepresentation(this->RepresentationType);
            dataItem->lowActor->GetProperty()->EdgeVisibilityOff();
          }
        }
      }
    else
      {
        if (!lowResolution)
          {
          dataItem->actor->VisibilityOff();
          }
        else
          {
          dataItem->lowActor->VisibilityOff();
          }
      }
    }

  if (this->XSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorXCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorXCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorXCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorXCutter->VisibilityOff();
      }
    }

  if (this->YSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorYCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorYCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorYCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorYCutter->VisibilityOff();
      }
    }

  if (this->ZSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorZCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorZCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorZCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorZCutter->VisibilityOff();
      }
    }

  if (this->AIsosurface)
    {
      if (!lowResolution)
        {
        dataItem->actorAContour->VisibilityOn();
        dataItem->aContour->SetValue(0, this->aIsosurface);
        }
      else
        {
        dataItem->lowActorAContour->VisibilityOn();
        dataItem->lowAContour->SetValue(0, this->aIsosurface);
        }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorAContour->VisibilityOff();
      }
    else
      {
      dataItem->lowActorAContour->VisibilityOff();
      }
    }

  if (this->BIsosurface)
    {
      if (!lowResolution)
        {
        dataItem->actorBContour->VisibilityOn();
        dataItem->bContour->SetValue(0, this->bIsosurface);
        }
      else
        {
        dataItem->lowActorBContour->VisibilityOn();
        dataItem->lowBContour->SetValue(0, this->bIsosurface);
        }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorBContour->VisibilityOff();
      }
    else
      {
      dataItem->lowActorBContour->VisibilityOff();
      }
    }

  if (this->CIsosurface)
    {
    if (!lowResolution)
        {
        dataItem->actorCContour->VisibilityOn();
        dataItem->cContour->SetValue(0, this->cIsosurface);
        }
      else
        {
        dataItem->lowActorCContour->VisibilityOn();
        dataItem->lowCContour->SetValue(0, this->cIsosurface);
        }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorCContour->VisibilityOff();
      }
    else
      {
      dataItem->lowActorCContour->VisibilityOff();
      }
    }

  if (this->XContourSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorXContourCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorXContourCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorXContourCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorXContourCutter->VisibilityOff();
      }
    }

  if (this->YContourSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorYContourCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorYContourCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorYContourCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorYContourCutter->VisibilityOff();
      }
    }

  if (this->ZContourSlice)
    {
    if (!lowResolution)
      {
      dataItem->actorZContourCutter->VisibilityOn();
      }
    else
      {
      dataItem->lowActorZContourCutter->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->actorZContourCutter->VisibilityOff();
      }
    else
      {
      dataItem->lowActorZContourCutter->VisibilityOff();
      }
    }

  if (!lowResolution)
    {
    dataItem->contourFilter->SetNumberOfContours(this->ContourValues.size());
    }
  else
    {
    dataItem->lowContourFilter->SetNumberOfContours(
      this->ContourValues.size());
    }
  for(int i = 0; i < this->ContourValues.size(); ++i)
    {
    if (!lowResolution)
      {
      dataItem->contourFilter->SetValue(i, this->ContourValues.at(i));
      }
    else
      {
      dataItem->lowContourFilter->SetValue(i, this->ContourValues.at(i));
      }
    }
  if(this->ContourVisible)
    {
    if (!lowResolution)
      {
      dataItem->contourActor->VisibilityOn();
      }
    else
      {
      dataItem->lowContourActor->VisibilityOn();
      }
    }
  else
    {
    if (!lowResolution)
      {
      dataItem->contourActor->VisibilityOff();
      }
    else
      {
      dataItem->lowContourActor->VisibilityOff();
      }
    }

  /* Render the scene */
  dataItem->externalVTKWidget->GetRenderWindow()->Render();
  if (this->Rotations == ROTATIONS_MAX - 1)
    {
    vtkSmartPointer<vtkSmartVolumeMapper> mapperVolume =
      vtkSmartVolumeMapper::SafeDownCast(dataItem->actorVolume->GetMapper());
    std::cout << "Last used render mode: " << mapperVolume->GetLastUsedRenderMode() << std::endl;
    }

  clippingPlaneIndex = 0;
  for (int i = 0; i < NumberOfClippingPlanes &&
    clippingPlaneIndex < numberOfSupportedClippingPlanes; ++i)
    {
    if (ClippingPlanes[i].isActive())
      {
      /* Disable the clipping plane: */
      glDisable(GL_CLIP_PLANE0 + clippingPlaneIndex);
      /* Go to the next clipping plane: */
      ++clippingPlaneIndex;
      }
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::centerDisplayCallback(Misc::CallbackData* callBackData)
{
  if(!this->DataBounds)
    {
    std::cerr << "ERROR: Data bounds not set!!" << std::endl;
    return;
    }
  Vrui::setNavigationTransformation(this->Center, this->Radius);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::opacitySliderCallback(
  GLMotif::Slider::ValueChangedCallbackData* callBackData)
{
  this->Opacity = static_cast<double>(callBackData->value);
  opacityValue->setValue(callBackData->value);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeSamplingCallback(
  GLMotif::Slider::ValueChangedCallbackData* callBackData)
{
  this->sampling = static_cast<int>(callBackData->value);
  resolutionValue->setValue(callBackData->value);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeRepresentationCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* Adjust representation state based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowSurface") == 0)
    {
    this->RepresentationType = 2;
    this->Volume = false;
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowSurfaceWithEdges") == 0)
    {
    this->RepresentationType = 3;
    this->Volume = false;
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowWireframe") == 0)
    {
    this->RepresentationType = 1;
    this->Volume = false;
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowPoints") == 0)
    {
    this->RepresentationType = 0;
    this->Volume = false;
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowNone") == 0)
    {
    this->RepresentationType = -1;
    this->Volume = false;
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowVolume") == 0)
    {
    this->Volume = callBackData->set;
    if (this->Volume)
      {
      this->RepresentationType = -1;
      }
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowOutline") == 0)
    {
    this->Outline = callBackData->set;
    }
}
//----------------------------------------------------------------------------
void ExampleVTKReader::changeAnalysisToolsCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* Set the new analysis tool: */
  if (strcmp(callBackData->toggle->getName(), "ClippingPlane") == 0)
    {
    this->analysisTool = 0;
    }
  else if (strcmp(callBackData->toggle->getName(), "Flashlight") == 0)
    {
    this->analysisTool = 1;
    }
  else if (strcmp(callBackData->toggle->getName(), "FreeSlice") == 0)
    {
    this->analysisTool = 2;
    }
  else if (strcmp(callBackData->toggle->getName(), "Other") == 0)
    {
    this->analysisTool = 3;
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showSlicesDialogCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* open/close slices dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowSlicesDialog") == 0)
    {
    if (callBackData->set)
      {
      /* Open the slices dialog at the same position as the main menu: */
      Vrui::getWidgetManager()->popupPrimaryWidget(slicesDialog,
        Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
      }
    else
      {
      /* Close the slices dialog: */
      Vrui::popdownPrimaryWidget(slicesDialog);
      }
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showIsosurfacesDialogCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
    /* open/close isosurfaces dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowIsosurfacesDialog") == 0) {
    if (callBackData->set) {
      /* Open the isosurfaces dialog at the same position as the main menu: */
      Vrui::getWidgetManager()->popupPrimaryWidget(
        isosurfacesDialog, Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
    } else {
      /* Close the isosurfaces dialog: */
      Vrui::popdownPrimaryWidget(isosurfacesDialog);
    }
  }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showContoursDialogCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* open/close slices dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowContoursDialog") == 0)
    {
    if (callBackData->set)
      {
      /* Open the slices dialog at the same position as the main menu: */
      Vrui::getWidgetManager()->popupPrimaryWidget(ContoursDialog,
        Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
      }
    else
      {
      /* Close the slices dialog: */
      Vrui::popdownPrimaryWidget(ContoursDialog);
      }
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showTransferFunctionDialogCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* open/close transfer function dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowTransferFunctionDialog") == 0)
    {
    if (callBackData->set)
      {
      /* Open the transfer function dialog at the same position as the main menu: */
      Vrui::getWidgetManager()->popupPrimaryWidget(transferFunctionDialog,
        Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
      }
    else
      {
      /* Close the transfer function dialog: */
      Vrui::popdownPrimaryWidget(transferFunctionDialog);
    }
  }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showRenderingDialogCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* open/close rendering dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowRenderingDialog") == 0)
    {
    if (callBackData->set)
      {
      /* Open the rendering dialog at the same position as the main menu: */
      Vrui::getWidgetManager()->popupPrimaryWidget(renderingDialog,
        Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
      }
    else
      {
      /* Close the rendering dialog: */
      Vrui::popdownPrimaryWidget(renderingDialog);
      }
    }
}

//----------------------------------------------------------------------------
ClippingPlane * ExampleVTKReader::getClippingPlanes(void)
{
  return this->ClippingPlanes;
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getNumberOfClippingPlanes(void)
{
  return this->NumberOfClippingPlanes;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::toolCreationCallback(
  Vrui::ToolManager::ToolCreationCallbackData * callbackData)
{
  /* Check if the new tool is a locator tool: */
  Vrui::LocatorTool* locatorTool = dynamic_cast<Vrui::LocatorTool*> (
    callbackData->tool);
  if (locatorTool != 0)
    {
    BaseLocator* newLocator;
    if (analysisTool == 0)
      {
      /* Create a clipping plane locator object and
       * associate it with the new tool: */
      newLocator = new ClippingPlaneLocator(locatorTool, this);
      }
    else if (analysisTool == 1)
      {
      /* Create a flashlight locator object and
       * associate it with the new tool: */
      newLocator = new FlashlightLocator(locatorTool, this);
      }
    else if (analysisTool == 2)
      {
      /* Create a freeSlice locator object and
       * associate it with the new tool: */
      newLocator = new FreeSliceLocator(locatorTool, this);
      }

      /* Add new locator to list: */
      baseLocators.push_back(newLocator);
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::toolDestructionCallback(
  Vrui::ToolManager::ToolDestructionCallbackData * callbackData)
{
  /* Check if the to-be-destroyed tool is a locator tool: */
  Vrui::LocatorTool* locatorTool = dynamic_cast<Vrui::LocatorTool*> (
    callbackData->tool);
  if (locatorTool != 0)
    {
    /* Find the data locator associated with the tool in the list: */
    for (BaseLocatorList::iterator blIt = baseLocators.begin();
      blIt != baseLocators.end(); ++blIt)
      {
      if ((*blIt)->getTool() == locatorTool)
        {
        /* Remove the locator: */
        delete *blIt;
        baseLocators.erase(blIt);
        break;
        }
      }
    }
}

//----------------------------------------------------------------------------
int * ExampleVTKReader::getFlashlightSwitch(void)
{
  return this->FlashlightSwitch;
}

//----------------------------------------------------------------------------
double * ExampleVTKReader::getFlashlightPosition(void)
{
  return this->FlashlightPosition;
}

//----------------------------------------------------------------------------
double * ExampleVTKReader::getFlashlightDirection(void)
{
  return this->FlashlightDirection;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setAIsosurface(float aIsosurface)
{
  this->aIsosurface = aIsosurface;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setBIsosurface(float bIsosurface)
{
  this->bIsosurface = bIsosurface;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setCIsosurface(float cIsosurface)
{
  this->cIsosurface = cIsosurface;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showAIsosurface(bool AIsosurface)
{
  this->AIsosurface = AIsosurface;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showBIsosurface(bool BIsosurface)
{
  this->BIsosurface = BIsosurface;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showCIsosurface(bool CIsosurface)
{
  this->CIsosurface = CIsosurface;
}

//----------------------------------------------------------------------------
int * ExampleVTKReader::getFreeSliceVisibility(void)
{
  return this->FreeSliceVisibility;
}

//----------------------------------------------------------------------------
double * ExampleVTKReader::getFreeSliceOrigin(void)
{
  return this->FreeSliceOrigin;
}

//----------------------------------------------------------------------------
double * ExampleVTKReader::getFreeSliceNormal(void)
{
  return this->FreeSliceNormal;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setXSlice(int xSlice)
{
  this->xSlice = xSlice;
  this->xPlane->SetOrigin(this->xOrigin +
    (this->xSlice * this->DataSpacing[0]), this->yCenter, this->zCenter);

}

//----------------------------------------------------------------------------
void ExampleVTKReader::setYSlice(int ySlice)
{
  this->ySlice = ySlice;
  this->yPlane->SetOrigin(this->xCenter, this->yOrigin +
    (this->ySlice * this->DataSpacing[1]), this->zCenter);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setZSlice(int zSlice)
{
  this->zSlice = zSlice;
  this->zPlane->SetOrigin(this->xCenter, this->yCenter, this->zOrigin +
    (this->zSlice * this->DataSpacing[2]));
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showXSlice(bool XSlice)
{
  this->XSlice = XSlice;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showYSlice(bool YSlice)
{
  this->YSlice = YSlice;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showZSlice(bool ZSlice)
{
  this->ZSlice = ZSlice;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setXContourSlice(int xSlice)
{
  this->xContourSlice = xSlice;
  this->xContourPlane->SetOrigin(this->xOrigin +
    (this->xContourSlice * this->DataSpacing[0]), this->yCenter, this->zCenter);

}

//----------------------------------------------------------------------------
void ExampleVTKReader::setYContourSlice(int ySlice)
{
  this->yContourSlice = ySlice;
  this->yContourPlane->SetOrigin(this->xCenter, this->yOrigin +
    (this->yContourSlice * this->DataSpacing[1]), this->zCenter);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setZContourSlice(int zSlice)
{
  this->zContourSlice = zSlice;
  this->zContourPlane->SetOrigin(this->xCenter, this->yCenter, this->zOrigin +
    (this->zContourSlice * this->DataSpacing[2]));
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showXContourSlice(bool XSlice)
{
  this->XContourSlice = XSlice;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showYContourSlice(bool YSlice)
{
  this->YContourSlice = YSlice;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showZContourSlice(bool ZSlice)
{
  this->ZContourSlice = ZSlice;
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getWidth(void)
{
  return this->DataDimensions[0];
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getLength(void)
{
  return this->DataDimensions[1];
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getHeight(void)
{
  return this->DataDimensions[2];
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateIsosurfaceColorMap(double* IsosurfaceColormap)
{
  this->IsosurfaceColormap = IsosurfaceColormap;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateSliceColorMap(double* SliceColormap)
{
  this->SliceColormap = SliceColormap;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::alphaChangedCallback(Misc::CallbackData* callBackData)
{
  transferFunctionDialog->exportAlpha(this->VolumeColormap);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::contourValueChangedCallback(Misc::CallbackData* callBackData)
{
  this->ContourValues = ContoursDialog->getContourValues();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::volumeColorMapChangedCallback(
  Misc::CallbackData* callBackData)
{
  transferFunctionDialog->exportColorMap(this->VolumeColormap);
  this->updateAlpha();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateAlpha(void)
{
  transferFunctionDialog->exportAlpha(this->VolumeColormap);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateVolumeColorMap(void)
{
  transferFunctionDialog->exportColorMap(this->VolumeColormap);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeAlphaCallback(
  GLMotif::RadioBox::ValueChangedCallbackData* callBackData)
{
  int value = callBackData->radioBox->getToggleIndex(
    callBackData->newSelectedToggle);
  transferFunctionDialog->changeAlpha(value);
  updateAlpha();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeColorMapCallback(
  GLMotif::RadioBox::ValueChangedCallbackData* callBackData)
{
  int value = callBackData->radioBox->getToggleIndex(
    callBackData->newSelectedToggle);
  transferFunctionDialog->changeColorMap(value);
  updateVolumeColorMap();
  updateAlpha();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeResolutionCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  /* open/close rendering dialog based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowLowResolution") == 0)
    {
    if (callBackData->set)
      {
        lowResolution = true;
      }
    else
      {
        lowResolution = false;
      }
    }
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMinimum(void)
{
  return float(this->DataScalarRange[0]);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMaximum(void)
{
  return float(this->DataScalarRange[1]);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataIncrement(void)
{
  return float((this->DataScalarRange[1]-this->DataScalarRange[0])/20.0);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMidPoint(void)
{
  return float((this->DataScalarRange[1]-this->DataScalarRange[0])/2.0);
}

//----------------------------------------------------------------------------
std::vector<double> ExampleVTKReader::getContourValues(void)
{
  return this->ContourValues;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setContourVisible(bool visible)
{
  this->ContourVisible = visible;
}

//----------------------------------------------------------------------------
float * ExampleVTKReader::getHistogram(void)
{
  return this->Histogram;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setRequestedRenderMode(int mode)
{
  this->RequestedRenderMode = mode;
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getRequestedRenderMode(void) const
{
  return this->RequestedRenderMode;
}
