// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCutter.h>
#include <vtkLight.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

// ExampleVTKReader includes
#include "DataItem.h"

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

  this->aContour = vtkSmartPointer<vtkContourFilter>::New();
  this->aContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorAContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorAContour);
  this->bContour = vtkSmartPointer<vtkContourFilter>::New();
  this->bContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorBContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorBContour);
  this->cContour = vtkSmartPointer<vtkContourFilter>::New();
  this->cContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorCContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorCContour);

  this->freeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->freeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->freeSliceMapper->SetInputConnection(
    this->freeSliceCutter->GetOutputPort());
  this->freeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->freeSliceActor->SetMapper(this->freeSliceMapper);

  this->flashlight = vtkSmartPointer<vtkLight>::New();
  this->externalVTKWidget->GetRenderer()->AddLight(this->flashlight);


  this->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
  this->opacityFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();

  this->isosurfaceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->isosurfaceLUT->SetNumberOfColors(256);
  this->isosurfaceLUT->Build();

  this->modelLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->modelLUT->SetNumberOfColors(256);
  this->modelLUT->Build();

  this->sliceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->sliceLUT->SetNumberOfColors(256);
  this->sliceLUT->Build();
}

//----------------------------------------------------------------------------
ExampleVTKReader::DataItem::~DataItem(void)
{
}
