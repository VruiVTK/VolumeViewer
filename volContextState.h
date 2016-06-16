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

  // Contours
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

  // More contours (?)
  vtkSmartPointer<vtkCutter> xContourCutter;
  vtkSmartPointer<vtkCutter> xCutter;
  vtkSmartPointer<vtkCutter> yContourCutter;
  vtkSmartPointer<vtkCutter> yCutter;
  vtkSmartPointer<vtkCutter> zContourCutter;
  vtkSmartPointer<vtkCutter> zCutter;
  vtkSmartPointer<vtkPolyDataMapper> xCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> yCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> zCutterMapper;
  vtkSmartPointer<vtkActor> actorXContourCutter;
  vtkSmartPointer<vtkActor> actorXCutter;
  vtkSmartPointer<vtkActor> actorYContourCutter;
  vtkSmartPointer<vtkActor> actorYCutter;
  vtkSmartPointer<vtkActor> actorZContourCutter;
  vtkSmartPointer<vtkActor> actorZCutter;
  vtkSmartPointer<vtkCutter> lowXContourCutter;
  vtkSmartPointer<vtkCutter> lowXCutter;
  vtkSmartPointer<vtkCutter> lowYContourCutter;
  vtkSmartPointer<vtkCutter> lowYCutter;
  vtkSmartPointer<vtkCutter> lowZContourCutter;
  vtkSmartPointer<vtkCutter> lowZCutter;
  vtkSmartPointer<vtkPolyDataMapper> lowXCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowYCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowZCutterMapper;
  vtkSmartPointer<vtkActor> lowActorXContourCutter;
  vtkSmartPointer<vtkActor> lowActorXCutter;
  vtkSmartPointer<vtkActor> lowActorYContourCutter;
  vtkSmartPointer<vtkActor> lowActorYCutter;
  vtkSmartPointer<vtkActor> lowActorZContourCutter;
  vtkSmartPointer<vtkActor> lowActorZCutter;

  // Yet another contour (??):
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

  // Volume:
  vtkSmartPointer<vtkVolumeProperty> propertyVolume;
  vtkSmartPointer<vtkVolume> actorVolume;
  vtkSmartPointer<vtkVolumeProperty> lowPropertyVolume;
  vtkSmartPointer<vtkVolume> lowActorVolume;

  // TODO see if these are shared. If so, leave them here. If not, put in
  // vvGLObjects.
  vtkSmartPointer<vtkLookupTable> sliceLUT;
  vtkSmartPointer<vtkLookupTable> isosurfaceLUT;
  vtkSmartPointer<vtkColorTransferFunction> colorFunction;
  vtkSmartPointer<vtkPiecewiseFunction> opacityFunction;
};

#endif // VOLCONTEXTSTATE_H
