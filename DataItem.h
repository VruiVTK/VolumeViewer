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
class vtkCompositePolyDataMapper;
class vtkContourFilter;
class vtkPlaneCutter;
class vtkExtractVOI;
class vtkLight;
class vtkLookupTable;
class vtkPiecewiseFunction;
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
  vtkSmartPointer<vtkPlaneCutter> freeSliceCutter;
  vtkSmartPointer<vtkPlaneCutter> lowFreeSliceCutter;
  vtkSmartPointer<vtkPlaneCutter> lowXContourCutter;
  vtkSmartPointer<vtkPlaneCutter> lowXCutter;
  vtkSmartPointer<vtkPlaneCutter> lowYContourCutter;
  vtkSmartPointer<vtkPlaneCutter> lowYCutter;
  vtkSmartPointer<vtkPlaneCutter> lowZContourCutter;
  vtkSmartPointer<vtkPlaneCutter> lowZCutter;
  vtkSmartPointer<vtkPlaneCutter> xContourCutter;
  vtkSmartPointer<vtkPlaneCutter> xCutter;
  vtkSmartPointer<vtkPlaneCutter> yContourCutter;
  vtkSmartPointer<vtkPlaneCutter> yCutter;
  vtkSmartPointer<vtkPlaneCutter> zContourCutter;
  vtkSmartPointer<vtkPlaneCutter> zCutter;
  vtkSmartPointer<vtkExtractVOI> extract;
  vtkSmartPointer<vtkLight> flashlight;
  vtkSmartPointer<vtkCompositePolyDataMapper> aContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> bContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> cContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> freeSliceMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowAContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowBContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowCContourMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowFreeSliceMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowXCutterMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowYCutterMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> lowZCutterMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> xCutterMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> yCutterMapper;
  vtkSmartPointer<vtkCompositePolyDataMapper> zCutterMapper;
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
