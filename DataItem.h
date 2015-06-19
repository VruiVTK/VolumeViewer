#ifndef _DATAITEM_H
#define _DATAITEM_H
// VolumeViewer includes
#include "VolumeViewer.h"

// OpenGL/Motif includes
#include <GL/GLObject.h>

#include <vtkSmartPointer.h>

// Forward declarations
class ExternalVTKWidget;
class vtkActor;
class vtkColorTransferFunction;
class vtkContourFilter;
class vtkCutter;
class vtkExternalLight;
class vtkImageProperty;
class vtkImageResample;
class vtkImageResliceMapper;
class vtkImageSlice;
class vtkLight;
class vtkLookupTable;
class vtkPiecewiseFunction;
class vtkPolyDataMapper;
class vtkProperty;
class vtkRenderer;
class vtkSmartVolumeMapper;
class vtkVolume;
class vtkVolumeProperty;

struct VolumeViewer::DataItem : public GLObject::DataItem
{
/* Elements */
public:
  /* VTK components */
  vtkSmartPointer<ExternalVTKWidget> externalVTKWidget;
  vtkSmartPointer<vtkExternalLight> externalLight;
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

  vtkSmartPointer<vtkImageResliceMapper> xReslice;
  vtkSmartPointer<vtkImageProperty> xImageProperty;
  vtkSmartPointer<vtkImageSlice> actorXReslice;
  vtkSmartPointer<vtkImageResliceMapper> lowXReslice;
  vtkSmartPointer<vtkImageProperty> lowXImageProperty;
  vtkSmartPointer<vtkImageSlice> lowActorXReslice;

  vtkSmartPointer<vtkCutter> xCutter;
  vtkSmartPointer<vtkCutter> yContourCutter;
  vtkSmartPointer<vtkCutter> yCutter;
  vtkSmartPointer<vtkCutter> zContourCutter;
  vtkSmartPointer<vtkCutter> zCutter;
  vtkSmartPointer<vtkImageResample> extract;
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
  vtkSmartPointer<vtkSmartVolumeMapper> mapperVolume;
  vtkSmartPointer<vtkSmartVolumeMapper> lowMapperVolume;
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
