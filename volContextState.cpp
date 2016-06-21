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

  this->freesliceLUT = vtkSmartPointer<vtkLookupTable>::New();
  this->freesliceLUT->SetNumberOfColors(256);
  this->freesliceLUT->Build();
}
