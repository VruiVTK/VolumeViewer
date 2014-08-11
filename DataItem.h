#ifndef _DATAITEM_H
#define _DATAITEM_H

// OpenGL/Motif includes
#include <GL/GLObject.h>

// ExampleVTKReader includes
#include "ExampleVTKReader.h"

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

struct ExampleVTKReader::DataItem : public GLObject::DataItem
{
/* Elements */
public:
  /* VTK components */
  vtkSmartPointer<ExternalVTKWidget> externalVTKWidget;
  vtkSmartPointer<vtkActor> actor;
  vtkSmartPointer<vtkActor> actorAContour;
  vtkSmartPointer<vtkActor> actorBContour;
  vtkSmartPointer<vtkActor> actorCContour;
  vtkSmartPointer<vtkActor> actorOutline;
  vtkSmartPointer<vtkActor> actorXContourCutter;
  vtkSmartPointer<vtkActor> actorXCutter;
  vtkSmartPointer<vtkActor> actorYContourCutter;
  vtkSmartPointer<vtkActor> actorYCutter;
  vtkSmartPointer<vtkActor> actorZContourCutter;
  vtkSmartPointer<vtkActor> actorZCutter;
  vtkSmartPointer<vtkActor> contourActor;
  vtkSmartPointer<vtkActor> freeSliceActor;
  vtkSmartPointer<vtkActor> lowActor;
  vtkSmartPointer<vtkActor> lowActorAContour;
  vtkSmartPointer<vtkActor> lowActorBContour;
  vtkSmartPointer<vtkActor> lowActorCContour;
  vtkSmartPointer<vtkActor> lowActorOutline;
  vtkSmartPointer<vtkActor> lowActorXContourCutter;
  vtkSmartPointer<vtkActor> lowActorXCutter;
  vtkSmartPointer<vtkActor> lowActorYContourCutter;
  vtkSmartPointer<vtkActor> lowActorYCutter;
  vtkSmartPointer<vtkActor> lowActorZContourCutter;
  vtkSmartPointer<vtkActor> lowActorZCutter;
  vtkSmartPointer<vtkActor> lowContourActor;
  vtkSmartPointer<vtkActor> lowFreeSliceActor;
  vtkSmartPointer<vtkContourFilter> aContour;
  vtkSmartPointer<vtkContourFilter> bContour;
  vtkSmartPointer<vtkContourFilter> cContour;
  vtkSmartPointer<vtkContourFilter> contourFilter;
  vtkSmartPointer<vtkContourFilter> lowAContour;
  vtkSmartPointer<vtkContourFilter> lowBContour;
  vtkSmartPointer<vtkContourFilter> lowCContour;
  vtkSmartPointer<vtkContourFilter> lowContourFilter;
  vtkSmartPointer<vtkCutter> freeSliceCutter;
  vtkSmartPointer<vtkCutter> lowFreeSliceCutter;
  vtkSmartPointer<vtkCutter> lowXContourCutter;
  vtkSmartPointer<vtkCutter> lowXCutter;
  vtkSmartPointer<vtkCutter> lowYContourCutter;
  vtkSmartPointer<vtkCutter> lowYCutter;
  vtkSmartPointer<vtkCutter> lowZContourCutter;
  vtkSmartPointer<vtkCutter> lowZCutter;
  vtkSmartPointer<vtkCutter> xContourCutter;
  vtkSmartPointer<vtkCutter> xCutter;
  vtkSmartPointer<vtkCutter> yContourCutter;
  vtkSmartPointer<vtkCutter> yCutter;
  vtkSmartPointer<vtkCutter> zContourCutter;
  vtkSmartPointer<vtkCutter> zCutter;
  vtkSmartPointer<vtkExtractVOI> extract;
  vtkSmartPointer<vtkLight> flashlight;
  vtkSmartPointer<vtkPolyDataMapper> aContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> bContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> cContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> freeSliceMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowAContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowBContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowCContourMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowFreeSliceMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowXCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowYCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> lowZCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> xCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> yCutterMapper;
  vtkSmartPointer<vtkPolyDataMapper> zCutterMapper;
  vtkSmartPointer<vtkVolume> actorVolume;
  vtkSmartPointer<vtkVolume> lowActorVolume;
  vtkSmartPointer<vtkVolumeProperty> lowPropertyVolume;
  vtkSmartPointer<vtkVolumeProperty> propertyVolume;

  vtkSmartPointer<vtkLookupTable> modelLUT;
  vtkSmartPointer<vtkLookupTable> sliceLUT;
  vtkSmartPointer<vtkLookupTable> isosurfaceLUT;
  vtkSmartPointer<vtkColorTransferFunction> colorFunction;
  vtkSmartPointer<vtkPiecewiseFunction> opacityFunction;

  /* Constructor and destructor*/
  DataItem(void);
  virtual ~DataItem(void);
};

#endif //_DATAITEM_H
