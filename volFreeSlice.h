#ifndef VOLFREESLICE_H
#define VOLFREESLICE_H

#include <vvLODAsyncGLObject.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class vtkActor;
class vtkDataObject;
class vtkFlyingEdgesPlaneCutter;
class vtkLookupTable;
class vtkPlane;
class vtkPolyDataMapper;

class volFreeSlice : public vvLODAsyncGLObject
{
public:
  using Superclass = vvLODAsyncGLObject;

  volFreeSlice();
  ~volFreeSlice();

  bool visible() const;
  void setVisible(bool vis);

  const std::array<double, 3>& origin() const;
  void setOrigin(const std::array<double, 3> &o);

  const std::array<double, 3>& normal() const;
  void setNormal(const std::array<double, 3> &o);

  struct FreeSliceState : public ObjectState
  {
    FreeSliceState();

    void update(const vvApplicationState &state) override;

    vtkNew<vtkLookupTable> color;
    bool visible{false};
    std::array<double, 3> origin{0., 0., 0.};
    std::array<double, 3> normal{1., 0., 0.};
  };

  struct FreeSliceLODData : public LODData
  {
    vtkSmartPointer<vtkDataObject> slice;
  };

  struct FreeSliceDataPipeline : public DataPipeline
  {
    FreeSliceDataPipeline(LevelOfDetail l);

    void configure(const ObjectState &objState,
                   const vvApplicationState &appState) override;
    bool needsUpdate(const ObjectState &objState,
                     const LODData &result) const override;
    void execute() override;
    void exportResult(LODData &result) const override;

    LevelOfDetail lod;
    vtkNew<vtkPlane> plane;
    vtkNew<vtkFlyingEdgesPlaneCutter> cutter;
  };

  struct FreeSliceRenderPipeline : public RenderPipeline
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

#endif // VOLFREESLICE_H
