#ifndef VOLSLICES_H
#define VOLSLICES_H

#include <vvLODAsyncGLObject.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class vtkActor;
class vtkCutter;
class vtkDataObject;
class vtkLookupTable;
class vtkPlane;
class vtkPolyDataMapper;

class volSlices : public vvLODAsyncGLObject
{
public:
  using Superclass = vvLODAsyncGLObject;

  volSlices();
  ~volSlices() override;

  void setSliceVisible(size_t dim, bool vis);
  bool sliceVisible(size_t dim) const;

  void setContourSliceVisible(size_t dim, bool vis);
  bool contourSliceVisible(size_t dim) const;

  void setSliceLocation(size_t dim, size_t sliceIdx);
  size_t sliceLocation(size_t dim) const;

  void setContourSliceLocation(size_t dim, size_t sliceIdx);
  size_t contourSliceLocation(size_t dim) const;

  struct ObjectState : public Superclass::ObjectState
  {
    ObjectState();

    void update(const vvApplicationState &state) override;

    vtkNew<vtkLookupTable> color;

    // Which plane of the image data to use:
    std::array<size_t, 3> sliceLocations;
    std::array<size_t, 3> contourLocations;

    std::array<bool, 3> sliceVisible;
    std::array<bool, 3> contourVisible;

    std::array<vtkNew<vtkPlane>, 3> slicePlanes;
    std::array<vtkNew<vtkPlane>, 3> contourPlanes;
  };

  struct LODData : public Superclass::LODData
  {
    std::array<vtkSmartPointer<vtkDataObject>, 3> slices;
    std::array<vtkSmartPointer<vtkDataObject>, 3> contours;
  };

  struct DataPipeline : public Superclass::DataPipeline
  {
    DataPipeline(LevelOfDetail l);

    void configure(const Superclass::ObjectState &objState,
                   const vvApplicationState &appState) override;
    bool needsUpdate(const Superclass::ObjectState &objState,
                     const Superclass::LODData &result) const override;
    void execute() override;
    void exportResult(Superclass::LODData &result) const override;

    LevelOfDetail lod;
    std::array<vtkNew<vtkCutter>, 3> sliceCutters;
    std::array<vtkNew<vtkCutter>, 3> contourCutters;
  };

  struct RenderPipeline : public Superclass::RenderPipeline
  {
    void init(const Superclass::ObjectState &objState,
              vvContextState &contextState) override;
    void update(const Superclass::ObjectState &objState,
                const vvApplicationState &appState,
                const vvContextState &contextState,
                const Superclass::LODData &result) override;
    void disable() override;

    std::array<vtkNew<vtkPolyDataMapper>, 3> sliceMappers;
    std::array<vtkNew<vtkActor>, 3> sliceActors;
    std::array<vtkNew<vtkPolyDataMapper>, 3> contourMappers;
    std::array<vtkNew<vtkActor>, 3> contourActors;
  };

private:
  std::string progressLabel() const override;

  Superclass::ObjectState* createObjectState() const override;

  Superclass::DataPipeline*
  createDataPipeline(LevelOfDetail lod) const override;

  Superclass::RenderPipeline*
  createRenderPipeline(LevelOfDetail lod) const override;

  Superclass::LODData* createLODData(LevelOfDetail lod) const override;
};

#endif // VOLSLICES_H
