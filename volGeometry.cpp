#include "volGeometry.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkProperty.h>

#include <GL/GLContextData.h>

#include <vvContextState.h>

#include <cassert>

//------------------------------------------------------------------------------
volGeometry::volGeometry()
  : m_visible(true),
    m_opacity(1.0),
    m_representation(Representation::Surface)
{
  m_color->SetNumberOfColors(256);
  m_color->Build();
}

//------------------------------------------------------------------------------
volGeometry::~volGeometry()
{
}

//------------------------------------------------------------------------------
void volGeometry::initVvContext(vvContextState &vvContext,
                                GLContextData &contextData) const
{
  this->Superclass::initVvContext(vvContext, contextData);

  assert("Duplicate context initialization detected!" &&
         !contextData.retrieveDataItem<DataItem>(this));

  DataItem *dataItem = new DataItem;
  contextData.addDataItem(this, dataItem);

  dataItem->mapper->SetLookupTable(m_color.Get());
  dataItem->mapper->SetColorModeToMapScalars();
  dataItem->actor->SetMapper(dataItem->mapper.Get());
  vvContext.renderer().AddActor(dataItem->actor.Get());
}

//------------------------------------------------------------------------------
void volGeometry::syncApplicationState(const vvApplicationState &appState)
{
  this->Superclass::syncApplicationState(appState);

  const volApplicationState &state =
      static_cast<const volApplicationState&>(appState);
  if (m_color->GetNumberOfTableValues() == 0 ||
      state.colorMapTimeStamp() > m_color->GetMTime())
    {
    const double *data = state.colorMap().data();
    for (vtkIdType i = 0; i < 256; ++i)
      {
      // Don't set opacity in the table -- we set it on the actor's property
      m_color->SetTableValue(i,
                             data[(4*i) + 0],
                             data[(4*i) + 1],
                             data[(4*i) + 2], 1.);
      }
    }
}

//------------------------------------------------------------------------------
void volGeometry::syncContextState(const vvApplicationState &appState,
                                   const vvContextState &contextState,
                                   GLContextData &contextData) const
{
  this->Superclass::syncContextState(appState, contextState, contextData);

  DataItem *dataItem = contextData.retrieveDataItem<DataItem>(this);
  assert(dataItem);

  dataItem->actor->SetVisibility(m_visible ? 1 : 0);
  if (m_visible)
    {
    const volApplicationState &state =
        static_cast<const volApplicationState&>(appState);
    if (state.forceLowResolution())
      {
      dataItem->mapper->SetInputData(state.reader().typedReducedDataObject());
      }
    else
      {
      dataItem->mapper->SetInputData(state.reader().typedDataObject());
      }

    std::array<double, 2> scalarRange = state.reader().scalarRange();
    dataItem->mapper->SetScalarRange(scalarRange.data());

    dataItem->actor->GetProperty()->SetOpacity(m_opacity);
    switch (m_representation)
      {
      case Representation::Points:
        dataItem->actor->GetProperty()->SetRepresentationToPoints();
        dataItem->actor->GetProperty()->SetEdgeVisibility(false);
        break;

      case Representation::WireFrame:
        dataItem->actor->GetProperty()->SetRepresentationToWireframe();
        dataItem->actor->GetProperty()->SetEdgeVisibility(false);
        break;

      case Representation::Surface:
        dataItem->actor->GetProperty()->SetRepresentationToSurface();
        dataItem->actor->GetProperty()->SetEdgeVisibility(false);
        break;

      case Representation::SurfaceWithEdges:
        dataItem->actor->GetProperty()->SetRepresentationToSurface();
        dataItem->actor->GetProperty()->SetEdgeVisibility(true);
        break;
      }
    }
}

//------------------------------------------------------------------------------
bool volGeometry::visible() const
{
  return m_visible;
}

//------------------------------------------------------------------------------
void volGeometry::setVisible(bool v)
{
  m_visible = v;
}

//------------------------------------------------------------------------------
double volGeometry::opacity() const
{
  return m_opacity;
}

//------------------------------------------------------------------------------
void volGeometry::setOpacity(double val)
{
  m_opacity = val;
}

//------------------------------------------------------------------------------
volGeometry::Representation volGeometry::representation() const
{
  return m_representation;
}

//------------------------------------------------------------------------------
void volGeometry::setRepresentation(Representation val)
{
  m_representation = val;
}
