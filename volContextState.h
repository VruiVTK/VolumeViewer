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

  // Extract (probably used by one of the other filters
  vtkSmartPointer<vtkExtractVOI> extract;
};

#endif // VOLCONTEXTSTATE_H
