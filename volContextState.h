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

  // Contours (a,b,c)
  vtkSmartPointer<vtkContourFilter> aContour;
  vtkSmartPointer<vtkContourFilter> bContour;
  vtkSmartPointer<vtkContourFilter> cContour;
  vtkSmartPointer<vtkPolyDataMapper> aContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> bContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> cContourMapper;
  vtkSmartPointer<vtkActor> actorAContour;
  vtkSmartPointer<vtkActor> actorBContour;
  vtkSmartPointer<vtkActor> actorCContour;
  vtkSmartPointer<vtkContourFilter> lowAContour;
  vtkSmartPointer<vtkContourFilter> lowBContour;
  vtkSmartPointer<vtkContourFilter> lowCContour;
  vtkSmartPointer<vtkPolyDataMapper> lowAContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowBContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowCContourMapper;
  vtkSmartPointer<vtkActor> lowActorAContour;
  vtkSmartPointer<vtkActor> lowActorBContour;
  vtkSmartPointer<vtkActor> lowActorCContour;

  // Contour (free form)
  vtkSmartPointer<vtkContourFilter> contourFilter;
  vtkSmartPointer<vtkActor> contourActor;
  vtkSmartPointer<vtkContourFilter> lowContourFilter;
  vtkSmartPointer<vtkActor> lowContourActor;

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
  vtkSmartPointer<vtkLookupTable> isosurfaceLUT;
};

#endif // VOLCONTEXTSTATE_H
