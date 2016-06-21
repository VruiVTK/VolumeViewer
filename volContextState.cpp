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
}
