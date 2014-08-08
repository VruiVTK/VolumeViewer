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
class vtkContourFilter;
class vtkCutter;
class vtkLight;
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
  vtkSmartPointer<vtkActor> actorOutline;
  vtkSmartPointer<vtkLight> flashlight;
  vtkSmartPointer<vtkVolume> actorVolume;
  vtkSmartPointer<vtkVolumeProperty> propertyVolume;
  vtkSmartPointer<vtkCutter> xCutter;
  vtkSmartPointer<vtkPolyDataMapper> xCutterMapper;
  vtkSmartPointer<vtkActor> actorXCutter;
  vtkSmartPointer<vtkCutter> yCutter;
  vtkSmartPointer<vtkPolyDataMapper> yCutterMapper;
  vtkSmartPointer<vtkActor> actorYCutter;
  vtkSmartPointer<vtkCutter> zCutter;
  vtkSmartPointer<vtkPolyDataMapper> zCutterMapper;
  vtkSmartPointer<vtkActor> actorZCutter;
  vtkSmartPointer<vtkContourFilter> aContour;
  vtkSmartPointer<vtkPolyDataMapper> aContourMapper;
  vtkSmartPointer<vtkActor> actorAContour;
  vtkSmartPointer<vtkContourFilter> bContour;
  vtkSmartPointer<vtkPolyDataMapper> bContourMapper;
  vtkSmartPointer<vtkActor> actorBContour;
  vtkSmartPointer<vtkContourFilter> cContour;
  vtkSmartPointer<vtkPolyDataMapper> cContourMapper;
  vtkSmartPointer<vtkActor> actorCContour;
  vtkSmartPointer<vtkContourFilter> contourFilter;
  vtkSmartPointer<vtkActor> contourActor;
  vtkSmartPointer<vtkCutter> xContourCutter;
  vtkSmartPointer<vtkCutter> yContourCutter;
  vtkSmartPointer<vtkCutter> zContourCutter;
  vtkSmartPointer<vtkActor> actorXContourCutter;
  vtkSmartPointer<vtkActor> actorYContourCutter;
  vtkSmartPointer<vtkActor> actorZContourCutter;
  vtkSmartPointer<vtkCutter> freeSliceCutter;
  vtkSmartPointer<vtkPolyDataMapper> freeSliceMapper;
  vtkSmartPointer<vtkActor> freeSliceActor;

  /* Constructor and destructor*/
  DataItem(void);
  virtual ~DataItem(void);
};

#endif //_DATAITEM_H
