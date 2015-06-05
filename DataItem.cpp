// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCutter.h>
#include <vtkImageProperty.h>
#include <vtkImageResample.h>
#include <vtkImageResliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

// ExampleVTKReader includes
#include "DataItem.h"

//----------------------------------------------------------------------------
ExampleVTKReader::DataItem::DataItem(void)
{
  this->extract = vtkSmartPointer<vtkImageResample>::New();
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

  this->mapperVolume = vtkSmartPointer<vtkSmartVolumeMapper>::New();
  this->lowMapperVolume = vtkSmartPointer<vtkSmartVolumeMapper>::New();
  this->actorVolume = vtkSmartPointer<vtkVolume>::New();
  this->lowActorVolume = vtkSmartPointer<vtkVolume>::New();
  renderer->AddVolume(this->actorVolume);
  renderer->AddVolume(this->lowActorVolume);
  this->propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
  this->lowPropertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();

  this->xReslice = vtkSmartPointer<vtkImageResliceMapper>::New();
  this->xImageProperty = vtkSmartPointer<vtkImageProperty>::New();
  this->actorXReslice = vtkSmartPointer<vtkImageSlice>::New();
  this->actorXReslice->SetMapper(this->xReslice);
  this->actorXReslice->SetProperty(this->xImageProperty);
  renderer->AddViewProp(this->actorXReslice);
  this->lowXReslice = vtkSmartPointer<vtkImageResliceMapper>::New();
  this->lowXImageProperty = vtkSmartPointer<vtkImageProperty>::New();
  this->lowActorXReslice = vtkSmartPointer<vtkImageSlice>::New();
  this->lowActorXReslice->SetMapper(this->lowXReslice);
  this->lowActorXReslice->SetProperty(this->lowXImageProperty);
  renderer->AddViewProp(this->lowActorXReslice);

  this->xCutter = vtkSmartPointer<vtkCutter>::New();
  this->xCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorXCutter = vtkSmartPointer<vtkActor>::New();
  this->lowXCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowXCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorXCutter = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorXCutter);
  renderer->AddActor(this->lowActorXCutter);
  this->yCutter = vtkSmartPointer<vtkCutter>::New();
  this->yCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorYCutter = vtkSmartPointer<vtkActor>::New();
  this->lowYCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowYCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorYCutter = vtkSmartPointer<vtkActor>::New();
  renderer->AddActor(this->actorYCutter);
  renderer->AddActor(this->lowActorYCutter);
  this->zCutter = vtkSmartPointer<vtkCutter>::New();
  this->zCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorZCutter = vtkSmartPointer<vtkActor>::New();
  this->lowZCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowZCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
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
  this->aContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorAContour = vtkSmartPointer<vtkActor>::New();
  this->lowAContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowAContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorAContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorAContour);
  renderer->AddVolume(this->lowActorAContour);
  this->bContour = vtkSmartPointer<vtkContourFilter>::New();
  this->bContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorBContour = vtkSmartPointer<vtkActor>::New();
  this->lowBContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowBContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorBContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorBContour);
  renderer->AddVolume(this->lowActorBContour);
  this->cContour = vtkSmartPointer<vtkContourFilter>::New();
  this->cContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorCContour = vtkSmartPointer<vtkActor>::New();
  this->lowCContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowCContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorCContour = vtkSmartPointer<vtkActor>::New();
  renderer->AddVolume(this->actorCContour);
  renderer->AddVolume(this->lowActorCContour);

  this->freeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->freeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->freeSliceMapper->SetInputConnection(this->freeSliceCutter->GetOutputPort());
  this->freeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->freeSliceActor->SetMapper(this->freeSliceMapper);
  renderer->AddActor(this->freeSliceActor);
  this->lowFreeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowFreeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowFreeSliceMapper->SetInputConnection(this->lowFreeSliceCutter->GetOutputPort());
  this->lowFreeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->lowFreeSliceActor->SetMapper(this->lowFreeSliceMapper);
  renderer->AddActor(this->lowFreeSliceActor);

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
