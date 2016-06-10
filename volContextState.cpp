#include "volContextState.h"

// VTK includes
#include <ExternalVTKWidget.h>
#include <vtkActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkCutter.h>
#include <vtkExtractVOI.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

volContextState::volContextState()
{
  this->extract = vtkSmartPointer<vtkExtractVOI>::New();

  this->actor = vtkSmartPointer<vtkActor>::New();
  this->lowActor = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->actor);
  this->renderer().AddActor(this->lowActor);

  this->actorOutline = vtkSmartPointer<vtkActor>::New();
  this->lowActorOutline = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->actorOutline);
  this->renderer().AddActor(this->lowActorOutline);

  this->actorVolume = vtkSmartPointer<vtkVolume>::New();
  this->lowActorVolume = vtkSmartPointer<vtkVolume>::New();
  this->renderer().AddVolume(this->actorVolume);
  this->renderer().AddVolume(this->lowActorVolume);
  this->propertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();
  this->lowPropertyVolume = vtkSmartPointer<vtkVolumeProperty>::New();

  this->xCutter = vtkSmartPointer<vtkCutter>::New();
  this->xCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorXCutter = vtkSmartPointer<vtkActor>::New();
  this->lowXCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowXCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorXCutter = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->actorXCutter);
  this->renderer().AddActor(this->lowActorXCutter);
  this->yCutter = vtkSmartPointer<vtkCutter>::New();
  this->yCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorYCutter = vtkSmartPointer<vtkActor>::New();
  this->lowYCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowYCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorYCutter = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->actorYCutter);
  this->renderer().AddActor(this->lowActorYCutter);
  this->zCutter = vtkSmartPointer<vtkCutter>::New();
  this->zCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorZCutter = vtkSmartPointer<vtkActor>::New();
  this->lowZCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowZCutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorZCutter = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->actorZCutter);
  this->renderer().AddActor(this->lowActorZCutter);

  this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->contourActor = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->contourActor);
  this->lowContourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->lowContourActor = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->lowContourActor);

  this->actorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->actorXContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorXContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorXContourCutter->GetProperty()->SetColor(1.0, 0.0, 0.0);
  this->lowActorXContourCutter->GetProperty()->SetLineWidth(3);
  this->renderer().AddActor(this->actorXContourCutter);
  this->renderer().AddActor(this->lowActorXContourCutter);
  this->actorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->actorYContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorYContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorYContourCutter->GetProperty()->SetColor(0.0, 1.0, 0.0);
  this->lowActorYContourCutter->GetProperty()->SetLineWidth(3);
  this->renderer().AddActor(this->actorYContourCutter);
  this->renderer().AddActor(this->lowActorYContourCutter);
  this->actorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->actorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->actorZContourCutter->GetProperty()->SetLineWidth(3);
  this->lowActorZContourCutter = vtkSmartPointer<vtkActor>::New();
  this->lowActorZContourCutter->GetProperty()->SetColor(0.0, 0.0, 1.0);
  this->lowActorZContourCutter->GetProperty()->SetLineWidth(3);
  this->renderer().AddActor(this->actorZContourCutter);
  this->renderer().AddActor(this->lowActorZContourCutter);

  this->aContour = vtkSmartPointer<vtkContourFilter>::New();
  this->aContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorAContour = vtkSmartPointer<vtkActor>::New();
  this->lowAContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowAContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorAContour = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddVolume(this->actorAContour);
  this->renderer().AddVolume(this->lowActorAContour);
  this->bContour = vtkSmartPointer<vtkContourFilter>::New();
  this->bContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorBContour = vtkSmartPointer<vtkActor>::New();
  this->lowBContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowBContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorBContour = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddVolume(this->actorBContour);
  this->renderer().AddVolume(this->lowActorBContour);
  this->cContour = vtkSmartPointer<vtkContourFilter>::New();
  this->cContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->actorCContour = vtkSmartPointer<vtkActor>::New();
  this->lowCContour = vtkSmartPointer<vtkContourFilter>::New();
  this->lowCContourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowActorCContour = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddVolume(this->actorCContour);
  this->renderer().AddVolume(this->lowActorCContour);

  this->freeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->freeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->freeSliceMapper->SetInputConnection(this->freeSliceCutter->GetOutputPort());
  this->freeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->freeSliceActor->SetMapper(this->freeSliceMapper);
  this->renderer().AddActor(this->freeSliceActor);
  this->lowFreeSliceCutter = vtkSmartPointer<vtkCutter>::New();
  this->lowFreeSliceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->lowFreeSliceMapper->SetInputConnection(this->lowFreeSliceCutter->GetOutputPort());
  this->lowFreeSliceActor = vtkSmartPointer<vtkActor>::New();
  this->lowFreeSliceActor->SetMapper(this->lowFreeSliceMapper);
  this->renderer().AddActor(this->lowFreeSliceActor);

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
