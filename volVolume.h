#ifndef VOLVOLUME_H
#define VOLVOLUME_H

#include <vvGLObject.h>

#include <vtkNew.h>

class vtkColorTransferFunction;
class vtkPiecewiseFunction;
class vtkSmartVolumeMapper;
class vtkVolume;
class vtkVolumeProperty;

class volVolume : public vvGLObject
{
public:
  using Superclass = vvGLObject;

  enum class RenderMode
    {
    Default = 0,
    RayCastAndTexture,
    RayCast,
    Texture,
    GPU
    };

  struct DataItem : public Superclass::DataItem
  {
    DataItem();

    vtkNew<vtkSmartVolumeMapper> mapper;
    vtkNew<vtkVolume> actor;
  };

  volVolume();
  ~volVolume() override;

  void initVvContext(vvContextState &vvContext,
                     GLContextData &contextData) const override;
  void syncApplicationState(const vvApplicationState &appState) override;
  void syncContextState(const vvApplicationState &appState,
                        const vvContextState &contextState,
                        GLContextData &contextData) const override;

  bool visible() const;
  void setVisible(bool visible);

  RenderMode renderMode() const;
  void setRenderMode(RenderMode val);

private:
  bool m_visible{false};
  RenderMode m_renderMode{RenderMode::GPU};

  vtkNew<vtkColorTransferFunction> m_color;
  vtkNew<vtkPiecewiseFunction> m_opacity;
  vtkNew<vtkVolumeProperty> m_property;
};

#endif // VOLVOLUME_H
