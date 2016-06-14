#ifndef VOLGEOMETRY_H
#define VOLGEOMETRY_H

#include <vvGLObject.h>

#include <vtkNew.h>

class vtkActor;
class vtkDataSetMapper;
class vtkLookupTable;

class volGeometry : public vvGLObject
{
public:
  using Superclass = vvGLObject;

  volGeometry();
  ~volGeometry() override;

  struct DataItem : public Superclass::DataItem
  {
    vtkNew<vtkDataSetMapper> mapper;
    vtkNew<vtkActor> actor;
  };

  enum class Representation
    {
    Points,
    WireFrame,
    Surface,
    SurfaceWithEdges
    };

  void initVvContext(vvContextState &vvContext,
                     GLContextData &contextData) const override;
  void syncApplicationState(const vvApplicationState &appState) override;
  void syncContextState(const vvApplicationState &appState,
                        const vvContextState &contextState,
                        GLContextData &contextData) const override;

  bool visible() const;
  void setVisible(bool visible);

  double opacity() const;
  void setOpacity(double opacity);

  Representation representation() const;
  void setRepresentation(Representation representation);

private:
  bool m_visible;
  double m_opacity;
  Representation m_representation;
  vtkNew<vtkLookupTable> m_color;
};

#endif // VOLGEOMETRY_H
