// STD includes
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

// Must come before any gl.h include
#include <GL/glew.h>

// VTK includes
#include <vtkImageData.h>

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

// vtkVRUI includes
#include "vvProgress.h"

// ExampleVTKReader includes
#include "BaseLocator.h"
#include "ClippingPlane.h"
#include "ClippingPlaneLocator.h"
#include "ColorMap.h"
#include "Contours.h"
#include "ExampleVTKReader.h"
#include "Isosurfaces.h"
#include "FreeSliceLocator.h"
#include "ScalarWidget.h"
#include "Slices.h"
#include "TransferFunction1D.h"
#include "volApplicationState.h"
#include "volContextState.h"
#include "volContours.h"
#include "volFreeSlice.h"
#include "volGeometry.h"
#include "volIsosurface.h"
#include "volOutline.h"
#include "volReader.h"
#include "volSlices.h"
#include "volVolume.h"

//----------------------------------------------------------------------------
ExampleVTKReader::ExampleVTKReader(int& argc,char**& argv)
  : Superclass(argc, argv, new volApplicationState),
    m_volState(*static_cast<volApplicationState*>(m_state)),
    ClippingPlanes(NULL),
    ContoursDialog(NULL),
    FileName(0),
    FirstFrame(true),
    mainMenu(NULL),
    NumberOfClippingPlanes(6),
    opacityValue(NULL),
    renderingDialog(NULL),
    resolutionValue(NULL),
    slicesDialog(NULL),
    transferFunctionDialog(NULL),
    Verbose(false)
{
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
}

//----------------------------------------------------------------------------
ExampleVTKReader::~ExampleVTKReader(void)
{
  if(this->Histogram)
    {
    delete[] this->Histogram;
    }
}

//----------------------------------------------------------------------------
void ExampleVTKReader::initialize(void)
{
  this->Superclass::initialize();

  // Start async file read.
  m_volState.reader().setFileName(this->FileName);
  m_volState.reader().update(m_volState);

  /* Create the user interface: */
  renderingDialog = createRenderingDialog();
  mainMenu = createMainMenu();
  Vrui::setMainMenu(mainMenu);

  // TODO This is ugly, it'd be great to find a way around this.
  // Force sync reader output:
  while (m_volState.reader().running(std::chrono::seconds(1)))
    {
    std::cout << "Waiting for initial file read to complete..." << std::endl;
    }

  // Update cached data object, start reducer
  m_volState.reader().update(m_volState);

  // Wait for reduced data, too
  while (m_volState.reader().reducing(std::chrono::seconds(1)))
    {
    std::cout << "Waiting for initial file read to complete..." << std::endl;
    }
  m_volState.reader().update(m_volState); // Update cached data object
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setFileName(const char* name)
{
  if (this->FileName && name && (!strcmp(this->FileName, name)))
    {
    return;
    }
  if (this->FileName)
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
void ExampleVTKReader::setProgressVisibility(bool vis)
{
  m_volState.progress().setVisible(vis);
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
  showLowResolution->setToggle(m_volState.forceLowResolution());
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
  opacitySlider->setValue(m_volState.geometry().opacity());
  opacitySlider->setValueRange(0.0, 1.0, 0.1);
  opacitySlider->getValueChangedCallbacks().add(this,
    &ExampleVTKReader::opacitySliderCallback);
  opacityValue = new GLMotif::TextField("OpacityValue", dialog, 6);
  opacityValue->setFieldWidth(6);
  opacityValue->setPrecision(3);
  opacityValue->setValue(m_volState.geometry().opacity());

  /* Create sampling slider */
  GLMotif::Label* sampling_Label = new GLMotif::Label("Sampling", dialog,"Sampling:");
  GLMotif::Slider * resolutionSlider = new GLMotif::Slider("ResolutionSlider", dialog, GLMotif::Slider::HORIZONTAL, ss.fontHeight * 10.0f);
  resolutionSlider->setValueRange(1.0, 8.0, 1.0);
  resolutionSlider->setValue(static_cast<float>(
                               m_volState.reader().sampleRate()));
  resolutionSlider->getValueChangedCallbacks().add(this, &ExampleVTKReader::changeSamplingCallback);
  resolutionValue = new GLMotif::TextField("ResolutionValue", dialog, 6);
  resolutionValue->setFieldWidth(6);
  resolutionValue->setPrecision(1);
  resolutionValue->setValue(static_cast<float>(
                              m_volState.reader().sampleRate()));
  dialog->manageChild();

  return dialogPopup;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::frame(void)
{
  m_volState.reader().update(m_volState);

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

    this->slicesDialog = new Slices(m_volState.sliceColorMap().data(), this);
    this->slicesDialog->setSlicesColorMap(CINVERSE_RAINBOW, 0.0, 1.0);
    this->slicesDialog->exportSlicesColorMap(m_volState.sliceColorMap().data());
    this->updateSliceColorMap(m_volState.sliceColorMap().data());
    m_volState.sliceColorMapModified();

    this->isosurfacesDialog = new Isosurfaces(
          m_volState.isosurfaceColorMap().data(), this);
    this->isosurfacesDialog->setIsosurfacesColorMap(CINVERSE_RAINBOW, 0.0, 1.0);
    this->isosurfacesDialog->exportIsosurfacesColorMap(
          m_volState.isosurfaceColorMap().data());
    updateIsosurfaceColorMap(m_volState.isosurfaceColorMap().data());
    m_volState.isosurfaceColorMapModified();

    this->ContoursDialog = new Contours(this);
    this->ContoursDialog->getAlphaChangedCallbacks().add(this,
      &ExampleVTKReader::contourValueChangedCallback);

    /* Initialize Vrui navigation transformation: */
    centerDisplayCallback(0);

    double midPoint = static_cast<double>(this->getDataMidPoint());
    m_volState.isosurfaceA().setContourValue(midPoint);
    m_volState.isosurfaceB().setContourValue(midPoint);
    m_volState.isosurfaceC().setContourValue(midPoint);
    this->FirstFrame = false;
    }

  this->Superclass::frame();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::initContext(GLContextData& contextData) const
{
  this->Superclass::initContext(contextData);

  // Created by superclass:
  volContextState *context =
      contextData.retrieveDataItem<volContextState>(this);
  assert("volContextState initialized by vvApplication." && context);

  std::array<int, 3> dims = m_volState.reader().dimensions();

  vtkImageData *imageData = m_volState.reader().typedDataObject();
  for (int i = 0; i < dims[0]; ++i)
    {
    for (int j = 0; j < dims[1]; ++j)
      {
      for (int k = 0; k < dims[2]; ++k)
        {
        unsigned char *pixel =
            static_cast<unsigned char *>(imageData->GetScalarPointer(i,j,k));
        this->Histogram[static_cast<int>(pixel[0])] += 1;
        }
      }
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

  this->Superclass::display(contextData);

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
void ExampleVTKReader::centerDisplayCallback(Misc::CallbackData*)
{
  if (!m_volState.reader().bounds().IsValid())
    {
    std::cerr << "ERROR: Data bounds not set!!" << std::endl;
    return;
    }

  std::array<double, 3> center;
  m_volState.reader().bounds().GetCenter(center.data());
  double radius = m_volState.reader().bounds().GetDiagonalLength();
  Vrui::setNavigationTransformation(center.data(), radius * 0.75);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::opacitySliderCallback(
  GLMotif::Slider::ValueChangedCallbackData* callBackData)
{
  m_volState.geometry().setOpacity(callBackData->value);
  opacityValue->setValue(callBackData->value);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeSamplingCallback(
  GLMotif::Slider::ValueChangedCallbackData* callBackData)
{
  int sampling = static_cast<int>(callBackData->value);
  resolutionValue->setValue(callBackData->value);

  m_volState.reader().setSampleRate(sampling);
}

//----------------------------------------------------------------------------
void ExampleVTKReader::changeRepresentationCallback(
  GLMotif::ToggleButton::ValueChangedCallbackData* callBackData)
{
  using Representation = volGeometry::Representation;
  /* Adjust representation state based on which toggle button changed state: */
  if (strcmp(callBackData->toggle->getName(), "ShowSurface") == 0)
    {
    m_volState.geometry().setVisible(true);
    m_volState.geometry().setRepresentation(Representation::Surface);
    m_volState.volume().setVisible(false);
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowSurfaceWithEdges") == 0)
    {
    m_volState.geometry().setVisible(true);
    m_volState.geometry().setRepresentation(Representation::SurfaceWithEdges);
    m_volState.volume().setVisible(false);
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowWireframe") == 0)
    {
    m_volState.geometry().setVisible(true);
    m_volState.geometry().setRepresentation(Representation::WireFrame);
    m_volState.volume().setVisible(false);
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowPoints") == 0)
    {
    m_volState.geometry().setVisible(true);
    m_volState.geometry().setRepresentation(Representation::Points);
    m_volState.volume().setVisible(false);
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowNone") == 0)
    {
    m_volState.geometry().setVisible(false);
    m_volState.volume().setVisible(false);
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowVolume") == 0)
    {
    m_volState.volume().setVisible(callBackData->set);
    if (callBackData->set)
      {
      m_volState.geometry().setVisible(false);
      }
    }
  else if (strcmp(callBackData->toggle->getName(), "ShowOutline") == 0)
    {
    m_volState.outline().setVisible(callBackData->set);
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
  else if (strcmp(callBackData->toggle->getName(), "FreeSlice") == 0)
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
void ExampleVTKReader::setAIsosurface(float aIsosurface)
{
  m_volState.isosurfaceA().setContourValue(static_cast<double>(aIsosurface));
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setBIsosurface(float bIsosurface)
{
  m_volState.isosurfaceB().setContourValue(static_cast<double>(bIsosurface));
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setCIsosurface(float cIsosurface)
{
  m_volState.isosurfaceC().setContourValue(static_cast<double>(cIsosurface));
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showAIsosurface(bool visible)
{
  m_volState.isosurfaceA().setVisible(visible);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showBIsosurface(bool visible)
{
  m_volState.isosurfaceB().setVisible(visible);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showCIsosurface(bool visible)
{
  m_volState.isosurfaceC().setVisible(visible);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setFreeSliceVisibility(bool vis)
{
  m_volState.freeSlice().setVisible(vis);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
bool ExampleVTKReader::getFreeSliceVisibility()
{
  return m_volState.freeSlice().visible();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setFreeSliceOrigin(const double *origin)
{
  std::array<double, 3> o{origin[0], origin[1], origin[2]};
  m_volState.freeSlice().setOrigin(o);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
const double *ExampleVTKReader::getFreeSliceOrigin()
{
  return m_volState.freeSlice().origin().data();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setFreeSliceNormal(const double *normal)
{
  std::array<double, 3> n{normal[0], normal[1], normal[2]};
  m_volState.freeSlice().setNormal(n);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
const double *ExampleVTKReader::getFreeSliceNormal()
{
  return m_volState.freeSlice().normal().data();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setXSlice(int xSlice)
{
  m_volState.slices().setSliceLocation(0, xSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setYSlice(int ySlice)
{
  m_volState.slices().setSliceLocation(1, ySlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setZSlice(int zSlice)
{
  m_volState.slices().setSliceLocation(2, zSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showXSlice(bool xSlice)
{
  m_volState.slices().setSliceVisible(0, xSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showYSlice(bool ySlice)
{
  m_volState.slices().setSliceVisible(1, ySlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showZSlice(bool zSlice)
{
  m_volState.slices().setSliceVisible(2, zSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setXContourSlice(int xSlice)
{
  m_volState.slices().setContourSliceLocation(0, xSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setYContourSlice(int ySlice)
{
  m_volState.slices().setContourSliceLocation(1, ySlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setZContourSlice(int zSlice)
{
  m_volState.slices().setContourSliceLocation(2, zSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showXContourSlice(bool xSlice)
{
  m_volState.slices().setContourSliceVisible(0, xSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showYContourSlice(bool ySlice)
{
  m_volState.slices().setContourSliceVisible(1, ySlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::showZContourSlice(bool zSlice)
{
  m_volState.slices().setContourSliceVisible(2, zSlice);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getWidth(void)
{
  return m_volState.reader().dimensions()[0];
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getLength(void)
{
  return m_volState.reader().dimensions()[1];
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getHeight(void)
{
  return m_volState.reader().dimensions()[2];
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateIsosurfaceColorMap(double* IsosurfaceColormap)
{
  // TODO There was a leak here before the vtkVRUI refactor:
//  this->IsosurfaceColormap = IsosurfaceColormap;
  // Check that the input argument is cleaned up properly when called.

  std::copy(IsosurfaceColormap, IsosurfaceColormap + 256,
            m_volState.isosurfaceColorMap().data());
  m_volState.isosurfaceColorMapModified();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateSliceColorMap(double* SliceColormap)
{
  // TODO There was a leak here before the vtkVRUI refactor:
  //  this->SliceColormap = SliceColormap;
  // Check that the input argument is cleaned up properly when called.

  std::copy(SliceColormap, SliceColormap + 256,
            m_volState.sliceColorMap().data());
  m_volState.sliceColorMapModified();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::alphaChangedCallback(Misc::CallbackData* callBackData)
{
  transferFunctionDialog->exportAlpha(m_volState.colorMap().data());
  m_volState.colorMapModified();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::contourValueChangedCallback(Misc::CallbackData*)
{
  m_volState.contours().setContourValues(ContoursDialog->getContourValues());
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::volumeColorMapChangedCallback(
  Misc::CallbackData* callBackData)
{
  transferFunctionDialog->exportColorMap(m_volState.colorMap().data());
  m_volState.colorMapModified();
  this->updateAlpha();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateAlpha(void)
{
  transferFunctionDialog->exportAlpha(m_volState.colorMap().data());
  m_volState.colorMapModified();
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::updateVolumeColorMap(void)
{
  transferFunctionDialog->exportColorMap(m_volState.colorMap().data());
  m_volState.colorMapModified();
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
    m_volState.setForceLowResolution(callBackData->set);
    }
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMinimum(void)
{
  return float(m_volState.reader().scalarRange()[0]);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMaximum(void)
{
  return float(m_volState.reader().scalarRange()[1]);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataIncrement(void)
{
  std::array<double, 2> scalarRange = m_volState.reader().scalarRange();
  return static_cast<float>((scalarRange[1] - scalarRange[0]) / 20.0);
}

//----------------------------------------------------------------------------
float ExampleVTKReader::getDataMidPoint(void)
{
  std::array<double, 2> scalarRange = m_volState.reader().scalarRange();
  return static_cast<float>((scalarRange[1] - scalarRange[0]) / 2.0);
}

//----------------------------------------------------------------------------
std::vector<double> ExampleVTKReader::getContourValues()
{
  return m_volState.contours().contourValues();
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setContourVisible(bool visible)
{
  m_volState.contours().setVisible(visible);
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
float * ExampleVTKReader::getHistogram(void)
{
  return this->Histogram;
}

//----------------------------------------------------------------------------
void ExampleVTKReader::setRequestedRenderMode(int mode)
{
  m_volState.volume().setRenderMode(volVolume::RenderMode(mode));
  Vrui::requestUpdate();
}

//----------------------------------------------------------------------------
int ExampleVTKReader::getRequestedRenderMode(void) const
{
  return static_cast<int>(m_volState.volume().renderMode());
}

//----------------------------------------------------------------------------
vvContextState *ExampleVTKReader::createContextState() const
{
  return new volContextState;
}
