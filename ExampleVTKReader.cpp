// STD includes
#include <iostream>
#include <string>
#include <math.h>

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

// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCutter.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkImageData.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkLight.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkOutlineFilter.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPlane.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkXMLImageDataReader.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkPointData.h>

// ExampleVTKReader includes
#include "BaseLocator.h"
#include "ClippingPlane.h"
#include "ClippingPlaneLocator.h"
#include "ColorMap.h"
#include "Contours.h"
#include "ExampleVTKReader.h"
#include "FlashlightLocator.h"
#include "ScalarWidget.h"
#include "Slices.h"
#include "TransferFunction1D.h"


//----------------------------------------------------------------------------
ExampleVTKReader::DataItem::DataItem(void)
{
  /* Initialize VTK renderwindow and renderer */
  this->externalVTKWidget = vtkSmartPointer<ExternalVTKWidget>::New();
  this->actor = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actor);
  this->actorOutline = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorOutline);
  this->actorVolume = vtkSmartPointer<vtkVolume>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorVolume);
  this->propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
  this->xCutter = vtkSmartPointer<vtkCutter>::New();
  this->xCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorXCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorXCutter);
  this->yCutter = vtkSmartPointer<vtkCutter>::New();
  this->yCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorYCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorYCutter);
  this->zCutter = vtkSmartPointer<vtkCutter>::New();
  this->zCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorZCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorZCutter);

  this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->contourActor = vtkSmartPointer<vtkActor>::New();
  this->actorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->actorXContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(
    this->actorXContourCutter);
  this->actorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->actorYContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(
    this->actorYContourCutter);
  this->actorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->actorZContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(
    this->actorZContourCutter);

  this->flashlight = vtkSmartPointer<vtkLight>::New();
  this->externalVTKWidget->GetRenderer()->AddLight(this->flashlight);
}

//----------------------------------------------------------------------------
ExampleVTKReader::DataItem::~DataItem(void)
{
}

//----------------------------------------------------------------------------
ExampleVTKReader::ExampleVTKReader(int& argc,char**& argv)
  :Vrui::Application(argc,argv),
  analysisTool(0),
  ClippingPlanes(NULL),
  ContoursDialog(NULL),
  ContourVisible(true),
//  ContourValues(0),
//  NumberOfContourValues(0),
  FileName(0),
  FirstFrame(true),
  FlashlightDirection(0),
  FlashlightPosition(0),
  FlashlightSwitch(0),
  mainMenu(NULL),
  NumberOfClippingPlanes(6),
  Opacity(1.0),
  opacityValue(NULL),
  Outline(true),
  renderingDialog(NULL),
  RepresentationType(2),
  slicesDialog(NULL),
  transferFunctionDialog(NULL),
  Verbose(false),
  Volume(false),
  xCenter(0),
  xOrigin(0),
  xSlice(0),
  XSlice(false),
  xContourSlice(0),
  XContourSlice(false),
  yCenter(0),
  yOrigin(0),
  ySlice(0),
  YSlice(false),
  yContourSlice(0),
  YContourSlice(false),
  zCenter(0),
  zOrigin(0),
  zSlice(0),
  ZSlice(false),
  zContourSlice(0),
  ZContourSlice(false)
{

  this->modelLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->modelLUT->SetNumberOfColors(256);
  this->modelLUT->Build();

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

  this->VolumeColormap = new double[4*256];
//  this->ContourValues = new std::vector<double>();

  this->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
  this->opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();

  this->SliceColormap = new double[4*256];

  this->sliceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->sliceLUT->SetNumberOfColors(256);
  this->sliceLUT->Build();

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


  GLMotif::Button* centerDisplayButton = new GLMotif::Button(
    "CenterDisplayButton",mainMenu,"Center Display");
  centerDisplayButton->getSelectCallbacks().add(
    this,&ExampleVTKReader::centerDisplayCallback);

  GLMotif::ToggleButton * showSlicesDialog = new GLMotif::ToggleButton(
    "ShowSlicesDialog", mainMenu, "Slices");
  showSlicesDialog->setToggle(false);
  showSlicesDialog->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::showSlicesDialogCallback);

  GLMotif::ToggleButton * showContoursDialog = new GLMotif::ToggleButton(
    "ShowContoursDialog", mainMenu, "Contours");
  showContoursDialog->setToggle(false);
  showContoursDialog->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::showContoursDialogCallback);

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
  GLMotif::ToggleButton* showFlashlight=new GLMotif::ToggleButton(
    "Flashlight",analysisTools_RadioBox,"Flashlight");
  showFlashlight->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeAnalysisToolsCallback);
  GLMotif::ToggleButton* showOther=new GLMotif::ToggleButton(
    "Other",analysisTools_RadioBox,"Other");
  showOther->getValueChangedCallbacks().add(
    this,&ExampleVTKReader::changeAnalysisToolsCallback);

  analysisTools_RadioBox->setSelectionMode(GLMotif::RadioBox::ALWAYS_ONE);
  analysisTools_RadioBox->setSelectedToggle(showClippingPlane);

  analysisToolsMenu->manageChild();
  return analysisToolsMenuPopup;
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
  dialog->setOrientation(GLMotif::RowColumn::HORIZONTAL);

  /* Create opacity slider */
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
    updateAlpha();
    updateVolumeColorMap();

    this->slicesDialog = new Slices(this->SliceColormap, this);
    this->slicesDialog->setSlicesColorMap(CINVERSE_RAINBOW, 0.0, 1.0);
    slicesDialog->exportSlicesColorMap(this->SliceColormap);
    updateSliceColorMap(this->SliceColormap);

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
    this->FirstFrame = false;
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::initContext(GLContextData& contextData) const
{
  /* Create a new context data item */
  DataItem* dataItem = new DataItem();
  contextData.addDataItem(this, dataItem);

  vtkNew<vtkDataSetMapper> mapper;
  dataItem->actor->SetMapper(mapper.GetPointer());

  vtkNew<vtkOutlineFilter> dataOutline;

  vtkNew<vtkSmartVolumeMapper> mapperVolume;

  vtkSmartPointer<vtkUnsignedCharArray> scalars;
  if(this->FileName)
    {
    vtkNew<vtkXMLImageDataReader> reader;
    reader->SetFileName(this->FileName);
    reader->Update();

    mapper->SetInputConnection(reader->GetOutputPort());
    reader->GetOutput()->GetDimensions(this->DataDimensions);
    reader->GetOutput()->GetBounds(this->DataBounds);
    reader->GetOutput()->GetOrigin(this->DataOrigin);
    reader->GetOutput()->GetExtent(this->DataExtent);
    reader->GetOutput()->GetSpacing(this->DataSpacing);
    reader->GetOutput()->GetScalarRange(this->DataScalarRange);

    mapper->SetInputConnection(reader->GetOutputPort());

    dataOutline->SetInputConnection(reader->GetOutputPort());

    mapperVolume->SetInputConnection(reader->GetOutputPort());

    dataItem->xCutter->SetInputConnection(reader->GetOutputPort());

    dataItem->yCutter->SetInputConnection(reader->GetOutputPort());

    dataItem->zCutter->SetInputConnection(reader->GetOutputPort());

    dataItem->contourFilter->SetInputConnection(reader->GetOutputPort());

    scalars = vtkUnsignedCharArray::SafeDownCast(
      reader->GetOutput()->GetPointData()->GetScalars());
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
//    for (int i = 0; i < this->DataDimensions[0]; ++i)
//      {
//      for (int j = 0; j < this->DataDimensions[1]; ++j)
//        {
//        for (int k = 0; k < this->DataDimensions[2]; ++k)
//          {
//          unsigned char * pixel = static_cast<unsigned char *>(
//            imageData->GetScalarPointer(i,j,k));
//          pixel[0] = 255.0;
//          }
//        }
//      }
    imageData->GetBounds(this->DataBounds);
    imageData->GetScalarRange(this->DataScalarRange);
    imageData->GetExtent(this->DataExtent);

    mapper->SetInputData(imageData.GetPointer());

    dataOutline->SetInputData(imageData.GetPointer());

    mapperVolume->SetInputData(imageData.GetPointer());

    dataItem->xCutter->SetInputData(imageData.GetPointer());

    dataItem->yCutter->SetInputData(imageData.GetPointer());

    dataItem->zCutter->SetInputData(imageData.GetPointer());

    dataItem->contourFilter->SetInputData(imageData.GetPointer());

    scalars = vtkUnsignedCharArray::SafeDownCast(
      imageData->GetPointData()->GetScalars());
    }

  mapper->SetScalarRange(this->DataScalarRange);
  mapper->SetLookupTable(this->modelLUT);
  mapper->SetColorModeToMapScalars();

  vtkNew<vtkPolyDataMapper> mapperOutline;
  mapperOutline->SetInputConnection(dataOutline->GetOutputPort());
  dataItem->actorOutline->SetMapper(mapperOutline.GetPointer());
  dataItem->actorOutline->GetProperty()->SetColor(1,1,1);

  mapperVolume->SetInteractiveUpdateRate(12.0);
  mapperVolume->SetRequestedRenderMode(3);
  mapperVolume->SetBlendModeToComposite();
  this->colorFunction->AddRGBPoint(this->DataScalarRange[0], 1.0, 1.0, 1.0);
  this->colorFunction->AddRGBPoint(this->DataScalarRange[1], 0.0, 0.0, 0.0);
  this->opacityFunction->AddPoint(this->DataScalarRange[0], 0.0);
  this->opacityFunction->AddPoint(this->DataScalarRange[1], 1.0);
  dataItem->propertyVolume->ShadeOff();
  dataItem->propertyVolume->SetScalarOpacityUnitDistance(1.0);
  dataItem->propertyVolume->SetColor(this->colorFunction);
  dataItem->propertyVolume->SetScalarOpacity(this->opacityFunction);
  dataItem->propertyVolume->SetInterpolationTypeToLinear();
  dataItem->actorVolume->SetProperty(dataItem->propertyVolume);
  dataItem->actorVolume->SetMapper(mapperVolume.GetPointer());

  dataItem->xCutter->SetCutFunction(this->xPlane);
  dataItem->xCutterMapper->SetInputConnection(dataItem->xCutter->GetOutputPort());
  dataItem->xCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->xCutterMapper->SetLookupTable(this->sliceLUT);
  dataItem->xCutterMapper->SetColorModeToMapScalars();
  dataItem->actorXCutter->SetMapper(dataItem->xCutterMapper);

  dataItem->yCutter->SetCutFunction(this->yPlane);
  dataItem->yCutterMapper->SetInputConnection(dataItem->yCutter->GetOutputPort());
  dataItem->yCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->yCutterMapper->SetLookupTable(this->sliceLUT);
  dataItem->yCutterMapper->SetColorModeToMapScalars();
  dataItem->actorYCutter->SetMapper(dataItem->yCutterMapper);

  dataItem->zCutter->SetCutFunction(this->zPlane);
  dataItem->zCutterMapper->SetInputConnection(dataItem->zCutter->GetOutputPort());
  dataItem->zCutterMapper->SetScalarRange(this->DataScalarRange);
  dataItem->zCutterMapper->SetLookupTable(this->sliceLUT);
  dataItem->zCutterMapper->SetColorModeToMapScalars();
  dataItem->actorZCutter->SetMapper(dataItem->zCutterMapper);

  vtkNew<vtkPolyDataMapper> contourMapper;
  contourMapper->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  contourMapper->ScalarVisibilityOff();
  dataItem->contourActor->SetMapper(contourMapper.GetPointer());

  vtkNew<vtkCutter> xContourCutter;
  xContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  xContourCutter->SetCutFunction(this->xContourPlane);
  vtkNew<vtkPolyDataMapper> mapperXContourCutter;
  mapperXContourCutter->SetInputConnection(xContourCutter->GetOutputPort());
  mapperXContourCutter->ScalarVisibilityOff();
  dataItem->actorXContourCutter->SetMapper(mapperXContourCutter.GetPointer());

  vtkNew<vtkCutter> yContourCutter;
  yContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  yContourCutter->SetCutFunction(this->yContourPlane);
  vtkNew<vtkPolyDataMapper> mapperYContourCutter;
  mapperYContourCutter->SetInputConnection(yContourCutter->GetOutputPort());
  mapperYContourCutter->ScalarVisibilityOff();
  dataItem->actorYContourCutter->SetMapper(mapperYContourCutter.GetPointer());

  vtkNew<vtkCutter> zContourCutter;
  zContourCutter->SetInputConnection(dataItem->contourFilter->GetOutputPort());
  zContourCutter->SetCutFunction(this->zContourPlane);
  vtkNew<vtkPolyDataMapper> mapperZContourCutter;
  mapperZContourCutter->SetInputConnection(zContourCutter->GetOutputPort());
  mapperZContourCutter->ScalarVisibilityOff();
  dataItem->actorZContourCutter->SetMapper(mapperZContourCutter.GetPointer());

  dataItem->flashlight->SwitchOff();
  dataItem->flashlight->SetLightTypeToHeadlight();
  dataItem->flashlight->SetColor(0.0, 1.0, 1.0);
  dataItem->flashlight->SetConeAngle(15);
  dataItem->flashlight->SetPositional(true);

  for(int i = 0; i < scalars->GetNumberOfTuples(); ++i)
    {
    this->Histogram[static_cast<int>(scalars->GetTuple1(i))] += 1;
    }
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
  /* Save OpenGL state: */
  glPushAttrib(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_ENABLE_BIT|
    GL_LIGHTING_BIT|GL_POLYGON_BIT);
  /* Get context data item */
  DataItem* dataItem = contextData.retrieveDataItem<DataItem>(this);

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

  if (this->Outline)
    {
    dataItem->actorOutline->VisibilityOn();
    }
  else
    {
    dataItem->actorOutline->VisibilityOff();
    }

  if (this->Volume)
    {
    dataItem->actorVolume->VisibilityOn();
    dataItem->actor->VisibilityOff();
    }
  else
    {
    dataItem->actorVolume->VisibilityOff();
    dataItem->actor->GetProperty()->SetOpacity(this->Opacity);
    if (this->RepresentationType != -1)
      {
      dataItem->actor->VisibilityOn();
      if (this->RepresentationType == 3)
        {
        dataItem->actor->GetProperty()->SetRepresentationToSurface();
        dataItem->actor->GetProperty()->EdgeVisibilityOn();
        }
      else
        {
        dataItem->actor->GetProperty()->SetRepresentation(this->RepresentationType);
        dataItem->actor->GetProperty()->EdgeVisibilityOff();
        }
      }
    else
      {
      dataItem->actor->VisibilityOff();
      }
    }

  if (this->XSlice)
    {
    dataItem->actorXCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorXCutter->VisibilityOff();
    }

  if (this->YSlice)
    {
    dataItem->actorYCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorYCutter->VisibilityOff();
    }

  if (this->ZSlice)
    {
    dataItem->actorZCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorZCutter->VisibilityOff();
    }

  if (this->XContourSlice)
    {
    dataItem->actorXContourCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorXContourCutter->VisibilityOff();
    }

  if (this->YContourSlice)
    {
    dataItem->actorYContourCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorYContourCutter->VisibilityOff();
    }

  if (this->ZContourSlice)
    {
    dataItem->actorZContourCutter->VisibilityOn();
    }
  else
    {
    dataItem->actorZContourCutter->VisibilityOff();
    }

  dataItem->contourFilter->SetNumberOfContours(this->ContourValues.size());
  for(int i = 0; i < this->ContourValues.size(); ++i)
    {
    dataItem->contourFilter->SetValue(i, this->ContourValues.at(i));
    }
  if(this->ContourVisible)
    {
    dataItem->externalVTKWidget->GetRenderer()->AddActor(dataItem->contourActor);
    }
  else
    {
    dataItem->externalVTKWidget->GetRenderer()->RemoveActor(dataItem->contourActor);
    }

  /* Render the scene */
  dataItem->externalVTKWidget->GetRenderWindow()->Render();

  glPopAttrib();

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
  else if (strcmp(callBackData->toggle->getName(), "Other") == 0)
    {
    this->analysisTool = 2;
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
void ExampleVTKReader::updateSliceColorMap(double* SliceColormap)
{
  this->SliceColormap = SliceColormap;
  for (int i=0;i<256;i++)
    {
    this->sliceLUT->SetTableValue(i, this->SliceColormap[4*i + 0],
      this->SliceColormap[4*i + 1],
      this->SliceColormap[4*i + 2], 1.0);
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::alphaChangedCallback(Misc::CallbackData* callBackData)
{
  transferFunctionDialog->exportAlpha(this->VolumeColormap);
  this->opacityFunction->RemoveAllPoints();
  double step = (this->DataScalarRange[1] - this->DataScalarRange[0])/255.0;
  for (int i = 0; i < 256; i++)
    {
    this->opacityFunction->AddPoint(this->DataScalarRange[0] +
      (double)(i*step), this->VolumeColormap[4*i + 3]);
    }
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
  this->colorFunction->RemoveAllPoints();
  double step = (this->DataScalarRange[1] - this->DataScalarRange[0])/255.0;
  for (int i = 0; i < 256; i++)
    {
    this->colorFunction->AddRGBPoint(this->DataScalarRange[0] +
      (double)(i*step), this->VolumeColormap[4*i + 0],
      this->VolumeColormap[4*i + 1], this->VolumeColormap[4*i + 2]);
    }
  updateModelColorMap();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateAlpha(void)
{
  transferFunctionDialog->exportAlpha(this->VolumeColormap);
  this->opacityFunction->RemoveAllPoints();
  double step = (this->DataScalarRange[1] - this->DataScalarRange[0])/255.0;
  for (int i = 0; i < 256; i++)
    {
    this->opacityFunction->AddPoint(this->DataScalarRange[0] +
      (double)(i*step), this->VolumeColormap[4*i + 3]);
    }
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateVolumeColorMap(void)
{
  transferFunctionDialog->exportColorMap(this->VolumeColormap);
  this->colorFunction->RemoveAllPoints();
  double step = (this->DataScalarRange[1] - this->DataScalarRange[0])/255.0;
  for (int i = 0; i < 256; i++)
    {
    this->colorFunction->AddRGBPoint(this->DataScalarRange[0] +
      (double)(i*step), this->VolumeColormap[4*i + 0],
      this->VolumeColormap[4*i + 1], this->VolumeColormap[4*i + 2]);
    }
  updateModelColorMap();
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
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateModelColorMap(void)
{
  for (int i=0;i<256;i++)
    {
    this->modelLUT->SetTableValue(i, this->VolumeColormap[4*i + 0],
      this->VolumeColormap[4*i + 1],
      this->VolumeColormap[4*i + 2], 1.0);
    }
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
