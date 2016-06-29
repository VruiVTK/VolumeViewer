#ifndef VOLISOSURFACE_H
#define VOLISOSURFACE_H

#include <vvLODAsyncGLObject.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class vtkActor;
class vtkDataObject;
class vtkFlyingEdges3D;
class vtkLookupTable;
class vtkPolyDataMapper;

class volIsosurface : public vvLODAsyncGLObject
{
public:
  using Superclass = vvLODAsyncGLObject;

  volIsosurface();
  ~volIsosurface();

  bool visible() const;
  void setVisible(bool vis);

  double contourValue() const;
  void setContourValue(double val);

  struct IsosurfaceState : public ObjectState
  {
    IsosurfaceState();

    void update(const vvApplicationState &state) override;

    vtkNew<vtkLookupTable> color;
    bool visible{false};
    double contourValue{0.};
  };

  struct IsosurfaceLODData : public LODData
  {
    vtkSmartPointer<vtkDataObject> contour;
  };

  struct IsosurfaceDataPipeline : public DataPipeline
  {
    IsosurfaceDataPipeline(LevelOfDetail l);

    void configure(const ObjectState &objState,
                   const vvApplicationState &appState) override;
    bool needsUpdate(const ObjectState &objState,
                     const LODData &result) const override;
    void execute() override;
    void exportResult(LODData &result) const override;

    LevelOfDetail lod;
    vtkNew<vtkFlyingEdges3D> contour;
  };

  struct IsosurfaceRenderPipeline : public RenderPipeline
  {
    void init(const ObjectState &objState,
              vvContextState &contextState) override;
    void update(const ObjectState &objState,
                const vvApplicationState &appState,
                const vvContextState &contextState,
                const LODData &result) override;
    void disable() override;

    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkActor> actor;
  };

private:

  std::string progressLabel() const override;

  ObjectState* createObjectState() const override;

  DataPipeline* createDataPipeline(LevelOfDetail lod) const override;

  RenderPipeline* createRenderPipeline(LevelOfDetail lod) const override;

  LODData* createLODData(LevelOfDetail lod) const override;

};

#endif // VOLISOSURFACE_H
