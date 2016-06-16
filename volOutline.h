#ifndef VOLOUTLINE_H
#define VOLOUTLINE_H

#include <vvGLObject.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

class vtkActor;
class vtkDataObject;
class vtkOutlineFilter;
class vtkPolyDataMapper;

class volOutline : public vvGLObject
{
public:
  using Superclass = vvGLObject;

  struct DataItem : public Superclass::DataItem
  {
    DataItem();

    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkActor> actor;
  };

  volOutline();
  ~volOutline() override;

  void initVvContext(vvContextState &vvContext,
                     GLContextData &contextData) const override;
  void syncApplicationState(const vvApplicationState &appState) override;
  void syncContextState(const vvApplicationState &appState,
                        const vvContextState &contextState,
                        GLContextData &contextData) const override;

  bool visible() const;
  void setVisible(bool visible);

private:
  bool m_visible{true};

  vtkNew<vtkOutlineFilter> m_filter;
  vtkSmartPointer<vtkDataObject> m_outline;
};

#endif // VOLOUTLINE_H
