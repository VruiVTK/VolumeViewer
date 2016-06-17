#include "volVolume.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <GL/GLContextData.h>

#include <vtkColorTransferFunction.h>
#include <vtkDataObject.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

#include <cassert>

//------------------------------------------------------------------------------
volVolume::DataItem::DataItem()
{
  actor->SetMapper(mapper.Get());
}

//------------------------------------------------------------------------------
volVolume::volVolume()
{
  m_property->ShadeOff();
  m_property->SetScalarOpacityUnitDistance(1.0);
  m_property->SetInterpolationTypeToLinear();
  m_property->SetColor(m_color.Get());
  m_property->SetScalarOpacity(m_opacity.Get());
}

//------------------------------------------------------------------------------
volVolume::~volVolume()
{
}

//------------------------------------------------------------------------------
void volVolume::initVvContext(vvContextState &vvContext,
                              GLContextData &contextData) const
{
  this->Superclass::initVvContext(vvContext, contextData);

  assert("Duplicate context initialization detected!" &&
         !contextData.retrieveDataItem<DataItem>(this));

  DataItem *dataItem = new DataItem;
  contextData.addDataItem(this, dataItem);

  dataItem->actor->SetProperty(m_property.Get());
  vvContext.renderer().AddVolume(dataItem->actor.Get());
}

//------------------------------------------------------------------------------
void volVolume::syncApplicationState(const vvApplicationState &appState)
{
  this->Superclass::syncApplicationState(appState);

  // Update color/opacity lookups
  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);
  if (state.reader().dataObject() &&
      state.reader().dataObject()->GetMTime() > m_color->GetMTime())
    {
    m_color->RemoveAllPoints();
    m_opacity->RemoveAllPoints();
    std::array<double, 2> scalarRange = state.reader().scalarRange();
    double step = (scalarRange[1] - scalarRange[0]) / 255.0;
    for (vtkIdType i = 0; i < 255; ++i)
      {
      m_color->AddRGBPoint(scalarRange[0] + (i * step),
                           state.colorMap()[4*i + 0],
                           state.colorMap()[4*i + 1],
                           state.colorMap()[4*i + 2]);
      m_opacity->AddPoint(scalarRange[0] + (i * step),
                          state.colorMap()[4*i + 3]);
      }
    }
}

//------------------------------------------------------------------------------
void volVolume::syncContextState(const vvApplicationState &appState,
                                 const vvContextState &contextState,
                                 GLContextData &contextData) const
{
  this->Superclass::syncContextState(appState, contextState, contextData);

  DataItem *dataItem = contextData.retrieveDataItem<DataItem>(this);
  assert(dataItem);

  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);

  dataItem->mapper->SetInputDataObject(state.reader().dataObject());
  dataItem->actor->SetVisibility(m_visible ? 1 : 0);

  if (m_visible)
    {
    switch (m_renderMode)
      {
      case RenderMode::Default:
        dataItem->mapper->SetRequestedRenderMode(
              vtkSmartVolumeMapper::DefaultRenderMode);
        break;

      case RenderMode::RayCastAndTexture:
        dataItem->mapper->SetRequestedRenderMode(
              vtkSmartVolumeMapper::RayCastAndTextureRenderMode);
        break;

      case RenderMode::RayCast:
        dataItem->mapper->SetRequestedRenderMode(
              vtkSmartVolumeMapper::RayCastRenderMode);
        break;

      case RenderMode::Texture:
        dataItem->mapper->SetRequestedRenderMode(
              vtkSmartVolumeMapper::TextureRenderMode);
        break;

      case RenderMode::GPU:
        dataItem->mapper->SetRequestedRenderMode(
              vtkSmartVolumeMapper::GPURenderMode);
        break;

      default:
        std::cerr << "Unknown render mode." << std::endl;
        break;
      }
    }
}

//------------------------------------------------------------------------------
bool volVolume::visible() const
{
  return m_visible;
}

//------------------------------------------------------------------------------
void volVolume::setVisible(bool val)
{
  m_visible = val;
}

//------------------------------------------------------------------------------
volVolume::RenderMode volVolume::renderMode() const
{
  return m_renderMode;
}

//------------------------------------------------------------------------------
void volVolume::setRenderMode(RenderMode val)
{
  m_renderMode = val;
}
