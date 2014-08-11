// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCutter.h>
#include <vtkExtractVOI.h>
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
  this->extract = vtkSmartPointer<vtkExtractVOI>::New();
  /* Initialize VTK renderwindow and renderer */
  this->externalVTKWidget = vtkSmartPointer<ExternalVTKWidget>::New();
  this->actor = vtkSmartPointer<vtkActor>::New();
  this->lowActor = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actor);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActor);

  this->actorOutline = vtkSmartPointer<vtkActor>::New();
  this->lowActorOutline = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorOutline);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorOutline);

  this->actorVolume = vtkSmartPointer<vtkVolume>::New();
  this->lowActorVolume = vtkSmartPointer<vtkVolume>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorVolume);
  this->externalVTKWidget->GetRenderer()->AddVolume(this->lowActorVolume);
  this->propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
  this->lowPropertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();

  this->xCutter = vtkSmartPointer<vtkCutter>::New();
  this->xCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorXCutter = vtkSmartPointer<vtkActor>::New();
  this->lowXCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowXCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorXCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorXCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorXCutter);
  this->yCutter = vtkSmartPointer<vtkCutter>::New();
  this->yCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorYCutter = vtkSmartPointer<vtkActor>::New();
  this->lowYCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowYCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorYCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorYCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorYCutter);
  this->zCutter = vtkSmartPointer<vtkCutter>::New();
  this->zCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorZCutter = vtkSmartPointer<vtkActor>::New();
  this->lowZCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowZCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorZCutter = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorZCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorZCutter);

  this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->contourActor = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->contourActor);
  this->lowContourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->lowContourActor = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowContourActor);

  this->actorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->actorXContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->lowActorXContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorXContourCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorXContourCutter);
  this->actorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->actorYContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->lowActorYContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorYContourCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorYContourCutter);
  this->actorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->actorZContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->lowActorZContourCutter->GetProperty()->SetLineWidth(3);
  this->externalVTKWidget->GetRenderer()->AddActor(this->actorZContourCutter);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowActorZContourCutter);

  this->aContour = vtkSmartPointer<vtkContourFilter>::New();
  this->aContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorAContour = vtkSmartPointer<vtkActor>::New();
  this->lowAContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowAContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorAContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorAContour);
  this->externalVTKWidget->GetRenderer()->AddVolume(this->lowActorAContour);
  this->bContour = vtkSmartPointer<vtkContourFilter>::New();
  this->bContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorBContour = vtkSmartPointer<vtkActor>::New();
  this->lowBContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowBContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorBContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorBContour);
  this->externalVTKWidget->GetRenderer()->AddVolume(this->lowActorBContour);
  this->cContour = vtkSmartPointer<vtkContourFilter>::New();
  this->cContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorCContour = vtkSmartPointer<vtkActor>::New();
  this->lowCContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowCContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorCContour = vtkSmartPointer<vtkActor>::New();
  this->externalVTKWidget->GetRenderer()->AddVolume(this->actorCContour);
  this->externalVTKWidget->GetRenderer()->AddVolume(this->lowActorCContour);

  this->freeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->freeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->freeSliceMapper->SetInputConnection(this->freeSliceCutter->GetOutputPort());
  this->freeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->freeSliceActor->SetMapper(this->freeSliceMapper);
  this->externalVTKWidget->GetRenderer()->AddActor(this->freeSliceActor);
  this->lowFreeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowFreeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowFreeSliceMapper->SetInputConnection(this->lowFreeSliceCutter->GetOutputPort());
  this->lowFreeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->lowFreeSliceActor->SetMapper(this->lowFreeSliceMapper);
  this->externalVTKWidget->GetRenderer()->AddActor(this->lowFreeSliceActor);

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
