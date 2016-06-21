#ifndef VOLCONTEXTSTATE_H
#define VOLCONTEXTSTATE_H

#include <vvContextState.h>

// VTK includes
#include <vtkSmartPointer.h>

// Forward declarations
class ExternalVTKWidget;
class vtkActor;
class vtkColorTransferFunction;
class vtkContourFilter;
class vtkCutter;
class vtkExtractVOI;
class vtkLight;
class vtkLookupTable;
class vtkPiecewiseFunction;
class vtkPolyDataMapper;
class vtkProperty;
class vtkRenderer;
class vtkVolume;
class vtkVolumeProperty;

// Most of this class could be refactored into vvGLObjects.
class volContextState : public vvContextState
{
public:
  volContextState();

  // TODO refactor these into vvGLObjects:

  // Slice:
  vtkSmartPointer<vtkCutter> freeSliceCutter;
  vtkSmartPointer<vtkPolyDataMapper> freeSliceMapper;
  vtkSmartPointer<vtkActor> freeSliceActor;
  vtkSmartPointer<vtkCutter> lowFreeSliceCutter;
  vtkSmartPointer<vtkPolyDataMapper> lowFreeSliceMapper;
  vtkSmartPointer<vtkActor> lowFreeSliceActor;

  // Extract (probably used by one of the other filters
  vtkSmartPointer<vtkExtractVOI> extract;

  // TODO see if these are shared. If so, leave them here. If not, put in
  // vvGLObjects.
  vtkSmartPointer<vtkLookupTable> freesliceLUT;
};

#endif // VOLCONTEXTSTATE_H
