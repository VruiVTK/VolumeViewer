// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkCompositePolyDataMapper.h>
#include <vtkContourFilter.h>
#include <vtkPlaneCutter.h>
#include <vtkExtractVOI.h>
#include <vtkLight.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
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

  /* Use depth peeling to enable transparency */
  vtkRenderer* renderer = this->externalVTKWidget->AddRenderer();
  renderer->SetUseDepthPeeling(1);
  renderer->SetMaximumNumberOfPeels(4);
  renderer->SetOcclusionRatio(0.1);

  this->actor = vtkSmartPointer<vtkActor>::New();
  this->lowActor = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actor);
  renderer->AddActor(this->lowActor);

  this->actorOutline = vtkSmartPointer<vtkActor>::New();
  this->lowActorOutline = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorOutline);
  renderer->AddActor(this->lowActorOutline);

  this->actorVolume = vtkSmartPointer<vtkVolume>::New();
  this->lowActorVolume = vtkSmartPointer<vtkVolume>::New();
  renderer->AddVolume(this->actorVolume);
  renderer->AddVolume(this->lowActorVolume);
  this->propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
  this->lowPropertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();

  this->xCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->xCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorXCutter = vtkSmartPointer<vtkActor>::New();
  this->lowXCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->lowXCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorXCutter = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorXCutter);
  renderer->AddActor(this->lowActorXCutter);
  this->yCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->yCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorYCutter = vtkSmartPointer<vtkActor>::New();
  this->lowYCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->lowYCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorYCutter = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorYCutter);
  renderer->AddActor(this->lowActorYCutter);
  this->zCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->zCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorZCutter = vtkSmartPointer<vtkActor>::New();
  this->lowZCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->lowZCutterMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorZCutter = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorZCutter);
  renderer->AddActor(this->lowActorZCutter);

  this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->contourActor = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->contourActor);
  this->lowContourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->lowContourActor = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->lowContourActor);

  this->actorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->actorXContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->lowActorXContourCutter->GetProperty()->SetLineWidth(3);
  renderer->AddActor(this->actorXContourCutter);
  renderer->AddActor(this->lowActorXContourCutter);
  this->actorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->actorYContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->lowActorYContourCutter->GetProperty()->SetLineWidth(3);
  renderer->AddActor(this->actorYContourCutter);
  renderer->AddActor(this->lowActorYContourCutter);
  this->actorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->actorZContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->lowActorZContourCutter->GetProperty()->SetLineWidth(3);
  renderer->AddActor(this->actorZContourCutter);
  renderer->AddActor(this->lowActorZContourCutter);

  this->aContour = vtkSmartPointer<vtkContourFilter>::New();
  this->aContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorAContour = vtkSmartPointer<vtkActor>::New();
  this->lowAContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowAContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorAContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorAContour);
  renderer->AddVolume(this->lowActorAContour);
  this->bContour = vtkSmartPointer<vtkContourFilter>::New();
  this->bContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorBContour = vtkSmartPointer<vtkActor>::New();
  this->lowBContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowBContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorBContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorBContour);
  renderer->AddVolume(this->lowActorBContour);
  this->cContour = vtkSmartPointer<vtkContourFilter>::New();
  this->cContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->actorCContour = vtkSmartPointer<vtkActor>::New();
  this->lowCContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowCContourMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowActorCContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorCContour);
  renderer->AddVolume(this->lowActorCContour);

  this->freeSliceCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->freeSliceMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->freeSliceMapper->SetInputConnection(this->freeSliceCutter->GetOutputPort());
  this->freeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->freeSliceActor->SetMapper(this->freeSliceMapper);
  renderer->AddActor(this->freeSliceActor);
  this->lowFreeSliceCutter = vtkSmartPointer<vtkPlaneCutter>::New();
  this->lowFreeSliceMapper = vtkSmartPointer<vtkCompositePolyDataMapper>::New();
  this->lowFreeSliceMapper->SetInputConnection(this->lowFreeSliceCutter->GetOutputPort());
  this->lowFreeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->lowFreeSliceActor->SetMapper(this->lowFreeSliceMapper);
  renderer->AddActor(this->lowFreeSliceActor);

  this->flashlight = vtkSmartPointer<vtkLight>::New();
  renderer->AddLight(this->flashlight);


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
