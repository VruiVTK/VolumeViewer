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

  this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->contourActor = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->contourActor);
  this->lowContourFilter = vtkSmartPointer<vtkContourFilter>::New();
  this->lowContourActor = vtkSmartPointer<vtkActor>::New();
  this->renderer().AddActor(this->lowContourActor);

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

  this->isosurfaceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->isosurfaceLUT->SetNumberOfColors(256);
  this->isosurfaceLUT->Build();

  this->freesliceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->freesliceLUT->SetNumberOfColors(256);
  this->freesliceLUT->Build();
}
