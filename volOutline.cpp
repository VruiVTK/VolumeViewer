#include "volOutline.h"

#include "volApplicationState.h"
#include "volContextState.h"
#include "volReader.h"

#include <GL/GLContextData.h>

#include <vtkActor.h>
#include <vtkDataObject.h>
#include <vtkExternalOpenGLRenderer.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#include <cassert>

//------------------------------------------------------------------------------
volOutline::DataItem::DataItem()
{
  this->actor->SetMapper(this->mapper.Get());
  this->actor->GetProperty()->SetColor(1., 1., 1.);
}

//------------------------------------------------------------------------------
volOutline::volOutline()
{
}

//------------------------------------------------------------------------------
volOutline::~volOutline()
{
}

//------------------------------------------------------------------------------
void volOutline::initVvContext(vvContextState &vvContext,
                               GLContextData &contextData) const
{
  this->Superclass::initVvContext(vvContext, contextData);

  assert("Duplicate context initialization detected!" &&
         !contextData.retrieveDataItem<DataItem>(this));

  DataItem *dataItem = new DataItem;
  contextData.addDataItem(this, dataItem);

  vvContext.renderer().AddActor(dataItem->actor.Get());
}

//------------------------------------------------------------------------------
void volOutline::syncApplicationState(const vvApplicationState &appState)
{
  this->Superclass::syncApplicationState(appState);

  if (m_visible)
    {
    const volApplicationState &state =
        static_cast<const volApplicationState&>(appState);
    m_filter->SetInputData(state.reader().dataObject());
    m_filter->Update();
    m_outline.TakeReference(m_filter->GetOutput()->NewInstance());
    m_outline->ShallowCopy(m_filter->GetOutput());
    }
}

//------------------------------------------------------------------------------
void volOutline::syncContextState(const vvApplicationState &appState,
                                  const vvContextState &contextState,
                                  GLContextData &contextData) const
{
  this->Superclass::syncContextState(appState, contextState, contextData);

  DataItem *dataItem = contextData.retrieveDataItem<DataItem>(this);
  assert(dataItem);

  dataItem->mapper->SetInputDataObject(m_outline.Get());
  dataItem->actor->SetVisibility(m_visible ? 1 : 0);
}

//------------------------------------------------------------------------------
bool volOutline::visible() const
{
  return m_visible;
}

//------------------------------------------------------------------------------
void volOutline::setVisible(bool val)
{
  m_visible = val;
}
