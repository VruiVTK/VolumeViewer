#ifndef VOLCONTOURS_H
#define VOLCONTOURS_H

#include <vvLODAsyncGLObject.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vector>

class vtkActor;
class vtkDataObject;
class vtkContourFilter;
class vtkPolyDataMapper;

class volContours : public vvLODAsyncGLObject
{
public:
  using Superclass = vvLODAsyncGLObject;

  volContours();
  ~volContours();

  bool visible() const;
  void setVisible(bool vis);

  const std::vector<double>& contourValues() const;
  void setContourValues(const std::vector<double> &vals);

  /**
   * Returns the data pipeline output for the specified level of detail.
   * May return nullptr.
   */
  vtkDataObject* contourData(LevelOfDetail lod) const;

  struct ContourState : public Superclass::ObjectState
  {
    void update(const vvApplicationState &state) override;

    bool visible;
    std::vector<double> contourValues;
  };

  struct ContourLODData : public Superclass::LODData
  {
    vtkSmartPointer<vtkDataObject> contours;
  };

  struct ContourDataPipeline : public Superclass::DataPipeline
  {
    ContourDataPipeline(LevelOfDetail l);

    void configure(const Superclass::ObjectState &objState,
                   const vvApplicationState &appState) override;
    bool needsUpdate(const Superclass::ObjectState &objState,
                     const Superclass::LODData &result) const override;
    void execute() override;
    void exportResult(Superclass::LODData &result) const override;

    LevelOfDetail lod;
    vtkNew<vtkContourFilter> contour;
  };

  struct ContourRenderPipeline : public Superclass::RenderPipeline
  {
    void init(const Superclass::ObjectState &objState,
              vvContextState &contextState) override;
    void update(const Superclass::ObjectState &objState,
                const vvApplicationState &appState,
                const vvContextState &contextState,
                const Superclass::LODData &result) override;
    void disable() override;

    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkActor> actor;
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

#endif // VOLCONTOURS_H
